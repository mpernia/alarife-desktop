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

#include <QtGui>
#include <QtSql>
#include <QSpacerItem>
#include <QCryptographicHash>
#include <iostream>

using namespace std;

#include "../includes/login.h"
#include "../includes/alarife.h"
#include "../includes/database.h"



loginDlg::loginDlg(QWidget *parent, Book **encyc)  : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__); 

    islogin = false;

    setWindowTitle(tr("Selección de usuario"));
    setMinimumSize(350, 180);
    setMaximumSize(350, 180);

    edtPass = new QLineEdit;
    cmbUser = new QComboBox;

    edtPass->setEchoMode(QLineEdit::Password);
    cmbUser->addItems(QStringList() << "");

    btnCancel = new QPushButton(tr("Cancelar"));
    btnOk = new QPushButton(tr("Aceptar"));

    QSqlQuery *qry = new QSqlQuery;
    qry->exec("SELECT nombre FROM usuario");

    while (qry->next())
    {
        cmbUser->addItems(QStringList() << qry->value(0).toString());
    }
    delete qry;

    QSpacerItem *vSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum,
                                                  QSizePolicy::Expanding);
    QPixmap image;
    image = QPixmap(":/images/conf_user.png");

    QLabel *lblImage = new QLabel;
    lblImage->setPixmap(image.scaled(QSize(85,85),
                                    Qt::IgnoreAspectRatio,
                                    Qt::FastTransformation));

    QLabel *lblUser = new QLabel(tr("Usuarios:"));
    QLabel *lblPass = new QLabel(tr("Contraseña:"));


    QVBoxLayout *lytMain = new QVBoxLayout(this);

    QHBoxLayout *lytCentral = new QHBoxLayout();
    lytCentral->addWidget(lblImage);

    QVBoxLayout *lytUsers = new QVBoxLayout();
    lytUsers->addWidget(lblUser);
    lytUsers->addWidget(cmbUser);
    lytUsers->addWidget(lblPass);
    lytUsers->addWidget(edtPass);
    lytUsers->addItem(vSpacer);

    lytCentral->addLayout(lytUsers);
    lytMain->addLayout(lytCentral);

    QHBoxLayout *lytButtons = new QHBoxLayout();
    QSpacerItem *hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    lytButtons->addItem(hSpacer);
    lytButtons->addWidget(btnCancel);
    lytButtons->addWidget(btnOk);

    lytMain->addLayout(lytButtons);

    setLayout(lytMain);

    setTabOrder(cmbUser, edtPass);
    setTabOrder(edtPass, btnOk);
    setTabOrder(btnOk, btnCancel);

    connect(btnOk, SIGNAL(clicked()), this, SLOT(checkLogin()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(canceLogin()));
    connect(cmbUser,SIGNAL(currentIndexChanged(int)), this, SLOT(changeItems(int)));
    connect(edtPass, SIGNAL(returnPressed()), this, SLOT(checkLogin()));

    encyclopedia = (*encyc);
    enciclopedia = encyclopedia->getIndex();
    usr_type = 0;
}

loginDlg::~loginDlg()
{

}


void loginDlg::changeItems(int index)
{
    usr_type = index;
    usr_name = cmbUser->currentText();
    if (usr_name.contains("'")) usr_name.replace("'", "\''", Qt::CaseSensitive);
}

void loginDlg::closeEvent(QCloseEvent *event)
{
    emit closeDlg(islogin);
    event->accept();
}

void loginDlg::checkLogin()
{
    if(usr_type == 0)
    {
        QMessageBox::critical(this, tr("Selección de usuario"),
                              tr("Error. Debe seleccionar un usuario "
                                 "y su contraseña correspondiente."), QMessageBox::Cancel);
    }else{
        QCryptographicHash *md5 = new QCryptographicHash(QCryptographicHash::Md5);
        QByteArray *byte = new QByteArray;

        md5->addData(byte->append(edtPass->text()));
        QSqlQuery *qry = new QSqlQuery;
        qry->exec(QString("SELECT id_usuario, clave "
                          "FROM usuario WHERE nombre = '%1'").arg(usr_name));
        qry->first();

        if (qry->value(1).toString() != md5->result().data()){
            QMessageBox::critical(this, tr("Selección de usuario"),
                                  tr("Error. Contraseña incorrecta. Inténtelo otra vez"));
            edtPass->clear();
            edtPass->setFocus();
        }else{
            encyclopedia->setUserIndex(qry->value(0).toInt());
            islogin = true;
            close();
        }
    }
}

void loginDlg::canceLogin()
{
    islogin = false;
    close();
}

