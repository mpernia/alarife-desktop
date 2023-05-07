/*
  * QJSEditor es un editor de texto creado para el proyecto Alarife4.
  * Licencia GNU GPL
  * 2011
  * Herlin R. Matos Lastres <hmatos@grm.uci.cu>
  */

#include "includes/insimgdlg.h"
#include "ui_insimgdlg.h"
#include <QFileDialog>
#include <QWebFrame>
#include <QMessageBox>
#include <QDebug>
#include <QFileInfo>

#include "includes/database.h"
#include "includes/sqlfunctions.h"

InsImgDlg::InsImgDlg(WebView *wv,QString p,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsImgDlg)
{
    ui->setupUi(this);
    webView = wv;
    encPath = p;

    connect(ui->openImgButton,SIGNAL(clicked()),this,SLOT(openImage()));
    connect(ui->insert,SIGNAL(clicked()),this,SLOT(insertImg()));
}

InsImgDlg::~InsImgDlg()
{
    delete ui;
}

void InsImgDlg::openImage()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                                  QString(), tr("JPG, PNG(*.jpg *.png);;All Files (*)"));

    if (!fn.isEmpty() && QFile::exists(fn))
        ui->imgPath->setText(fn);
}

void InsImgDlg::insertImg()
{

    QString fn = ui->imgPath->text();

    if(fn.isEmpty() && !QFile::exists(fn))
        return;

    QString mediasPath = encPath + "/";

    int id = sqlAutoIncremental("id_adjunto","adjuntos");
    QString imgId = QString::number(id);

    QString newImgPath = mediasPath + imgId;
    QString imgHtml = "<img alt=\""+ui->altText->text()+"\" src=\""+ newImgPath +"\" style=\"width: 307px; height: 238px; \"/>";
    QString jsCode = "CKEDITOR.instances.editor.insertHtml('"+imgHtml+"')";

    QFileInfo fi(ui->imgPath->text());
    QString ext = fi.suffix();

    if(copyImg(ui->imgPath->text(), imgId)){
        QSqlQuery query;
        query.prepare("INSERT INTO adjuntos(id_adjunto, descripcion, extension,tipo) "
                      "VALUES (:id, :descripcion, :extension, 'I')");
        query.bindValue(":id",id);
        query.bindValue(":descripcion",ui->altText->text());
        query.bindValue(":extension",ext);
        if(query.exec()){
            webView->page()->mainFrame()->evaluateJavaScript(jsCode);
        }else{
            QFile::remove(newImgPath);
            qDebug()<<"DB Error:"<<query.lastError().text();
        }

        query.finish();
    }else
        QMessageBox::critical(this,tr("error"),tr("No se pudo insertar la imagen"));


    close();
}

bool InsImgDlg::copyImg(QString src, QString id)
{
    return QFile::copy(src, encPath + "/" + id);
}
