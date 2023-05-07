/*
  * QJSEditor es un editor de texto creado para el proyecto Alarife4.
  * Licencia GNU GPL
  * 2011
  * Herlin R. Matos Lastres <hmatos@grm.uci.cu>
  */

#include "../includes/htmlmanager.h"
#include <QWebElement>
#include <QWebElementCollection>
#include <QWebPage>
#include <QWebFrame>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QApplication>


HtmlManager::HtmlManager(QString p, QObject *parent) :
    QWebPage(parent)
{
    encPath = p;
}

QString HtmlManager::replaceAllImgSrc(QString newSrc)
{
    QWebElementCollection imgsList = this->mainFrame()->findAllElements("img");

    foreach(QWebElement img, imgsList)
    {
        img.setAttribute("src", newSrc);
    }

    return mainFrame()->toHtml();
}

QString HtmlManager::open(QString h)
{
    QString html = h.simplified();

    mainFrame()->setHtml(html);

    return this->mainFrame()->toHtml();
}

QString HtmlManager::prepareToSave(QString doc)
{
    QString html = doc.simplified();

    mainFrame()->setHtml(html);

    QString srcTmp, hrefTmp;
    QWebElementCollection imgsList = this->mainFrame()->findAllElements("img");

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
            img.setAttribute("src", "../medias/" + imgName);
        }
    }

    QWebElementCollection aList = this->mainFrame()->findAllElements("a");

    foreach(QWebElement alink, aList)
    {
        hrefTmp = alink.attribute("href");
        QStringList lstLink = hrefTmp.split("/");
        QString alinkName = lstLink.last();
        alink.setAttribute("href", "../medias/"+alinkName);
    }

    return this->mainFrame()->toHtml();
}
