#ifndef SPELLCHECKDIALOG_H
#define SPELLCHECKDIALOG_H

#include <QtGui>
#include <QDialog>
#include <QTextCursor>
#include <QModelIndex>

#include "aspell.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QTextCursor;
class QListWidget;
QT_END_NAMESPACE

class SpellCheckDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SpellCheckDialog(QString doc, QWidget *parent = 0);
    ~SpellCheckDialog();
    QString error();

private slots:
    void ignore_word();
    void change_word();
    void finish();

private:
    void create_widgets();
    void checkAll();
    void checkNext();
    void showSugList(QString w);
    void changeAllWords(QString ,QString);
    bool isOmitWord(QString w);
    bool isOk(QString );
    bool isNumber(QString );

    AspellConfig *spell_config;
    AspellSpeller *spell_checker;
    AspellCanHaveError *possible_err;
    QTextCursor *cur;
    QStringList omitidas;
    QStringList forReplace;


    QTextEdit *textEdit;
    QLabel *word;
    QListWidget *sugList;
    QPushButton *changeWord;
    QPushButton *omitWord;
    QPushButton *acept;
    QPushButton *cancel;

signals:
    void aceptado(QStringList);
};

#endif // SPELLCHECKDIALOG_H
