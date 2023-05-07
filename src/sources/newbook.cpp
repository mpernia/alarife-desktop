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
#include <QSpacerItem>
#include <QDir>
#include <QCryptographicHash>

#include "../includes/alarife.h"
#include "../includes/newbook.h"
#include "../includes/templatedlg.h"
#include "../includes/sqlfunctions.h"

QString StrSumary;


NewBook::NewBook(QWidget *parent, DbConnect **db, Book **encyc) : QWizard(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    AlarifeDb2 = (*db);
    encyclopedia = (*encyc);
    isNewBook = -1;

    setMinimumSize(500, 400);
    setMaximumSize(500, 400);

    addPage(new IntroductionPage);
    addPage(new AdministratorPage);
    addPage(new XMediaPage);
    addPage(new ConclusionPage);

    setWindowTitle(tr("Crear nueva enciclopedia"));
}

void NewBook::createAllDir(QString &dbName)
{
   QDir path;
   path = QDir::homePath();
   if (!path.exists("Alarife"))
   {
     path.mkdir("Alarife");
     path = QDir::homePath()+"/Alarife";
   }
   else path = QDir::homePath()+"/Alarife";

   if (path.exists(dbName) == false)
   {
       path.mkdir(dbName);
   } else {
       int sub=1;
       QString tmpName = dbName;
       while (path.exists(tmpName))
       {
           tmpName = QString("%1(%2)").arg(dbName).arg(sub);
           sub++;
       }
       dbName = tmpName;
       path.mkdir(dbName);
   }

   path = QDir::homePath()+"/Alarife/"+dbName;
}

void NewBook::accept()
{
    QString StrTitle = field("edtTitle").toString();
    QString StrDbName = field("edtDbName").toString();
    QString StrImageFile = field("edtImage").toString();
    QString StrSoundFile = field("edtSound").toString();
    QString StrVideoFile = field("edtVideo").toString();
    QString StrOtherFile = field("edtOther").toString();
    QString StrName = field("edtName").toString();
    QString StrLogin = field("edtLogin").toString();

    QCryptographicHash *md5 = new QCryptographicHash(QCryptographicHash::Md5);
    QByteArray *byte = new QByteArray;
    md5->addData(byte->append(field("edtPass").toString()));


    createAllDir(StrDbName);

    AlarifeDb2->ConnectionString(QDir::homePath()+"/Alarife/"+StrDbName+".alf");
    AlarifeDb2->Active(true);
    AlarifeDb2->CreateDb();
    isNewBook = AlarifeDb2->Connected();

    sqlInsertRecord_N("enciclopedia",
                    "id_enciclopedia, nombre",
                    QString("1, <&~%1~&>").arg(StrTitle));
    sqlInsertRecord_N("enc_tipo_ext",
                    "enciclopedia, archivo, extension",
                    QString("1, <&~ExtImage~&>, <&~%1~&>").arg(StrImageFile));
    sqlInsertRecord_N("enc_tipo_ext",
                    "enciclopedia, archivo, extension",
                    QString("1, <&~ExtSound~&>, <&~%1~&>").arg(StrSoundFile));
    sqlInsertRecord_N("enc_tipo_ext",
                    "enciclopedia, archivo, extension",
                    QString("1, <&~ExtVideo~&>, <&~%1~&>").arg(StrVideoFile));
    sqlInsertRecord_N("enc_tipo_ext",
                    "enciclopedia, archivo, extension",
                    QString("1, <&~ExtXMedia~&>, <&~%1~&>").arg(StrOtherFile));
    sqlInsertRecord_N("usuario",
                    "id_usuario, nombre, usuario, clave, tipousuario",
                    QString("1, <&~%1~&>, <&~%2~&>, <&~%3~&>, 1").arg(StrName).arg(StrLogin).arg(md5->result().data()));

    encyclopedia->setUserIndex(1);

    emit closeDlg(isNewBook);

    QDialog::accept();

}

IntroductionPage::IntroductionPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Introducción"));
    lblInfo = new QLabel(tr("Este asistente le ayudara en la  "
                          "creación de una nueva enciclopedia "
                          "luego de finalizado ud. podrá comenzar inmediatamente"
                          "a interactuar con el software"));
    lblInfo->setWordWrap(true);
    lblTitle   = new QLabel(tr("Escriba el Título de la enciclopedia:"));
    lblDbName = new QLabel(tr("\nEscriba el nombre del directorio de la enciclopedia:"));
    edtTitle   = new QLineEdit;
    edtDbName = new QLineEdit;

    registerField("edtTitle",edtTitle);
    registerField("edtDbName",edtDbName);

    QVBoxLayout *lytIntro = new QVBoxLayout;
    lytIntro->addWidget(lblInfo);
    lytIntro->addWidget(lblTitle);
    lytIntro->addWidget(edtTitle);
    lytIntro->addWidget(lblDbName);
    lytIntro->addWidget(edtDbName);
    setLayout(lytIntro);
}

AdministratorPage::AdministratorPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Crear Editor Jefe"));
    setSubTitle(tr("Escriba el nombre usuario y contraseña del Editor Jefe de la enciclopedia."));

    edtName = new QLineEdit;
    edtLogin = new QLineEdit;
    edtPass = new QLineEdit;
    edtPass->setEchoMode(QLineEdit::Password);

    registerField("edtName",edtName);
    registerField("edtLogin",edtLogin);
    registerField("edtPass",edtPass);

    QGridLayout *lytUser = new QGridLayout;
    lytUser->setAlignment(Qt::AlignCenter);
    lblLogin = new QLabel(tr("Escriba el usuario"));
    lblPass  = new QLabel(tr("Escriba la contraseña"));
    lytUser->addWidget(lblLogin,0,1);
    lytUser->addWidget(lblPass,0,2);
    lytUser->addWidget(edtLogin,1,1);
    lytUser->addWidget(edtPass,1,2);

    QVBoxLayout *lytAdmin = new QVBoxLayout;
    lblText = new QLabel(tr("Escriba el nombre completo:"));
    lytAdmin->addWidget(lblText);
    lytAdmin->addWidget(edtName);
    lytAdmin->addLayout(lytUser);

    setLayout(lytAdmin);
}

XMediaPage::XMediaPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Asociar Archivos X-Media"));
    setSubTitle(tr("Seleccione las extensiones de los archivos X-Media que desea gestionar."));

    lblMedia = new QLabel(tr("Seleccione las extensiones de los archivos X-Media que desea gestionar."));
    lblImage = new QLabel(tr("Archivos de imágenes:"));
    lblSound = new QLabel(tr("Archivos de audios:"));
    lblVideo = new QLabel(tr("Archivos de videos:"));
    lblOther = new QLabel(tr("Otros archivos:"));

    edtImage = new QLineEdit;
    edtSound = new QLineEdit;
    edtVideo = new QLineEdit;
    edtOther = new QLineEdit;

    lblMedia->setWordWrap(true);

    edtImage->setText("*.bmp; *.jpg; *.jpeg; *.png; *.gif; *.svg; *.tiff");
    edtSound->setText("*.ogg; *.mp2; *.mp3; *.wav; *.wma");
    edtVideo->setText("*.avi; *.mpg; *.vob; *.flv; *.webm; *.mp4");
    edtOther->setText("*.pdf; *.txt; *.rtf; *.odt; *.doc; *.zip; *.rar");

    registerField("edtImage",edtImage);
    registerField("edtSound",edtSound);
    registerField("edtVideo",edtVideo);
    registerField("edtOther",edtOther);

    QVBoxLayout *lytXMedia = new QVBoxLayout;
    lytXMedia->addWidget(lblMedia);
    lytXMedia->addWidget(lblImage);
    lytXMedia->addWidget(edtImage);
    lytXMedia->addWidget(lblSound);
    lytXMedia->addWidget(edtSound);
    lytXMedia->addWidget(lblVideo);
    lytXMedia->addWidget(edtVideo);
    lytXMedia->addWidget(lblOther);
    lytXMedia->addWidget(edtOther);

    setLayout(lytXMedia);
}

ConclusionPage::ConclusionPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Conclusión"));
    label = new QLabel;
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);

    setLayout(layout);
}

void ConclusionPage::initializePage()
{

    QString finishText = wizard()->buttonText(QWizard::FinishButton);

    finishText.remove('&');

    QString text = tr("<b>RESUMEN</b><p>Ttulo de la enciclopedia: %1 <br>Nombre del "
                     "directorio: %2<br>Dirección del directorio: %3<p>Formato de archivos asociados"
                     "<br>Formato de imagenes: %4<br>Formato de audios: %5<br>Formato de videos: %6"
                     "<br>Otros formatos: %7<p>Si los datos son correctos, haga clic en %8"
                     " para generar la base de datos y los directorios de la nueva enciclopedia.");

    label->setText(text.
                   arg(field("edtTitle").toString()).
                   arg(field("edtDbName").toString()).
                   arg(__DIR_FILES__ + "/Alarife/...").
                   arg(field("edtImage").toString()).
                   arg(field("edtSound").toString()).
                   arg(field("edtVideo").toString()).
                   arg(field("edtOther").toString()).
                   arg(finishText));
   }

