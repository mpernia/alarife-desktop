/*
  * QJSEditor es un editor de texto creado para el proyecto Alarife4.
  * Licencia GNU GPL
  * 2011
  * Herlin R. Matos Lastres <hmatos@grm.uci.cu>
  */
#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include "webview.h"
#include "itemid.h"


class WebView : public QWebView
{
    Q_OBJECT
public:
    WebView(ItemId, QWidget * parent = 0);
    void setModified(bool);
    bool isMod();
    void setFileName(QString );
    QString fileName();
    bool hasFileName();
    void setItemId(ItemId );
    ItemId* getItemId();
private:
    QWebView* createWindow( QWebPage::WebWindowType type);
private:
    bool modified;
    QString fn;
    ItemId itemID;
    bool firstTime;
private slots:
    void printFrame(QWebFrame *);
    void isFirstTime();
signals:
    void loadFirstTime();
};

#endif // WEBVIEW_H
