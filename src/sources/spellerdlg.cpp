/****************************************************************************
**
** Copyleft: 2018. Maikel Enrique Pernia Matos.
** Contact: perniamatos@infomed.sld.cu
**
** This file is part of the Alarife-4 application.
**
** GNU General Public License Usage
**
** This file may be used under the terms of the GNU General Public License;
** either version 2 of the License, or (at your option) any later version,
** as published by the Free Software Foundation and appearing in the file
** LICENSE.GPL included in the packaging of this file.  Please review the
** following information to ensure the GNU General Public License version 2
** requirements will be met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/

#include <QtGui>
#include <QDebug>
#include <QTextCharFormat>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <string>

#include <QDomDocument>
#include <QFile>
#include <QTextStream>

#include "../includes/spellerdlg.h"

SpellerDialog::SpellerDialog(QWidget *parent) : QDialog(parent)
{
    read_language_config();
    lang = (char*)"es";
    spell = new Speller(this, lang);
    read_config();
    create_widgets();
    enable_actions(false);    
    completed = false;

    active_spelling(cmbLanguage->currentText());

    connect(this, SIGNAL(textChanged()), this, SLOT(show_text()));

    connect(cmbLanguage, SIGNAL(currentIndexChanged(QString)), this, SLOT(active_spelling(QString)));

    connect(btnAddWord, SIGNAL(clicked()), this, SLOT(add_word()));
    connect(btnAutoChange, SIGNAL(clicked()), this, SLOT(auto_change_words()));
    connect(btnChange, SIGNAL(clicked()), this, SLOT(change_word()));
    connect(btnChangeAll, SIGNAL(clicked()), this, SLOT(change_all_word()));
    connect(btnIgnore, SIGNAL(clicked()), this, SLOT(ignore_word()));
    connect(btnIgnoreAll, SIGNAL(clicked()), this, SLOT(ignore_all_word()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(cancel()));

    connect(lstSuggestions, SIGNAL(clicked(QModelIndex)), this, SLOT(clicked_list()));
    connect(lstSuggestions, SIGNAL(currentTextChanged(QString)), this, SLOT(current_suggestion_word(QString)));
}

SpellerDialog::~SpellerDialog()
{

}

void SpellerDialog::closeEvent(QCloseEvent *event)
{
    if (blocked == true)
    {
        QMessageBox::information(this,
                                 tr("Cerrar Alarife"),
                                 tr("Actualmente se está realizando un proceso que "
                                    "aún no ha finalizado. Por favor espere..."));
        event->ignore();
    }else{
        if (completed)
        {
            write_config();
            emit textSpelled(changewordList);
        }
        event->accept();
    }
}

void SpellerDialog::setText(QString str)
{
    text = str;
    emit textChanged();
}

void SpellerDialog::active_spelling(QString text)
{
    lang = (char*)languageList[text].toStdString().c_str();

    if (spell != NULL) delete spell;
    spell = new Speller(this, lang);

    QMultiMap <QString, QString>::iterator iterPersonalWord = personalList.find(lang);
    while (iterPersonalWord != personalList.end() && iterPersonalWord.key() == lang)
    {
        spell->add_word(transcode(iterPersonalWord.value()));
        ++iterPersonalWord;
    }

    QMultiMap <QString, QString>::iterator iterIgnoreWord = ignoreList.find(lang);
    while (iterIgnoreWord != ignoreList.end() && iterIgnoreWord.key() == lang)
    {
        spell->ignore_word(transcode(iterIgnoreWord.value()));
        ++iterIgnoreWord;
    }

    emit textChanged();
}

void SpellerDialog::ignore_word()
{
    lastAction.i = 0;
    lastAction.oldText = cursor->selectedText();

    QString key = languageList[cmbLanguage->currentText()];
    if (!ignoreList.contains(key, cursor->selectedText())) ignoreList.insert(key, cursor->selectedText());

    cursor->removeSelectedText();
    cursor->insertText(lastAction.oldText);
    cursor->movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor);
    spellText();
}

void SpellerDialog::ignore_all_word()
{
    lastAction.i = 1;
    lastAction.oldText = cursor->selectedText();

    QString key = languageList[cmbLanguage->currentText()];
    if (!ignoreList.contains(key, cursor->selectedText())) ignoreList.insert(key, cursor->selectedText());

    change_text(lastAction.oldText, Qt::black);
    spellText();
}

void SpellerDialog::add_word()
{
    lastAction.i = 2;
    lastAction.oldText = cursor->selectedText();
    QString key = languageList[cmbLanguage->currentText()];
    if (!personalList.contains(key, cursor->selectedText())) personalList.insert(key, cursor->selectedText());

    spell->add_word(transcode(cursor->selectedText()));
    change_text(lastAction.oldText, Qt::black);
    cursor->movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor);
    spellText();
}

void SpellerDialog::change_word()
{
    lastAction.i = 3;
    lastAction.oldText = cursor->selectedText();
    lastAction.newText = currentWord;

    update_change_list();

    cursor->removeSelectedText();
    cursor->insertText(currentWord);
    spellText();
}

void SpellerDialog::change_all_word()
{
    lastAction.i = 4;
    lastAction.oldText = cursor->selectedText();
    lastAction.newText = currentWord;

    update_change_list();

    QTextCursor *cur = new QTextCursor(edtText->document());
    cur->movePosition(QTextCursor::Start);

    QTextCharFormat plainFormat(cur->charFormat());
    plainFormat.setForeground(Qt::black);

    while(!cur->atEnd())
    {
        cur->select(QTextCursor::WordUnderCursor);
        if(cur->selectedText() == lastAction.oldText)
        {
            cur->removeSelectedText();
            cur->insertText(lastAction.newText);
        }
        cur->movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor);
    }

    spellText();

}

void SpellerDialog::auto_change_words()
{
    lastAction.i = 5;
    lastAction.oldText = cursor->selectedText();
    lastAction.newText = currentWord;

    QString key = languageList[cmbLanguage->currentText()];
    autoChangeList.insert(key, lastAction);

    update_change_list();

    cursor->removeSelectedText();
    cursor->insertText(currentWord);
    spellText();
}


void SpellerDialog::spelling()
{
    blocked = true;
    cursor = new QTextCursor(edtText->document());

    scanText();
    spellText();
}

void SpellerDialog::scanText()
{
    cursor->movePosition(QTextCursor::Start);
    QTextCharFormat plainFormat(cursor->charFormat());
    QTextCharFormat colorFormat = plainFormat;
    colorFormat.setForeground(Qt::red);

    while (!cursor->atEnd())
    {
        cursor->select(QTextCursor::WordUnderCursor);
        if (spell->check_word(transcode(cursor->selectedText())) == 0) cursor->mergeCharFormat(colorFormat);
        cursor->movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor);
    }

    cursor->movePosition(QTextCursor::Start);
    position = cursor->position();
}


void SpellerDialog::spellText()
{
    enable_actions(false);
    cursor->setPosition(position, QTextCursor::MoveAnchor);

    if (position > 0 && position < text.length()) cursor->movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor);

    while (!cursor->atEnd())
    {
        cursor->select(QTextCursor::WordUnderCursor);
        if (spell->check_word(transcode(cursor->selectedText())) == 0)
        {
            position = cursor->position();
            enable_actions(true, 1);
            break;
        }
        cursor->movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor);
    }

    if (cursor->atEnd() && !spell->check_word(transcode(cursor->selectedText())) == 0)
    {
        QMessageBox::information(this,
                                 tr("Corrector Ortográfico"),
                                 tr("La revisión del texto ha finalizado exitosamente."));

        completed = true;
        blocked = false;
        enable_actions(false);
        btnCancel->setText(tr("Cerrar"));

    }

    if (cursor->hasSelection())
    {
        lstSuggestions->clear();
        lstSuggestions->addItems(spell->get_suggestions_lists(transcode(cursor->selectedText())));
    }
}

void SpellerDialog::show_text()
{
    edtText->clear();
    lstSuggestions->clear();

    edtText->setText(text);

    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(spelling()));
    timer->start(1500);
}

void SpellerDialog::cancel()
{
    blocked = false;
    close();
}

void SpellerDialog::current_suggestion_word(QString text)
{
    currentWord = text;
}

void SpellerDialog::clicked_list()
{
    enable_actions(true, 2);
}

void SpellerDialog::enable_actions(bool mode, int x)
{
    switch (x) {
    case 1 :
                btnAddWord->setEnabled(mode);
                btnIgnore->setEnabled(mode);
                btnIgnoreAll->setEnabled(mode);
        break;
    case 2 :
                btnChange->setEnabled(mode);
                btnChangeAll->setEnabled(mode);
                btnAutoChange->setEnabled(mode);
        break;
    default:
                btnAddWord->setEnabled(mode);
                btnAutoChange->setEnabled(mode);
                btnChange->setEnabled(mode);
                btnChangeAll->setEnabled(mode);
                btnIgnore->setEnabled(mode);
                btnIgnoreAll->setEnabled(mode);
        break;
    }
}

void SpellerDialog::create_widgets()
{
    lblLanguage = new QLabel(tr("Idioma del texto:"));
    QSpacerItem *hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    cmbLanguage = new QComboBox;
    cmbLanguage->setMinimumWidth(200);

    QMapIterator<QString, QString> iterLang(languageList);
    while (iterLang.hasNext())
    {
        iterLang.next();
        cmbLanguage->addItem(iterLang.key());
    }

    QHBoxLayout *lytTop = new QHBoxLayout;
    lytTop->addWidget(lblLanguage);
    lytTop->addItem(hSpacer);
    lytTop->addWidget(cmbLanguage);

    edtText = new QTextEdit;
    edtText->setReadOnly(true);

    lstSuggestions = new QListWidget;

    QSpacerItem *vSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *vSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    btnIgnore = new QPushButton(tr("Ignorar una vez"));
    btnIgnoreAll = new QPushButton(tr("Ignorar todo"));
    btnAddWord = new QPushButton(tr("Añadir al diccionario"));

    QVBoxLayout *lytText = new QVBoxLayout;
    lytText->addItem(vSpacer_2);
    lytText->addWidget(btnIgnore);
    lytText->addWidget(btnIgnoreAll);
    lytText->addWidget(btnAddWord);
    lytText->addItem(vSpacer_3);

    QSpacerItem *vSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *vSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    btnChange = new QPushButton(tr("Cambiar"));
    btnChangeAll = new QPushButton(tr("Cambiar todo"));
    btnAutoChange = new QPushButton(tr("Siempre cambiar"));

    QVBoxLayout *verticalLayout_4 = new QVBoxLayout;
    verticalLayout_4->addItem(vSpacer_4);
    verticalLayout_4->addWidget(btnChange);
    verticalLayout_4->addWidget(btnChangeAll);
    verticalLayout_4->addWidget(btnAutoChange);
    verticalLayout_4->addItem(vSpacer_5);

    lblText = new QLabel(tr("No está en el diccioario"));
    lblSuggestions = new QLabel(tr("Sugerencias:"));

    QGridLayout *lytCentral = new QGridLayout;
    lytCentral->addWidget(lblText, 0, 0, 1, 1);
    lytCentral->addWidget(edtText, 1, 0, 1, 1);
    lytCentral->addLayout(lytText, 1, 1, 1, 1);
    lytCentral->addLayout(verticalLayout_4, 3, 1, 1, 1);
    lytCentral->addWidget(lblSuggestions, 2, 0, 1, 1);
    lytCentral->addWidget(lstSuggestions, 3, 0, 1, 1);

    QSpacerItem *hSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    btnCancel = new QPushButton(tr("Cancelar"));

    QHBoxLayout *lytBottom = new QHBoxLayout();
    lytBottom->addItem(hSpacer_2);
    lytBottom->addWidget(btnCancel);

    QVBoxLayout *lytMain = new QVBoxLayout;
    lytMain->addLayout(lytTop);
    lytMain->addLayout(lytCentral);
    lytMain->addLayout(lytBottom);

    this->setLayout(lytMain);
    this->resize(500, 260);
    this->setWindowTitle(tr("Corrector ortográfico"));

    blocked = false;
}

void SpellerDialog::change_text(QString str, QBrush brush)
{
    QTextCursor *cur = new QTextCursor(edtText->document());
    cur->movePosition(QTextCursor::Start);

    QTextCharFormat plainFormat(cur->charFormat());
    plainFormat.setForeground(brush);

    while(!cur->atEnd())
    {
        cur->select(QTextCursor::WordUnderCursor);
        if(cur->selectedText() == str) cur->mergeCharFormat(plainFormat);
        cur->movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor);
    }
}

void SpellerDialog::write_config()
{
    QStringList list;

    QMapIterator<QString, QString> iterLang(languageList);
    while (iterLang.hasNext())
    {
        iterLang.next();
        list.append(iterLang.value());
    }

    QDomDocument docAspell;

    QString header = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n"
                     "<!--\n"
                     "==================================================\n"
                     "         " __APP_NAME__ "    Version: " __APP_VERSION__ "\n"
                     "==================================================\n"
                     " (c) 2011-2018 by Maikel Enrique Pernia Matos.\n\n"
                     " This file may be used under the terms of the GNU \n"
                     " General Public License version 2 as published by \n"
                     " the Free Software Foundation.\n\n"
                     "==================================================\n-->\n\n";

    QDomElement eleAspell = docAspell.createElement("aspell");

    for (int i = 0; i < list.count(); i++)
    {
        QDomElement eleLang = docAspell.createElement("lang");
        eleLang.setAttribute("name", list.at(i));
        eleAspell.appendChild(eleLang);

        QDomElement eleDict = docAspell.createElement("dictionary");
        eleLang.appendChild(eleDict);

        QMultiMap<QString, QString>::iterator iterPersonalWord = personalList.find(list.at(i));
        while (iterPersonalWord != personalList.end() && iterPersonalWord.key() == list.at(i))
        {
            QDomElement eleWord = docAspell.createElement("word");
            eleWord.setAttribute("text", iterPersonalWord.value());
            eleDict.appendChild(eleWord);
            ++iterPersonalWord;
        }

        QDomElement eleIgnore = docAspell.createElement("ignore");
        eleLang.appendChild(eleIgnore);

        QMultiMap<QString, QString>::iterator iterIgnoreWord = ignoreList.find(list.at(i));
        while (iterIgnoreWord != ignoreList.end() && iterIgnoreWord.key() == list.at(i))
        {
            QDomElement eleWord = docAspell.createElement("word");
            eleWord.setAttribute("text", iterIgnoreWord.value());
            eleIgnore.appendChild(eleWord);
            ++iterIgnoreWord;
        }

        QDomElement eleAutoChange = docAspell.createElement("autochange");
        eleLang.appendChild(eleAutoChange);

        QMultiMap<QString, Action>::iterator iterAutoChangeWord = autoChangeList.find(list.at(i));
        while (iterAutoChangeWord != autoChangeList.end() && iterAutoChangeWord.key() == list.at(i))
        {
            Action act = iterAutoChangeWord.value();
            QDomElement eleWord = docAspell.createElement("word");
            eleWord.setAttribute("text1", act.oldText);
            eleWord.setAttribute("text2", act.newText);
            eleAutoChange.appendChild(eleWord);
            ++iterAutoChangeWord;
        }
    }

    docAspell.appendChild(eleAspell);

    QFile file(FILE_PATH);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream << header << docAspell.toString(4);
}

void SpellerDialog::read_config()
{
    QDomDocument docAspell("aspell");
    QFile file(FILE_PATH);

    docAspell.setContent(&file);

    QDomElement eleAspell = docAspell.documentElement();

    QDomNode node = eleAspell.firstChild();

    while(!node.isNull())
    {
        QDomElement childNode = node.toElement();

        if (childNode.toElement().tagName() == "lang")
        {
            QString lng = childNode.attribute("name", "");

            QDomElement eleDict = childNode.toElement().firstChildElement();
            while (!eleDict.isNull())
            {
                if (eleDict.toElement().tagName() == "dictionary")
                {
                    QDomElement eleWord = eleDict.toElement().firstChildElement();
                    while (!eleWord.isNull())
                    {
                        QString word = eleWord.attribute("text", "");
                        personalList.insert(lng, word);
                        eleWord = eleWord.nextSiblingElement();
                    }
                }

                if (eleDict.toElement().tagName() == "ignore")
                {
                    QDomElement eleWord = eleDict.toElement().firstChildElement();
                    while (!eleWord.isNull())
                    {
                        QString word = eleWord.attribute("text", "");
                        ignoreList.insert(lng, word);
                        eleWord = eleWord.nextSiblingElement();
                    }
                }

                if (eleDict.toElement().tagName() == "autochange")
                {
                    QDomElement eleWord = eleDict.toElement().firstChildElement();
                    while (!eleWord.isNull())
                    {
                        Action act;
                        act.oldText = eleWord.attribute("text1", "");
                        act.newText = eleWord.attribute("text2", "");
                        autoChangeList.insert(lng, act);
                        eleWord = eleWord.nextSiblingElement();
                    }
                }

                eleDict = eleDict.nextSiblingElement();
            }
        }
        node = node.nextSibling();
    }

    file.close();
}

void SpellerDialog::read_language_config()
{
    QString path = __APP_PATH__;
    if (path.contains("/bin")) path.remove("/bin", Qt::CaseSensitive);

    QDomDocument docAspell("aspell");
    QFile file(path + "/qjseditor/lang/aspell.config");

    docAspell.setContent(&file);

    QDomElement eleAspell = docAspell.documentElement();

    QDomNode node = eleAspell.firstChild();

    while(!node.isNull())
    {
        QDomElement childNode = node.toElement();

        if (childNode.toElement().tagName() == "dic")
        {
            QString langName = childNode.attribute("name", "");
            QString langCode = childNode.attribute("code", "");
            languageList.insert(langName, langCode);
        }
        node = node.nextSibling();
    }
    file.close();
}

void SpellerDialog::update_change_list()
{
    if (!changewordList.contains(lastAction.oldText))
    {
        changewordList.insert(lastAction.oldText, lastAction.newText);
    }
}

QString SpellerDialog::transcode(QString str)
{
    #if defined(Q_OS_LINUX)
        return  str.toUtf8();
    #elif defined(Q_OS_WIN)
        return str.toLatin1();
    #endif
}
