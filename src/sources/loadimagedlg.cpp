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

#include "../includes/loadimagedlg.h"
#include "../includes/alarife.h"
#include "../includes/strfunction.h"
#include "../includes/sqlfunctions.h"


LoadImageDlg::LoadImageDlg(QWidget *parent, int mode) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    setMinimumSize(420,300);
    setMaximumSize(420,300);

    md = mode;
    btnClose = new QPushButton(tr("Cancelar"));
    btnLoad = new QPushButton(tr("Seleccionar"));
    btnOk = new QPushButton(tr("Aceptar"));
    edtPath = new QLineEdit;
    edtDescription = new QLineEdit;
    lblPreview = new QLabel;
    lblDescribe = new QLabel;

    QSpacerItem *vSpacer0 = new QSpacerItem(5, 40, QSizePolicy::Minimum,
                                           QSizePolicy::Expanding);
    QSpacerItem *vSpacer1 = new QSpacerItem(5, 40, QSizePolicy::Minimum,
                                           QSizePolicy::Expanding);

    QGridLayout *lytButtonPanel = new QGridLayout;
    lytButtonPanel->setAlignment(Qt::AlignRight);
    lytButtonPanel->addWidget(btnClose, 0, 1);
    lytButtonPanel->addWidget(btnOk, 0, 2);

    QHBoxLayout *lytPath = new QHBoxLayout;
    lytPath->addWidget(edtPath);
    lytPath->addWidget(btnLoad);

    QVBoxLayout *lytLeft = new QVBoxLayout;
    lytLeft->addWidget(lblDescribe);
    lytLeft->addItem(vSpacer0);

    QVBoxLayout *lytRigth = new QVBoxLayout;
    lytRigth->addWidget(lblPreview);
    lytRigth->addItem(vSpacer1);

    QGridLayout *lytCenter = new QGridLayout;

    lblText = new QLabel(tr("Información del archivo:"));
    lytCenter->addWidget(lblText, 0, 0);
    lblText = new QLabel(tr("Vista Previa:"));
    lytCenter->addWidget(lblText, 0, 1);
    lytCenter->addLayout(lytLeft,1,0);
    lytCenter->addLayout(lytRigth,1,1);

    QVBoxLayout *lytMain = new QVBoxLayout;
    lblText = new QLabel(tr("Direccion del archivo a insertar:"));
    lytMain->addWidget(lblText);
    lytMain->addLayout(lytPath);
    lytMain->addLayout(lytCenter);
    lblText = new QLabel(tr("Descripcion del archivo:"));
    lytMain->addWidget(lblText);
    lytMain->addWidget(edtDescription);
    lytMain->addLayout(lytButtonPanel);

    connect(btnClose, SIGNAL(clicked()), this, SLOT(CancelExec()));
    connect(btnLoad, SIGNAL(clicked()), this, SLOT(InsertExec()));
    connect(btnOk, SIGNAL(clicked()), this, SLOT(AceptExec()));

    setLayout(lytMain);
    if (md == 0)
        setWindowTitle(tr("Insertar X-Media"));
    else
        setWindowTitle(tr("Editar X-Media"));
}

void LoadImageDlg::setFileExtension(QString ext)
{
    extension = ext;
}

void LoadImageDlg::setFileType(int m)
{
    type = m;
}

void LoadImageDlg::AceptExec()
{
    if (!fileName.isEmpty()){
        if (md == 0){
            sqlInsertRecord_N("adjuntos",
                            "id_adjunto, descripcion, extension, tipo",
                            QString("%1, <&~%2~&>, <&~%3~&>, <&~%4~&>").
                                    arg(id).
                                    arg(edtDescription->text()).
                                    arg(QFileInfo(fileName).suffix()).
                                    arg(ini));
        }else{
            sqlUpdateRecord_N("adjuntos",
                            QString("descripcion = <&~%1~&>, extension = <&~%2~&>, tipo = <&~%3~&>").
                                    arg(edtDescription->text()).
                                    arg(QFileInfo(fileName).suffix()).
                                    arg(ini),
                            QString("WHERE id_adjunto = %1 ").arg(id));
        }

        if (fileName != oldFileName){
            QFile::remove(oldFileName);
            QFile::copy(fileName, QString("%1/%2.%3").arg(mediaPath).
                        arg(id).
                        arg(QFileInfo(fileName).suffix()));
        }
        emit refreshQuery();
        close();
    }

}

void LoadImageDlg::CancelExec()
{
    if (md == 0)
    {
        edtPath->clear();
        edtDescription->clear();
    }else close();
}

void LoadImageDlg::InsertExec()
{
    QString ext = extension;
    ext.replace(";", "");

    oldFileName = fileName;
    fileName = QFileDialog::getOpenFileName(this,  tr("Seleccionar Imagen"),
                                            QString(), tr("Archivos de formato [%1] (%2)").
                                            arg(extension).arg(ext));

    if (!fileName.isEmpty())
    {
        edtPath->setText(fileName);
        if (md == 0) id = sqlAutoIncremental("id_adjunto", "adjuntos");
        showValues();
    }
}

void LoadImageDlg::loadValues()
{
    QSqlQuery *qry = new QSqlQuery;
    qry->exec(QString("SELECT id_adjunto||'.'||extension AS fileMedia, descripcion, "
                      "id_adjunto FROM adjuntos WHERE id_adjunto = %1").arg(md));
    qry->first();

    fileName = QString("%1/%2").arg(mediaPath).arg(qry->value(0).toString());
    edtDescription->setText(qry->value(1).toString());
    id = qry->value(2).toInt();
    showValues();
}

void LoadImageDlg::showValues()
{
    switch (type)
    {
        case 1: tmpName = fileName;
                ini = "I";
            break;
        case 2: tmpName =":/images/no_sound.png";
                ini = "A";
            break;
        case 3: tmpName =":/images/no_video.png";
                ini = "V";
            break;
        case 4: tmpName =":/images/no_xmedia.png";
                ini = "X";
            break;
    }

    QPixmap *thumbnails = new QPixmap(tmpName);
    lblPreview->setPixmap(thumbnails->scaled(QSize(130,130),
                                       Qt::IgnoreAspectRatio,
                                       Qt::FastTransformation));
    lblPreview->setAlignment(Qt::AlignCenter);
    delete thumbnails;

    lblDescribe->setText(QString(tr("Nombre: %1<br>Extensión: %2<br>Tamaño: %3")).
                         arg(id).
                         arg(QFileInfo(fileName).suffix()).
                         arg(getFileSize(QFileInfo(fileName).size())));
}

