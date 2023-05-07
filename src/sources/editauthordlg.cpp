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
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtSql>

#include "../includes/database.h"
#include "../includes/alarife.h"
#include "../includes/gridimage.h"
#include "../includes/editauthordlg.h"
#include "../includes/sqlfunctions.h"

EditAuthorDlg::EditAuthorDlg(QWidget *parent, int index, int opt) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    setMinimumSize(450, 250);
    setMaximumSize(450, 250);

    option = opt;
    id = index;
    change_img = false;

    edtName = new QLineEdit;
    edtTitle = new QLineEdit;
    edtMail = new QLineEdit;
    edtJob = new QLineEdit;

    btnCancel = new QPushButton(tr("Cancelar"));
    btnOk = new QPushButton(tr("Aceptar"));

    btnCancel->setMaximumWidth(__BTN_SIZE__);
    btnOk->setMaximumWidth(__BTN_SIZE__);

    QVBoxLayout *lytUser = new QVBoxLayout;
    lblCaption = new QLabel(tr("Nombre:"));
    lytUser->addWidget(lblCaption);
    lytUser->addWidget(edtName);
    lblCaption = new QLabel(tr("Título:"));
    lytUser->addWidget(lblCaption);
    lytUser->addWidget(edtTitle);
    lblCaption = new QLabel(tr("Cargo:"));
    lytUser->addWidget(lblCaption);
    lytUser->addWidget(edtJob);
    lblCaption = new QLabel(tr("E-Mail:"));
    lytUser->addWidget(lblCaption);
    lytUser->addWidget(edtMail);

    photo = QPixmap(":/images/0.gif");

    btnLoad = new QToolButton;
    btnLoad->setIconSize(QSize(130,150));
    btnLoad->setMinimumSize(140, 160);
    btnLoad->setMaximumSize(140, 160);
    btnLoad->setIcon(QPixmap(photo));

    QVBoxLayout *lytPhoto = new QVBoxLayout;
    lblCaption = new QLabel(tr("Foto:"));
    lytPhoto->addWidget(lblCaption);
    lytPhoto->addWidget(btnLoad);

    QHBoxLayout *lytChild = new QHBoxLayout;
    lytChild->addLayout(lytPhoto);
    lytChild->addLayout(lytUser);

    QGridLayout *lytButton = new QGridLayout;
    lytButton->setAlignment(Qt::AlignRight);
    lytButton->addWidget(btnCancel,0,1);
    lytButton->addWidget(btnOk,0,2);

    QVBoxLayout *lytMain = new QVBoxLayout;
    lytMain->addLayout(lytChild);
    lytMain->addLayout(lytButton);

    if (option == 0)
        updateInterface(AddingMode);
    else
        updateInterface(EditingMode);

    setLayout(lytMain);

    connect(btnLoad, SIGNAL(clicked()), this, SLOT(LoadPhotoExec()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(btnOk, SIGNAL(clicked()), this, SLOT(PostValuesExec()));    
}

void EditAuthorDlg::updateInterface(Mode mode)
{
    currentMode = mode;
    switch (currentMode) {
        case AddingMode:
            edtMail->clear();
            edtJob->clear();
            edtName->clear();
            edtTitle->clear();
        break;
        case EditingMode:
            qry = new QSqlQuery;
            getValueRecord(&qry, id);
            edtName->setText(qry->value(0).toString());
            edtTitle->setText(qry->value(1).toString());
            edtJob->setText(qry->value(2).toString());
            edtMail->setText(qry->value(3).toString());
            fotos = qry->value(4).toInt();

            if (QFile(QString("%1/%2.jpg").arg(mediaPath).arg(id)).exists())
            {
                photo = QPixmap(QString("%1/%2").arg(mediaPath).arg(sqlGetImageName(fotos)));
                delete qry;
            }
            btnLoad->setIcon(QPixmap(photo));
        break;
    }
}

void EditAuthorDlg::setCaption(QString caption)
{
    setWindowTitle(caption);
}

void EditAuthorDlg::LoadPhotoExec()
{
    int btn = QMessageBox::question(this, tr("Cambiar imagen"),
                                    tr("¿Desea cambiar la imagen actual?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if (btn == QMessageBox::Yes)
    {
        QString ext = extension;
        ext.replace(";", "");
        fileName = QFileDialog::getOpenFileName(this,  tr("Seleccionar Imagen"),
                                                QString(), tr("Archivos de formato [%1] (%2)").
                                                arg(extension).
                                                arg(ext));

        if (!fileName.isEmpty())
        {
            change_img = true;
            photo = QPixmap(fileName);
            btnLoad->setIcon(QPixmap(photo));
        }
    }
}

void EditAuthorDlg::PostValuesExec()
{
    if (edtName->text() == "") {
        QMessageBox::information(this, tr("Existen campos requeridos vacios"),
                                 tr("Por favor, inserte el nombre de usario"));
        return;
    }

    int id_autor;
    int logo = 0;
    if (currentMode == AddingMode)
    {
        if (change_img == true)
        {
            logo = sqlAutoIncremental("id_adjunto", "adjuntos");

            sqlInsertRecord_N("adjuntos",
                            "id_adjunto, descripcion, extension, tipo",
                            QString("%1, <&~%2~&>, <&~%3~&>, <&~I~&>").
                                    arg(logo).
                                    arg(edtName->text()).
                                    arg(QFileInfo(fileName).suffix()));

            QFile::copy(fileName, QString("%1/%2.%3").arg(mediaPath).
                                                      arg(logo).
                                                      arg(QFileInfo(fileName).suffix()));
        }

        id_autor = sqlAutoIncremental("id_autor", "autor");

        if (sqlInsertRecord_N("autor",
                            "id_autor, nombre, titulo, cargo, correo,foto",
                            QString("%1, <&~%2~&>, <&~%3~&>, <&~%4~&>, <&~%5~&>, %6").
                                    arg(id_autor).
                                    arg(edtName->text()).
                                    arg(edtTitle->text()).
                                    arg(edtJob->text()).
                                    arg(edtMail->text()).
                                    arg(logo)))
        {
            sqlInsertRecord_N("enc_autor",
                            "enciclopedia, autor, colaborador, equipo",
                            QString("%1, %2, %3, %4").
                                    arg(idEnc).
                                    arg(id_autor).
                                    arg(colab).
                                    arg(team));
        }else{
            QMessageBox::information(this, tr("Inserción fallida"),
                                     tr("Error, en la inserción del autor. \"%1\"").
                                     arg(edtName->text()));
            return;
        }

    } else if (currentMode == EditingMode) {
        if (change_img == true){
            if (fotos == 0){
                fotos = sqlAutoIncremental("id_adjunto", "adjuntos");

                sqlInsertRecord_N("adjuntos",
                                "id_adjunto, descripcion, extension, tipo",
                                QString("%1, <&~%2~&>, <&~%3~&>, <&~I~&>").
                                arg(fotos).
                                arg(edtName->text()).
                                arg(QFileInfo(fileName).suffix()));
            }else{
                sqlUpdateRecord_N("adjuntos",
                                QString("descripcion = <&~%1~&>, extension = <&~%2~&>, tipo = <&~I~&>").
                                        arg(edtName->text()).
                                        arg(QFileInfo(fileName).suffix()),
                                QString("WHERE id_adjunto = %1").arg(fotos));

                QFile::remove(QString("%1/%2").
                              arg(mediaPath).
                              arg(sqlGetImageName(fotos)));
            }

            QFile::copy(fileName, QString("%1/%2.%3").
                                    arg(mediaPath).
                                    arg(fotos).
                                    arg(QFileInfo(fileName).suffix()));
        }

        id_autor = id;

        if (!sqlUpdateRecord_N("autor",
                             QString("nombre = <&~%1~&>, titulo = <&~%2~&>, "
                                     "cargo = <&~%3~&>, correo = <&~%4~&>, foto = %5").
                                     arg(edtName->text()).
                                     arg(edtTitle->text()).
                                     arg(edtJob->text()).
                                     arg(edtMail->text()).
                                     arg(fotos),
                             QString("WHERE id_autor = %1").arg(id_autor)))
        {
            QMessageBox::information(this, tr("Edición fallida"),
                                         tr("Error, \"%1\" en la actualización del autor.").
                                         arg(edtName->text()));
            return;
        }
    }


    if (currentMode == AddingMode)
        updateInterface(AddingMode);
    else
        updateInterface(EditingMode);

    RefreshGrid();
    close();

}

void EditAuthorDlg::CancelValuesExec()
{
    updateInterface(EditingMode);
}

int EditAuthorDlg::getRollValue(int id)
{
    int usr = -1;
    qry = new QSqlQuery;
    qry->prepare("SELECT enc_autor.colaborador, enc_autor.equipo "
                 "FROM enc_autor WHERE enc_autor.autor = :id_autor");
    qry->bindValue(":id_autor", id);
    qry->first();
    if (qry->value(0).toInt() == 0 && qry->value(1).toInt() == 1) usr = 0;
    else if (qry->value(0).toInt() == 1 && qry->value(1).toInt() == 0) usr = 1;
    delete qry;
    return usr;
}

void EditAuthorDlg::refreshvalue(QTableView **tbl, QString sql)
{
   grid = (*tbl);
   sqlStr = sql;
}

void EditAuthorDlg::RefreshGrid()
{
    QSqlQueryModel *master = new QSqlQueryModel;

    master->setQuery(sqlStr);
    grid->setModel(master);

    for (int i=0; i < master->rowCount(); i++){
        grid->setRowHeight(i,85);
    }

    grid->show();
    grid->selectRow(0);
}

void EditAuthorDlg::setTypeUsr(int param1, int param2)
{
    colab = param1;
    team = param2;
}

void EditAuthorDlg::getValueRecord(QSqlQuery **query, int id)
{
    (*query)->prepare("SELECT nombre, titulo, cargo, correo, foto "
                      "FROM autor WHERE id_autor = :id_autor");
    (*query)->bindValue(":id_autor", id);
    (*query)->exec();
    (*query)->first();
}
