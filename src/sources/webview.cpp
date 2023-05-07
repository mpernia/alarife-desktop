/*
  * QJSEditor es un editor de texto creado para el proyecto Alarife4.
  * Licencia GNU GPL
  * 2011
  * Herlin R. Matos Lastres <hmatos@grm.uci.cu>
  * Last modified: 2018/03/04
  * By: Maikel Enrique Pernía Matos <perniamatos@infomed.sld.cu>
  */
#include "../includes/webview.h"
#include "../includes/alarife.h"

#include <QWebPage>
#include <QMessageBox>
#include <QPrintPreviewDialog>
#include <QVBoxLayout>
#include <QPrintDialog>
#include <QPrinter>
#include <QDebug>
#include <QDir>
#include <QApplication>



WebView::WebView(ItemId i,QWidget * parent): QWebView (parent)
{
    firstTime = true;
    setItemId(i);

    settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
    settings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    settings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    settings()->setAttribute(QWebSettings::LocalStorageDatabaseEnabled, true);
    settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);

    settings()->setMaximumPagesInCache(20);
    settings()->setObjectCacheCapacities(50,200,300);

    QString path = QString("%1/qjseditor/qjseditor.html").arg(__APP_PATH__); 
    if (path.contains("/bin")) path.remove("/bin"); 
    setUrl(QUrl::fromLocalFile(path)); 

    modified = false;

    connect(this->page(),SIGNAL(printRequested(QWebFrame*)),this,SLOT(printFrame(QWebFrame*)));
    connect(this,SIGNAL(loadFinished(bool)),this,SLOT(isFirstTime()));
}

void WebView::setModified(bool m)
{
    modified = m;
}

bool WebView::isMod()
{
    return modified;
}

void WebView::setFileName(QString f)
{
    fn = f;
}

bool WebView::hasFileName()
{
    if(fileName().isNull())
        return false;
    return true;
}

QString WebView::fileName()
{
    return fn;
}

QWebView* WebView::createWindow( QWebPage::WebWindowType /*type */)
{
    QWebView *wv = new QWebView;

    return wv;
}

void WebView::printFrame(QWebFrame *frame)
{
    QPrinter *printer = new QPrinter(QPrinter::HighResolution);
    QPrintDialog printDialog(printer);
    if (printDialog.exec() == QDialog::Accepted)
    {
        frame->print(printer);
    }
}

void WebView::setItemId(ItemId i)
{
    itemID = i;
}

ItemId* WebView::getItemId()
{
    return &itemID;
}

void WebView::isFirstTime()
{
    if(firstTime){
        emit loadFirstTime();
        firstTime = false;
    }
}
