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

#ifndef ROOTDLG_H
#define ROOTDLG_H

#include <QWidget>
#include <QDialog>

#include "config.h"
#include "database.h"
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

//----------------------------------------------------------------------------------------------
class RootDlg : public QDialog{
    Q_OBJECT
    public:
        RootDlg(QWidget *parent = 0);
    protected:
        void closeEvent(QCloseEvent *event);
    signals:
        void isChecked(bool);
    private slots:
        void checkUser();
    private:
        QLabel *lblText;
        QLineEdit *edtPass;
        QPushButton *btnCancel,
                    *btnOk;

        bool access;
};

#endif
