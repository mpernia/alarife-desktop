/*
  * QJSEditor es un editor de texto creado para el proyecto Alarife4.
  * Licencia GNU GPL
  * 2011
  * Herlin R. Matos Lastres <hmatos@grm.uci.cu>
  */
#ifndef INSIMGDLG_H
#define INSIMGDLG_H

#include <QDialog>
#include "webview.h"


namespace Ui {
    class InsImgDlg;
}

class InsImgDlg : public QDialog
{
    Q_OBJECT

public:
    explicit InsImgDlg(WebView *, QString p,QWidget *parent = 0);
    ~InsImgDlg();

private:
    Ui::InsImgDlg *ui;
    WebView *webView;
    QString encPath;
private:
    bool copyImg(QString src, QString id);

private slots:
    void openImage();
    void insertImg();
};

#endif // INSIMGDLG_H
