#include <QSqlQuery>
#include <QWebFrame>
#include <QWebPage>

#include "../includes/mineditor.h"
#include "../includes/htmlmanager.h"
#include "../includes/mainwindow.h"
#include "../includes/insertmediadlg.h"
#include "../includes/sqlfunctions.h"
#include "../includes/strfunction.h"


MinEditor::MinEditor(QWidget *parent, Book *encyc) : QWebView(parent)
{
    firstTime = true;

    encyclopedia = encyc;

    settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
    settings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    settings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    settings()->setAttribute(QWebSettings::LocalStorageDatabaseEnabled, true);
    settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);

    QString path = QString("%1/qjseditor/qjseditorMin.html").arg(__APP_PATH__);
    if (path.contains("/bin")) path.remove("/bin");
    setUrl(QUrl::fromLocalFile(path));
    connect(this,SIGNAL(loadFinished(bool)),this,SLOT(isFirstTime()));
}

QWebView* MinEditor::createWindow( QWebPage::WebWindowType /*type*/ )
{
    QWebView *wv = new QWebView;

    return wv;
}

void MinEditor::saveData()
{
    QString document = this->page()->mainFrame()->evaluateJavaScript("CKEDITOR.instances.editor.getData()").toString();
    document = document.simplified();

    document.replace("'", "&#39;");

    QString html = document;

    QWebPage page;
    page.mainFrame()->setHtml(html);

    QString srcTmp, hrefTmp;
    QWebElementCollection imgsList = page.mainFrame()->findAllElements("img");

    foreach(QWebElement img, imgsList)
    {
        srcTmp = img.attribute("src");
        QStringList lst = srcTmp.split("/");
        QString imgName = lst.last();

        if(imgName == "audio.jpg"){
            img.setAttribute("src", "../imagenes/audio.jpg");
        }else if(imgName == "video.jpg"){
            img.setAttribute("src", "../imagenes/video.jpg");
        }else if(imgName == "xmedias.jpg"){
            img.setAttribute("src", "../imagenes/xmedias.jpg");
        }else{
            img.setAttribute("src", "../medias/"+imgName);
        }
    }

    QWebElementCollection aList = page.mainFrame()->findAllElements("a");

    foreach(QWebElement alink, aList)
    {
        hrefTmp = alink.attribute("href");
        QStringList lstLink = hrefTmp.split("/");
        QString alinkName = lstLink.last();
        alink.setAttribute("href", "medias/"+alinkName);
    }

    document = page.mainFrame()->toHtml();

    if(document.contains("<body")) document = selectUndefineArea(document,"body");

    sqlUpdateRecord_N("enciclopedia",
                    QString("resumen=<&~%1~&>").arg(document),
                    QString("WHERE id_enciclopedia=%1").arg(encyclopedia->getIndex()));
}

void MinEditor::insertData(QString html)
{
    HtmlManager htmlManager(encyclopedia->getMediaPath());

    QString line = htmlManager.open(html);

    line.replace("'", "&#39;");

    QString jsCode = "CKEDITOR.instances.editor.setData('"+line+"')";
    this->page()->mainFrame()->evaluateJavaScript(jsCode);
}

void MinEditor::insertMediaDlg()
{
    InsertMediaDlg *media = new InsertMediaDlg(this, (WebView*)(this), encyclopedia);
    media->exec();
}

void MinEditor::isFirstTime()
{
    if(firstTime){
        emit loadFirstTime();
        firstTime = false;
    }
}
