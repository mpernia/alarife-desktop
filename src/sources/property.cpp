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

#include "../includes/alarife.h"
#include "../includes/property.h"
#include "../includes/sqlfunctions.h"

bool closeE, closeL, closeT, closeF;


void PropertyDlg::closeEvent(QCloseEvent *event)
{
    if (closeE == true || closeL == true || closeT == true || closeF == true)
    {
        QMessageBox::warning(this, tr("Acciones pendientes"),
                             tr("Existe acciones sin finalizar. Por favor \n"
                                "guarde o cancele los cambios."));
        event->ignore();
    }else{
        event->accept();
        refreshValue();
    }    
}

PropertyDlg::PropertyDlg(QWidget *parent, Book **encyc) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    encyclopedia = (*encyc);

    setWindowTitle(tr("Configuración de la enciclopedia"));
    setMaximumSize(400,450);
    setMinimumSize(400,450);

    pageTemplate = new TemplateTabPage(this, encyclopedia->getIndex());
    pageFiles = new FilesTabPage(this, encyclopedia->getIndex());
    pageExtras = new ExtraTabPage(this, encyclopedia->getIndex());

    tabPage = new QTabWidget;
    tabPage->addTab(pageTemplate, tr("Plantilla"));
    tabPage->addTab(pageFiles, tr("Tipos de archivos"));
    tabPage->addTab(pageExtras, tr("Audio - Video"));

    QVBoxLayout *lytCredits = new QVBoxLayout;
    lytCredits->addWidget(tabPage);

    connect(pageTemplate, SIGNAL(modifiedTemplate()), this, SLOT(refreshValue()));
    connect(pageFiles, SIGNAL(modifiedExtension()), this, SLOT(refreshValue()));

    setLayout(lytCredits);
}

PropertyDlg::~PropertyDlg()
{
}


void PropertyDlg::refreshValue()
{
    QSqlQuery *qry_0 = new QSqlQuery;
    qry_0->exec("SELECT plantilla FROM enciclopedia");
    qry_0->first();

    encyclopedia->setTemplate(qry_0->value(0).toString());

    qry_0->exec(QString("SELECT extension FROM enc_tipo_ext WHERE enciclopedia = %1").
                  arg(encyclopedia->getIndex()));
    qry_0->first();
    encyclopedia->setExtImagen(qry_0->value(0).toString());
    qry_0->next();
    encyclopedia->setExtSonido(qry_0->value(0).toString());
    qry_0->next();
    encyclopedia->setExtVideo(qry_0->value(0).toString());
    qry_0->next();
    encyclopedia->setExtXMedia(qry_0->value(0).toString());

    qry_0->exec(QString("SELECT plantilla FROM enciclopedia WHERE "
                        "id_enciclopedia = %1").
                        arg(encyclopedia->getIndex()));
    qry_0->first();
    encyclopedia->setTemplate(qry_0->value(0).toString());
    delete qry_0;
}

TemplateTabPage::TemplateTabPage(QWidget *parent, int id) : QWidget(parent)
{
    QSpacerItem *vSpacer0 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *vSpacer1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *vSpacer2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    idEncyc = id;

    qryTemplate = new QSqlQuery;
    qryTemplate->exec(QString("SELECT plantilla FROM enciclopedia WHERE "
                              "id_enciclopedia = %1").arg(idEncyc));
    qryTemplate->first();
    dirName = qryTemplate->value(0).toString();
    delete qryTemplate;

    btnTEdit = new QPushButton(tr("Editar"));
    btnTPost = new QPushButton(tr("Aceptar"));
    btnTCancel = new QPushButton(tr("Cancelar"));

    QGridLayout *lytButtonPanel = new QGridLayout;
    lytButtonPanel->setAlignment(Qt::AlignCenter);
    lytButtonPanel->addWidget(btnTCancel,0,1);
    lytButtonPanel->addWidget(btnTEdit,0,2);
    lytButtonPanel->addWidget(btnTPost,0,3);

    lblImage = new QLabel;
    lblDescribe = new QLabel;

    QVBoxLayout *lytAbout = new QVBoxLayout;
    lytAbout->addItem(vSpacer0);
    lytAbout->addWidget(lblImage);
    lytAbout->addItem(vSpacer1);
    lytAbout->addWidget(lblDescribe);
    lytAbout->addItem(vSpacer2);
    lytAbout->addLayout(lytButtonPanel);

    connect(btnTEdit, SIGNAL(clicked()), this, SLOT(TemplateEditExec()));
    connect(btnTPost, SIGNAL(clicked()), this, SLOT(TemplatePostExec()));
    connect(btnTCancel, SIGNAL(clicked()), this, SLOT(TemplateCancelExec()));

    getTemplateValue();
    setTemplateWriteMode(true);

    this->setLayout(lytAbout);

}

void TemplateTabPage::getTemplateValue()
{
    QString path = __APP_PATH__;
    if (path.contains("/bin")) path.remove("/bin");

    QPixmap thumbnails;

    if (dirName.isEmpty())
    {
        thumbnails = QPixmap(":/images/no_image1.png");
    }else{

        thumbnails = QPixmap(path + __DIR_TEMPLATE__ + dirName + "/thumbnails.jpg");
    }

    lblImage->setPixmap(thumbnails.scaled(QSize(220,200),
                                        Qt::IgnoreAspectRatio,
                                        Qt::FastTransformation));
    lblImage->setAlignment(Qt::AlignCenter);

    QSettings settings(path + __DIR_TEMPLATE__ + dirName + "/information",
                       QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

    settings.beginGroup("template");
    QString swap = settings.value("author").toString();
    swap.replace(" | ", ", ", Qt::CaseSensitive);

    QString version = settings.value("version").toString();

    if (!version.isEmpty()) version.prepend(tr("Versión: "));

    lblDescribe->setText(QString(tr("Nombre: %1    %2<br>Autor(es): %3<br>Copyright: %4<br>%5")).
                         arg(dirName).
                         arg(version).
                         arg(swap).
                         arg(settings.value("copyright").toString()).
                         arg(settings.value("comment").toString()));
    lblDescribe->setWordWrap(true);
    settings.endGroup();
}

void TemplateTabPage::setTemplateWriteMode(bool mode)
{
    btnTEdit->setEnabled(mode);
    if (mode == false)
    {
        btnTCancel->setEnabled(true);
        btnTPost->setEnabled(true);
        closeT = true;
    }
    else {
        btnTCancel->setEnabled(false);
        btnTPost->setEnabled(false);
        closeT = false;
    }
}

void TemplateTabPage::setTemplateClearCache()
{
    
}

void TemplateTabPage::TemplateEditExec()
{
    oldDirName = dirName;
    TemplateDlg *temp = new TemplateDlg(this);
    temp->show();

    connect(temp,SIGNAL(getSelecction(QString)),this, SLOT(setSelection(QString)));

    getTemplateValue();
    setTemplateWriteMode(false);
}

void TemplateTabPage::TemplateCancelExec()
{
    dirName = oldDirName;
    getTemplateValue();
    setTemplateWriteMode(true);
}

void TemplateTabPage::TemplatePostExec()
{
    if (sqlUpdateRecord_N("enciclopedia",
                        QString("plantilla = <&~%1~&>").arg(dirName),
                        QString("WHERE id_enciclopedia = %1").arg(idEncyc)))
    {
        setTemplateWriteMode(true);
        setTemplateClearCache();

        emit modifiedTemplate();

    }else{
        QMessageBox::critical(this, tr("Actualización Fallida"),
                              tr("Ha ocurrido un error, los datos no seran actualizados."));
        TemplateCancelExec();
    }
}

void TemplateTabPage::setSelection(QString text)
{
    dirName = text;
    getTemplateValue();
}

FilesTabPage::FilesTabPage(QWidget *parent, int id) : QWidget(parent)
{
     QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum,
                                                   QSizePolicy::Expanding);

     idEncyc = id;

    lblImage = new QLabel;
    lblSound = new QLabel;
    lblVideo = new QLabel;
    lblOther = new QLabel;

    edtImage = new QLineEdit;
    edtSound = new QLineEdit;
    edtVideo = new QLineEdit;
    edtOther = new QLineEdit;

    btnFEdit = new QPushButton(tr("Editar"));
    btnFPost = new QPushButton(tr("Aceptar"));
    btnFCancel = new QPushButton(tr("Cancelar"));

    QGridLayout *lytButtonPanel = new QGridLayout;
    lytButtonPanel->setAlignment(Qt::AlignCenter);
    lytButtonPanel->addWidget(btnFCancel,0,1);
    lytButtonPanel->addWidget(btnFEdit,0,2);
    lytButtonPanel->addWidget(btnFPost,0,3);

    QGridLayout *lytFile = new QGridLayout;
    lblText = new QLabel(tr("Archivos de imágenes:"));
    lytFile->addWidget(lblText,0,1);
    lytFile->addWidget(edtImage,1,1);
    lytFile->addWidget(lblImage,1,2);
    lblText = new QLabel(tr("Archivos de sonido:"));
    lytFile->addWidget(lblText,2,1);
    lytFile->addWidget(edtSound,3,1);
    lytFile->addWidget(lblSound,3,2);
    lblText = new QLabel(tr("Archivos de videos:"));
    lytFile->addWidget(lblText,4,1);
    lytFile->addWidget(edtVideo,5,1);
    lytFile->addWidget(lblVideo,5,2);
    lblText = new QLabel(tr("Otros archivos:"));
    lytFile->addWidget(lblText,6,1);
    lytFile->addWidget(edtOther,7,1);
    lytFile->addWidget(lblOther,7,2);

    QVBoxLayout *lytMain = new QVBoxLayout;
    lytMain->addLayout(lytFile);
    lytMain->addItem(verticalSpacer);
    lytMain->addLayout(lytButtonPanel);

    getFilesValue();
    setFilesWriteMode(true);

    connect(btnFEdit, SIGNAL(clicked()), this, SLOT(FilesEditExec()));
    connect(btnFPost, SIGNAL(clicked()), this, SLOT(FilesPostExec()));
    connect(btnFCancel, SIGNAL(clicked()), this, SLOT(FilesCancelExec()));

    setLayout(lytMain);

}

void FilesTabPage::getFilesValue()
{
    qryFileType = new QSqlQuery;

    qryFileType->exec(QString("SELECT extension FROM enc_tipo_ext WHERE enciclopedia = '%1'").arg(idEncyc));

    qryFileType->first();
    edtImage->setText(qryFileType->value(0).toString());
    qryFileType->next();
    edtSound->setText(qryFileType->value(0).toString());
    qryFileType->next();
    edtVideo->setText(qryFileType->value(0).toString());
    qryFileType->next();
    edtOther->setText(qryFileType->value(0).toString());

    delete qryFileType;
}

void FilesTabPage::setFilesWriteMode(bool mode)
{
    edtImage->setReadOnly(mode);
    edtSound->setReadOnly(mode);
    edtVideo->setReadOnly(mode);
    edtOther->setReadOnly(mode);

    btnFEdit->setEnabled(mode);
    if (mode == false)
    {
        btnFCancel->setEnabled(true);
        btnFPost->setEnabled(true);
        closeF = true;
    }
    else {
        btnFCancel->setEnabled(false);
        btnFPost->setEnabled(false);
        closeF = false;
    }
}

void FilesTabPage::setFilesClearCache()
{
    oldImage.clear();
    oldSound.clear();
    oldVideo.clear();
    oldOther.clear();
}

void FilesTabPage::FilesEditExec()
{
    setFilesWriteMode(false);

    oldImage = edtImage->text();
    oldSound = edtSound->text();
    oldVideo = edtVideo->text();
    oldOther = edtOther->text();

    lblImage->clear();
    lblSound->clear();
    lblVideo->clear();
    lblOther->clear();
}

void FilesTabPage::FilesCancelExec()
{
    edtImage->setText(oldImage);
    edtSound->setText(oldSound);
    edtVideo->setText(oldVideo);
    edtOther->setText(oldOther);

    setFilesWriteMode(true);
    setFilesClearCache();
}

void FilesTabPage::FilesPostExec()
{
    if (sqlUpdateRecord_N("enc_tipo_ext",
                        QString("extension = <&~%1~&>").arg(edtImage->text()),
                        QString("WHERE ((archivo = <&~ExtImage~&>) AND enciclopedia = %1)").
                                arg(idEncyc)))
    {
        lblImage->setPixmap(QPixmap(":/images/result-ok.png"));
        oldImage.clear();
    } else lblImage->setPixmap(QPixmap(":/images/result-error.png"));

    if (sqlUpdateRecord_N("enc_tipo_ext",
                        QString("extension = <&~%1~&>").arg(edtSound->text()),
                        QString("WHERE ((archivo = <&~ExtSound~&>) AND enciclopedia = %1)").
                                arg(idEncyc)))
    {
        lblSound->setPixmap(QPixmap(":/images/result-ok.png"));
        oldSound.clear();
    } else lblSound->setPixmap(QPixmap(":/images/result-error.png"));

    if (sqlUpdateRecord_N("enc_tipo_ext",
                        QString("extension = <&~%1~&>").arg(edtVideo->text()),
                        QString("WHERE ((archivo = <&~ExtVideo~&>) AND enciclopedia = %1)").
                                arg(idEncyc)))
    {
        lblVideo->setPixmap(QPixmap(":/images/result-ok.png"));
        oldVideo.clear();
    } else lblVideo->setPixmap(QPixmap(":/images/result-error.png"));

    if (sqlUpdateRecord_N("enc_tipo_ext",
                        QString("extension = <&~%1~&>").arg(edtOther->text()),
                        QString("WHERE ((archivo = <&~ExtXMedia~&>) AND enciclopedia = %1)").
                        arg(idEncyc)))
    {
        lblOther->setPixmap(QPixmap(":/images/result-ok.png"));
        oldOther.clear();
    } else lblOther->setPixmap(QPixmap(":/images/result-error.png"));

    setFilesWriteMode(true);

    emit modifiedExtension();
}

ExtraTabPage::ExtraTabPage(QWidget *parent, int id) : QWidget(parent)
{
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum,
                                                       QSizePolicy::Expanding);

    idEncyc = id;

    chkVideo = new QCheckBox(tr("Activar videos embebidos"));
    chkSound = new QCheckBox(tr("Activar sonidos embebidos"));

    lblInfo = new QLabel(tr("<div align='justify'>Alarife permite incrustar audios y vídeos "
                            "en las páginas de los artículos con la limitante "
                            "que no todos los formatos conocidos para estos "
                            "tipos de archivos estan soportados por lo que "
                            "si activa el uso de cualquiera de estos tipos de "
                            "archivos debe verificar que los audios y vídeos registrados "
                            "en las «Medias».</div>"));
    lblVideo = new QLabel(tr("Formatos de archivos de videos soportados:<br> ogg, mp4, webm"));
    lblSound = new QLabel(tr("Formatos de archivos de audios soportados:<br> ogg, mp3"));

    lblInfo->setWordWrap(true);
    lblVideo->setWordWrap(true);
    lblSound->setWordWrap(true);

    btnEEdit = new QPushButton(tr("Editar"));
    btnEPost = new QPushButton(tr("Aceptar"));
    btnECancel = new QPushButton(tr("Cancelar"));

    QGridLayout *lytButtonPanel = new QGridLayout;
    lytButtonPanel->setAlignment(Qt::AlignCenter);
    lytButtonPanel->addWidget(btnECancel,0,1);
    lytButtonPanel->addWidget(btnEEdit,0,2);
    lytButtonPanel->addWidget(btnEPost,0,3);

    QVBoxLayout *lytExtra = new QVBoxLayout;
    lytExtra->addWidget(lblSound);
    lytExtra->addWidget(chkSound);
    lytExtra->addWidget(lblVideo);
    lytExtra->addWidget(chkVideo);

    QVBoxLayout *lytMain = new QVBoxLayout;
    lytMain->addWidget(lblInfo);
    lytMain->addLayout(lytExtra);
    lytMain->addItem(verticalSpacer);
    lytMain->addLayout(lytButtonPanel);

    getExtrasValue();
    setExtrasWriteMode(false);

    connect(btnEEdit, SIGNAL(clicked()), this, SLOT(ExtrasEditExec()));
    connect(btnEPost, SIGNAL(clicked()), this, SLOT(ExtrasPostExec()));
    connect(btnECancel, SIGNAL(clicked()), this, SLOT(ExtrasCancelExec()));

    setLayout(lytMain);
}

void ExtraTabPage::ExtrasEditExec()
{
    oldSound = chkSound->isChecked();
    oldVideo = chkVideo->isChecked();
    setExtrasWriteMode(true);
}

void ExtraTabPage::ExtrasCancelExec()
{
    chkSound->setChecked(oldSound);
    chkVideo->setChecked(oldVideo);
    setExtrasWriteMode(false);
}

void ExtraTabPage::ExtrasPostExec()
{
    sqlUpdateRecord_N("enciclopedia",
                    QString("audio_embebido = %1").arg(chkSound->isChecked()),
                    QString("WHERE id_enciclopedia = %1").arg(idEncyc));
    sqlUpdateRecord_N("enciclopedia",
                    QString("video_embebido = %1").arg(chkVideo->isChecked()),
                    QString("WHERE id_enciclopedia = %1").arg(idEncyc));
    setExtrasWriteMode(false);
}

void ExtraTabPage::getExtrasValue()
{
    QSqlQuery *qryExtra = new QSqlQuery;

    qryExtra->exec(QString("SELECT audio_embebido, video_embebido "
                           "FROM enciclopedia WHERE id_enciclopedia = '%1'").arg(idEncyc));

    qryExtra->first();
    chkSound->setChecked(qryExtra->value(0).toBool());
    chkVideo->setChecked(qryExtra->value(1).toBool());
    delete qryExtra;
}


void ExtraTabPage::setExtrasWriteMode(bool mode)
{
    chkSound->setEnabled(mode);
    chkVideo->setEnabled(mode);

    btnECancel->setEnabled(mode);
    btnEPost->setEnabled(mode);
    closeE = mode;

    mode == false ? btnEEdit->setEnabled(true) : btnEEdit->setEnabled(false);
}
