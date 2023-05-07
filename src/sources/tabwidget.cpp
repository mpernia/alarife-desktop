/*
  * QJSEditor es un editor de texto creado para el proyecto Alarife4.
  * Licencia GNU GPL
  * 2011
  * Herlin R. Matos Lastres <hmatos@grm.uci.cu>
  */
#include "includes/tabwidget.h"
#include "includes/webview.h"
#include "includes/htmlmanager.h"
#include <QMessageBox>
#include <QWebFrame>
#include <QFileInfo>
#include <QFileDialog>


TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent)
{
    setUsesScrollButtons(true);
    setMovable(true);
}

void TabWidget::setChanged()
{

    this->setTabIcon(this->currentIndex(),QIcon(":/images/edit-state1.png"));

    WebView *currentWebview = static_cast<WebView*>(this->currentWidget());
    currentWebview->setModified(true);
}


