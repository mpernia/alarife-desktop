/*
  * QJSEditor es un editor de texto creado para el proyecto Alarife4.
  * Licencia GNU GPL
  * 2011
  * Herlin R. Matos Lastres <hmatos@grm.uci.cu>
  */
#ifndef HTMLMANAGER_H
#define HTMLMANAGER_H

#include <QWebPage>

class HtmlManager : public QWebPage
{
    Q_OBJECT
public:
    explicit HtmlManager(QString p, QObject *parent = 0);
    QString replaceAllImgSrc(QString );
    QString open(QString );
    QString prepareToSave(QString );

private:
    QString encPath;
signals:

public slots:

};

#endif // HTMLMANAGER_H
