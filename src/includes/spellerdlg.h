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

#ifndef SPELLERDLG_H
#define SPELLERDLG_H

#include <QtGui>
#include <QDialog>
#include <QTextCursor>

#include "alarife.h"
#include "speller.h"

#define FILE_PATH QDir::homePath() + "/Alarife/aspell.dic"

class SpellerDialog : public QDialog
{
    Q_OBJECT

public:
    SpellerDialog(QWidget *parent = 0);
    ~SpellerDialog();
    void setText(QString);
protected:
    void closeEvent(QCloseEvent *event);

signals:
    void textChanged();
    void textSpelled(QMap<QString, QString>);

private slots:
    void active_spelling(QString);
    // void active_undo_option();
    void ignore_word();
    void ignore_all_word();
    void add_word();
    void change_word();
    void change_all_word();
    void auto_change_words();
    // void undo_spelling();
    void spelling();
    void show_text();
    void cancel();
    void current_suggestion_word(QString);
    void clicked_list();

private:
    void enable_actions(bool, int x = 0);
    void create_widgets();
    void scanText();
    void spellText();
    void change_text(QString, QBrush);
    void write_config();
    void read_config();
    void read_language_config();
    void update_change_list();
    QString transcode(QString);

    QLabel *lblLanguage;
    QLabel *lblText;
    QLabel *lblSuggestions;
    QComboBox *cmbLanguage;
    QTextEdit *edtText;
    QListWidget *lstSuggestions;
    QPushButton *btnIgnore;
    QPushButton *btnIgnoreAll;
    QPushButton *btnAddWord;
    QPushButton *btnChange;
    QPushButton *btnChangeAll;
    QPushButton *btnAutoChange;
    QPushButton *btnCancel;

    QTextCursor *cursor;

    bool blocked;
    bool completed;

    typedef struct {
        int i = 0;
        QString oldText = "";
        QString newText = "";
    } Action;

    QMap <QString, QString> languageList;

    QMap <QString, QString> changewordList;

    QMultiMap <QString, QString> personalList;

    QMultiMap <QString, QString> ignoreList;

    QMultiMap <QString, Action> autoChangeList;

    Action lastAction;

    QString text;

    QString currentWord;

    Speller *spell;

    int position;

    char *lang;
};

#endif // SPELLERDLG_H
