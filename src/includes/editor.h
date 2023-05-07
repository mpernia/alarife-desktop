/*
  * QJSEditor es un editor de texto creado para el proyecto Alarife4.
  * Licencia GNU GPL
  * 2011
  * Herlin R. Matos Lastres <hmatos@grm.uci.cu>
  * Last modified: 2018/03/04
  * By: Maikel Enrique Pernía Matos <perniamatos@infomed.sld.cu>
  */
#ifndef EDITOR_H
#define EDITOR_H

#include <QtGui/QMainWindow>
#include <QProgressBar>

#include "itemid.h"
#include "webview.h"
#include "alarife.h"


class Editor : public QMainWindow
{
    Q_OBJECT
public:
    Editor(QWidget *parent = 0, Book **encyc = 0);
    ~Editor();
    void setItemId(ItemId);
private slots:
    void saveDocument();
    void showPropertyDocument();

    void showReference();
    void showAllReference();
    void insertReference(QString);
    void deleteReference(QString, QString);

    void spell();
    void help();
    void changeDocument();
    void createWebView();
    void showDocument();
    void loadDocument();


signals:
    void updateGridDetail(int);
public slots:
    void openDocument();
    void insertText(QMap<QString, QString>);
    void insMedias();
    void refresh();
    void showSlide();

private:
    void closeEvent(QCloseEvent *);

    QProgressBar *progressBar;
    QToolBar *toolBar;
    QStatusBar *statusBar;
    QString fn;
    QString originalDoc;
    ItemId itemID;
    WebView *webview;
    QString titleDocument;
    Book *encyclopedia;
    QString line;
};

#endif // EDITOR_H
