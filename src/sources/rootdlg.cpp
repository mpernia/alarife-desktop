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

#include "../includes/alarife.h"
#include "../includes/rootdlg.h"


RootDlg::RootDlg(QWidget *parent) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    setWindowTitle(tr("Autenticar Editor Jefe"));

    setMinimumSize(350, 200);
    setMaximumSize(350, 200);

    edtPass = new QLineEdit;
    btnOk = new QPushButton(tr("Autenticar"));
    btnCancel = new QPushButton(tr("Cancelar"));

    QSpacerItem *hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding,
                                           QSizePolicy::Minimum);
    QSpacerItem *hSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding,
                                           QSizePolicy::Minimum);
    QSpacerItem *vSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum,
                                           QSizePolicy::Expanding);
    QSpacerItem *vSpacer1 = new QSpacerItem(20, 40, QSizePolicy::Minimum,
                                            QSizePolicy::Expanding);
    edtPass->setEchoMode(QLineEdit::Password);

    btnOk->setMaximumWidth(__BTN_SIZE__);
    btnCancel->setMaximumWidth(__BTN_SIZE__);

    QHBoxLayout *lytPass = new QHBoxLayout;
    lblText = new QLabel(tr("Contraseña:"));
    lytPass->addWidget(lblText);
    lytPass->addWidget(edtPass);
    lytPass->addItem(hSpacer);

    QHBoxLayout *lytBtn = new QHBoxLayout;
    lytBtn->addItem(hSpacer1);
    lytBtn->addWidget(btnOk);
    lytBtn->addWidget(btnCancel);

    QVBoxLayout *lytMain = new QVBoxLayout;
    lblText = new QLabel(tr("<b>Autenticación requerida</b><p>Está intentando realizar una "
                            "acción que necesita permisos especiales. Es necesario "
                            "autenticarse como editor jefe para realizar dicha acción."));
    lblText->setWordWrap(true);
    lytMain->addWidget(lblText);
    lytMain->addItem(vSpacer);
    lytMain->addLayout(lytPass);
    lytMain->addItem(vSpacer1);
    lytMain->addLayout(lytBtn);

    setLayout(lytMain);

    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(btnOk, SIGNAL(clicked()), this, SLOT(checkUser()));
    connect(edtPass, SIGNAL(returnPressed()), this, SLOT(checkUser()));

    access = false;
}

void RootDlg::closeEvent(QCloseEvent *event)
{
    emit isChecked(access);
    event->accept();
}

void RootDlg::checkUser()
{
    QCryptographicHash *md5 = new QCryptographicHash(QCryptographicHash::Md5);
    QByteArray *byte = new QByteArray;

    md5->addData(byte->append(edtPass->text()));
    QSqlQuery *qry = new QSqlQuery;
    qry->exec(QString("SELECT tipousuario, clave "
                      "FROM usuario WHERE tipousuario = 1"));
    qry->first();
    if (qry->value(1).toString() != md5->result().data())
    {
        QMessageBox::critical(this, tr("Autenticación"),
                              tr("Error. Contraseña incorrecta. Inténtelo otra vez"));
        edtPass->clear();
        edtPass->setFocus();
    }else{
        access = true;
        close();
    }
}
