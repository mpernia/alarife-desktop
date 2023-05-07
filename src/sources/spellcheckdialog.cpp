#include <QDebug>
#include <QTextCharFormat>
#include <QMessageBox>
#include <QtGui>


#include "../includes/alarife.h"
#include "../includes/spellcheckdialog.h"

SpellCheckDialog::SpellCheckDialog(QString doc, QWidget *parent) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    create_widgets();

    textEdit->setText(doc);

    cur = new QTextCursor(textEdit->document());

    spell_config = new_aspell_config();

    aspell_config_replace(spell_config, "lang", "es");

    possible_err = new_aspell_speller(spell_config);
    spell_checker = 0;
    if (aspell_error_number(possible_err) != 0)
    {
        QMessageBox::question(this,
                              tr("Corrector Ortográfico"),
                              QString::fromAscii(aspell_error_message(possible_err)));
        return;
    }else
        spell_checker = to_aspell_speller(possible_err);

    checkAll();
    checkNext();

    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(changeWord, SIGNAL(clicked()), this, SLOT(change_word()));
    connect(omitWord, SIGNAL(clicked()), this, SLOT(ignore_word()));
    connect(acept, SIGNAL(clicked()), this, SLOT(finish()));
}

SpellCheckDialog::~SpellCheckDialog()
{

}

QString SpellCheckDialog::error()
{
    AspellConfig *spell_config = new_aspell_config();
    aspell_config_replace(spell_config, "lang", "es");
    AspellCanHaveError *possible_err = new_aspell_speller(spell_config);

    if (aspell_error_number(possible_err) != 0)
        return aspell_error_message(possible_err);
    else
        return "";
}

bool SpellCheckDialog::isNumber(QString str)
{ 
    for (int i=0; i< str.size(); i++)
    {
        if(str.at(i).isDigit())
            return true;
    }
    return false;
}

void SpellCheckDialog::checkAll()
{
    cur->movePosition(QTextCursor::Start);
    QTextCharFormat plainFormat(cur->charFormat());
    QTextCharFormat colorFormat = plainFormat;
    colorFormat.setForeground(Qt::red);

    while(!cur->atEnd()){
        cur->select(QTextCursor::WordUnderCursor);

        if(aspell_speller_check(spell_checker, cur->selectedText().toStdString().c_str(), -1) == 0
                && !isOk(cur->selectedText()) && !isNumber(cur->selectedText())){
            cur->mergeCharFormat(colorFormat);
        }

        cur->movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor);
    }
}

bool SpellCheckDialog::isOk(QString str)
{
    const AspellWordList *suggestions;
    AspellStringEnumeration *elements;
    const char *word;
    QString strWord;

    suggestions = aspell_speller_suggest(spell_checker, str.toStdString().c_str() , -1);
    elements = aspell_word_list_elements(suggestions);

    while((word = aspell_string_enumeration_next(elements)) != NULL ){
        strWord = tr(word);
        if(str == strWord){
            delete_aspell_string_enumeration(elements);
            return true;
        }
    }
    return false;
}

void SpellCheckDialog::checkNext()
{
    cur->movePosition(QTextCursor::Start);

    while(!cur->atEnd())
    {
        cur->select(QTextCursor::WordUnderCursor);
        if(aspell_speller_check(spell_checker, cur->selectedText().toStdString().c_str(), -1) == 0
                && !isOmitWord(cur->selectedText()) && !isOk(cur->selectedText()) && !isNumber(cur->selectedText()))
            break;
        cur->movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor);
    }

    if(cur->atEnd())
    {
        QMessageBox::information(this, tr("Corrector Ortográfico"), tr("Correción ortográfica finalizada"));
        changeWord->setEnabled(false);
        return;
    }

    if(cur->hasSelection())
    {
        showSugList(cur->selectedText());

        if(sugList->count() == 0)
        {
            omitidas << cur->selectedText();
            checkNext();
        }else{
            textEdit->find(cur->selectedText());
        }
    }

}

void SpellCheckDialog::create_widgets()
{
    textEdit = new QTextEdit;
    textEdit->setEnabled(false);
    textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    word = new QLabel(tr("Palabra:"));

    sugList = new QListWidget;
    sugList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    changeWord = new QPushButton(tr("Cambiar"));
    omitWord = new QPushButton(tr("Omitir"));
    acept = new QPushButton(tr("Aceptar"));
    cancel = new QPushButton(tr("Cancelar"));

    changeWord->setEnabled(true);

    QVBoxLayout *lytButtons = new QVBoxLayout();
    lytButtons->addWidget(changeWord);
    lytButtons->addWidget(omitWord);
    lytButtons->addWidget(acept);
    lytButtons->addWidget(cancel);

    QHBoxLayout *lytCentral = new QHBoxLayout;
    lytCentral->addWidget(sugList);
    lytCentral->addLayout(lytButtons);

    QVBoxLayout *lytMain = new QVBoxLayout;
    lytMain->addWidget(textEdit);
    lytMain->addWidget(word);
    lytMain->addLayout(lytCentral);

    setLayout(lytMain);
    setWindowTitle(tr("Corrector Ortográfico"));
}

void SpellCheckDialog::ignore_word()
{
    omitidas << cur->selectedText();
    QTextCharFormat format(cur->charFormat());
    format.setForeground(Qt::black);
    cur->mergeCharFormat(format);
    checkNext();
}

bool SpellCheckDialog::isOmitWord(QString str)
{
    foreach (QString word, omitidas)
    {
        if(word == str) return true;
    }

    return false;
}

void SpellCheckDialog::change_word()
{
    QString currentWord = cur->selectedText();
    QString replaceWord;

    if(sugList->currentItem() != NULL)
    {
        cur->removeSelectedText();
        replaceWord = sugList->currentItem()->text();
        cur->insertText(replaceWord);
        forReplace <<currentWord + ":" + replaceWord;
        omitidas << replaceWord;
        changeAllWords(currentWord, replaceWord);
        checkNext();
    }
}

void SpellCheckDialog::changeAllWords(QString word, QString str)
{
    cur->movePosition(QTextCursor::Start);
    QTextCharFormat plainFormat(cur->charFormat());
    QTextCharFormat colorFormat = plainFormat;
    plainFormat.setForeground(Qt::black);
    colorFormat.setForeground(Qt::red);

    while(!cur->atEnd())
    {
        cur->select(QTextCursor::WordUnderCursor);

        if(cur->selectedText() == word)
        {
            cur->mergeCharFormat(plainFormat);
            cur->insertText(str);
        }

        cur->movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor);
    }
}

void SpellCheckDialog::showSugList(QString str)
{
    const AspellWordList *suggestions;
    AspellStringEnumeration *elements;

    word->setText(str);
    suggestions = aspell_speller_suggest(spell_checker, str.toStdString().c_str(), -1);
    elements = aspell_word_list_elements(suggestions);
    sugList->clear();
    const char *word;

#ifdef Q_OS_LINUX
    while((word = aspell_string_enumeration_next(elements)) != NULL )
    {
        sugList->addItem(tr(word));
    }
#elif defined(Q_OS_WIN32)
    while((word = aspell_string_enumeration_next(elements)) != NULL ){
        sugList->addItem(QString::fromAscii(word));
    }
#endif

    delete_aspell_string_enumeration(elements);
}

void SpellCheckDialog::finish()
{
    for (int i = 0; i < forReplace.count(); i++) {
        qDebug() << "replace" << forReplace.at(i) << endl;
    }
    emit aceptado(forReplace);
    close();
}
