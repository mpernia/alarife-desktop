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

#include "../includes/editusers.h"
#include "../includes/database.h"
#include "../includes/sqlfunctions.h"

EditUsers::EditUsers(QWidget *parent, int id) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    mode = id;
    if (mode == 0)
        setWindowTitle(tr("Inserción de usuario"));
    else if (mode > 0)
        setWindowTitle(tr("Edición de usuario"));

    setMinimumSize(380, 215);
    setMaximumSize(380, 215);

    edtName = new QLineEdit;
    edtLogin = new QLineEdit;
    edtPass = new QLineEdit;
    edtPass->setEchoMode(QLineEdit::Password);

    edtPass->setToolTip(tr("Cambiar contraseña."));

    cmbTypeUsr = new QComboBox;
    cmbTypeUsr->addItems(QStringList() <<""<< tr("Editor Jefe") << tr("Editor"));

    btnChange = new QPushButton(tr("Aceptar"));

    QSpacerItem *vSpacer1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *vSpacer2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QPixmap photo;
    photo = QPixmap(":/images/0.gif");
    lblText = new QLabel;
    lblText->setPixmap(photo.scaled(QSize(100,100),
                                  Qt::IgnoreAspectRatio,
                                  Qt::FastTransformation));

    QVBoxLayout *lytImage = new QVBoxLayout;
    lytImage->addItem(vSpacer1);
    lytImage->addWidget(lblText);
    lytImage->addItem(vSpacer2);

    QHBoxLayout *lytButton = new QHBoxLayout;
    lytButton->addItem(hSpacer);
    lytButton->addWidget(btnChange);

    QGridLayout *lytUser = new QGridLayout;
    lblText = new QLabel(tr("usuario:"));
    lytUser->addWidget(lblText,0,0);
    lblText = new QLabel(tr("Nueva contraseña:"));
    lytUser->addWidget(lblText,0,1);
    lytUser->addWidget(edtLogin,1,0);
    lytUser->addWidget(edtPass,1,1);

    QVBoxLayout *lytData = new QVBoxLayout;
    lblText = new QLabel(tr("Nombre completo:"));
    lytData->addWidget(lblText);
    lytData->addWidget(edtName);
    lytData->addLayout(lytUser);
    lblText = new QLabel(tr("Tipo de usuario:"));
    lytData->addWidget(lblText);
    lytData->addWidget(cmbTypeUsr);
    lytData->addLayout(lytButton);


    QHBoxLayout *lytMain = new QHBoxLayout;
    lytMain->addLayout(lytImage);
    lytMain->addLayout(lytData);

    connect(btnChange, SIGNAL(clicked()), this, SLOT(ChangeValues()));

    setLayout(lytMain);

    if (id > 0)
    {
        QSqlQuery *qryUsr = new QSqlQuery;
        qryUsr->exec(QString("SELECT nombre, usuario, clave, tipousuario "
                             "FROM usuario WHERE id_usuario = %1").arg(id));
        qryUsr->first();
        edtName->setText(qryUsr->value(0).toString());
        edtLogin->setText(qryUsr->value(1).toString());

        switch (qryUsr->value(3).toInt())
        {
            case 1: cmbTypeUsr->setCurrentIndex(1);
                    break;
            case 2: cmbTypeUsr->setCurrentIndex(2);
                    break;
        }
    }
}

void EditUsers::ChangeValues()
{
    int tipousuario = 0;
    if (cmbTypeUsr->currentText() == "Editor Jefe" )
    {
        QSqlQuery *qry_2 = new QSqlQuery;
        qry_2->exec("SELECT COUNT(tipousuario) FROM usuario WHERE tipousuario = 1");
        qry_2->first();

        if (qry_2->value(0).toInt() == 1 && mode == 0)
        {
            QMessageBox::critical(this, tr("Insertar usuario"),
                                  tr("Error. En esta Enciclopedia ya "
                                     "existe un Editor Jefe."));
            return;
        } else
            tipousuario = 1;

    }
    else if (cmbTypeUsr->currentText() == "Editor")
        tipousuario = 2;

    QCryptographicHash *md5 = new QCryptographicHash(QCryptographicHash::Md5);
    QByteArray *byte = new QByteArray;
    md5->addData(byte->append(edtPass->text()));

    if (mode == 0)
    {
        if (sqlInsertRecord_N("usuario",
                            "id_usuario, nombre, usuario, clave, tipousuario",
                            QString("%1, <&~%2~&>, <&~%3~&>, <&~%4~&>, %5").
                                    arg(sqlAutoIncremental("id_usuario", "usuario")).
                                    arg(edtName->text()).
                                    arg(edtLogin->text()).
                                    arg(md5->result().data()).
                                    arg(tipousuario)))
        {
            emit refreshGrid(true);
            close();
        }else
            QMessageBox::critical(this, tr("Insertar usuario"),
                                     tr("Error, inserción de usuario fallida."));
    }else if (mode > 0){

        if (sqlUpdateRecord_N("usuario",
                            QString("nombre = <&~%1~&>, usuario = <&~%2~&>, tipousuario = %3").
                                    arg(edtName->text()).
                                    arg(edtLogin->text()).
                                    arg(tipousuario),
                            QString("WHERE id_usuario = %1").arg(mode)))
        {
            if (!edtPass->text().isEmpty())
            {
                sqlUpdateRecord_N("usuario",
                                QString("clave = <&~%1~&>").arg(md5->result().data()),
                                QString("WHERE id_usuario = %1").arg(mode));
            }
            emit refreshGrid(true);
            close();
        }else
            QMessageBox::critical(this, tr("Editar usuario"),
                                     tr("Error, edición de usuario fallida."));
    }
}

