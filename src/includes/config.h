/****************************************************************************
**
** Copyleft: 2012. Maikel Enrique Pernia Matos.
** Contact: corba@grannet.grm.sld.cu
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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QtSql>

#include "alarife.h"
#include "templateview.h"

QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
class QTableView;
class QSqlDatabase;
class QLabel;
class QLineEdit;
class QComboBox;
class QCheckBox;
class QSpacerItem;
class QTextEdit;
class QModelIndex;
QT_END_NAMESPACE

//----------------------------------------------------------------------------------------------
class AparenceTab : public QWidget{
Q_OBJECT
public:
    AparenceTab(QWidget *parent = 0);
    bool getModifique(){return isModifique;}
public slots:
    void ChangeStyle(QString style = "");
    void LoadStyleFiles(QString name = "");
    void ActiveControlStyle(bool active = false);
    void setStyleName(QString style) { strStyle = style; }
    void setStyleSheetName(QString stylesheet) { strStylesheet = stylesheet; }
    void saveAparence();
    void deleteStyle();
    void insertStyle();
    void updateStyles();
private:
    void LoadStyleNames();
    void setAppStyle();
    void setAppStyleSheet();

    QComboBox *cmbStyle,
              *cmbControls,
              *cmbLanguage;

    QCheckBox *chkActiveCtrl;

    QSpacerItem *vSpacer;

    QPushButton *btnDelete,
                *btnEdit,
                *btnNew;

    QString strStyle,
            strStylesheet,
            stylesheet,
            curFile;

    bool isModifique;
};
//----------------------------------------------------------------------------------------------
class UserTab : public QWidget{
Q_OBJECT
public:
    UserTab(QWidget *parent = 0);
private slots:
    void ActiveUserExec(bool mode=false);
    void EditUserExec();
    void InsertUserExec();
    void DeleteUserExec();
    void ShowUser(QModelIndex index);
    void getUserIndex(QModelIndex index);
private:

    void ShowUserValue(bool type = false);

    QSpacerItem *vSpacer;

    QLineEdit *edtPass,
              *edtLogin;

    QCheckBox *chkActiveUser;

    QPushButton *btnEdit,
                *btnIndert,
                *btnDelete;

    QTableView *tblUserView;

    QSqlQueryModel *users;

    QModelIndex idUsr;
};
//----------------------------------------------------------------------------------------------
class TemplateTab : public QWidget{
Q_OBJECT
public:
    TemplateTab(QWidget *parent = 0);
    bool getModifique(){return isModifique;}
private slots:
    void getSelection(QString);
    void deleteTemplates();
    void insertTemplates();
private:
    QLabel *lblText;

    QSpacerItem *vSpacer;

    QPushButton *btnAdd,
                *btnDelete;

    QTextEdit *TemplateList;

    QString name;

    bool isModifique;

    TemplateView *view;
};
//----------------------------------------------------------------------------------------------
class ConfigDialog : public QDialog{
    Q_OBJECT
    public:
        ConfigDialog(QWidget *parent = 0, Book **encyc = 0);
protected:
    void closeEvent(QCloseEvent *event);
    public slots:
        void changePage(QListWidgetItem *current, QListWidgetItem *previous);
    private slots:
        
    private:
        void createIcons();

        QListWidget *lstOptions;

        QStackedWidget *PageControl;

        AparenceTab *aparencetab;

        UserTab *usertab;

        TemplateTab *templatetab;

        Book *encyclopedia;
        bool access;
};
//----------------------------------------------------------------------------------------------

#endif
