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

#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QDialog>

#include "database.h"
#include "alarife.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
class QTableView;
QT_END_NAMESPACE


//----------------------------------------------------------------------------------------------
class loginDlg : public QDialog{
    Q_OBJECT
    public:
        loginDlg(QWidget *parent = 0, Book **encyc = 0);
        ~loginDlg();
    public slots:
        void checkLogin();
        void changeItems(int index = -1);
        void canceLogin();
    protected:
        void closeEvent(QCloseEvent *event);
    signals:
        void closeDlg(bool);
    private:
        QTableView *grid;
        QSqlQueryModel *model;
        QTextEdit *edit;
        QLabel      *lblText;
        QComboBox   *cmbUser;
        QLineEdit   *edtPass;
        QPushButton *btnOk,
                    *btnCancel;
        int usr_type,
            enciclopedia;
        QString usr_name;
        bool islogin;
        Book *encyclopedia;
};
//----------------------------------------------------------------------------------------------
#endif // LOGIN_H
