/*
  * QJSEditor es un editor de texto creado para el proyecto Alarife4.
  * Licencia GNU GPL
  * 2011
  * Herlin R. Matos Lastres <hmatos@grm.uci.cu>
  */
#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>


class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    TabWidget( QWidget * parent = 0 );

private slots:
    void setChanged();
private:

};

#endif // TABWIDGET_H
