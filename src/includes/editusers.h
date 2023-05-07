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

#ifndef EDITUSERS_H
#define EDITUSERS_H

#include <QtGui>
#include <QDialog>
#include <QWidget>
#include <QtSql>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QComboBox;
class QSpacerItem;
QT_END_NAMESPACE

//----------------------------------------------------------------------------------------------
class EditUsers : public QDialog{
Q_OBJECT
public:
    explicit EditUsers(QWidget *parent = 0, int id = 0);
signals:
    void refreshGrid(bool);
private slots:
    void ChangeValues();
private:
    QLabel *lblText;

    QLineEdit *edtName,
              *edtLogin,
              *edtPass;

    QComboBox *cmbTypeUsr;

    QPushButton *btnChange;

    int mode;
};

#endif // EDITUSERS_H
