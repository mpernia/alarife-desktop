/*
  * QJSEditor es un editor de texto creado para el proyecto Alarife4.
  * Licencia GNU GPL
  * 2011
  * Herlin R. Matos Lastres <hmatos@grm.uci.cu>
  * Last modified: 2018/03/04
  * By: Maikel Enrique Pernía Matos <perniamatos@infomed.sld.cu>
  */

#include <QtGui>
#include <QWebFrame>
#include <QStackedWidget>
#include <QSqlQuery>
#include <QSqlError>

#include "../includes/editor.h"
#include "../includes/webview.h"
#include "../includes/htmlmanager.h"
#include "../includes/spellerdlg.h"    
#include "../includes/insertmediadlg.h"
#include "../includes/itemproperty.h"  
#include "../includes/slide.h"         
#include "../includes/sqlfunctions.h"  
#include "../includes/strfunction.h"   
#include "../includes/reference.h"     
#include "../includes/strfunction.h"   

Editor::Editor(QWidget *parent, Book **encyc) : QMainWindow(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);

    toolBar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, toolBar);

    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    encyclopedia = (*encyc);

    QAction *saveDoc =  toolBar->addAction(QIcon(":/images/edit-save.png"),tr("Guardar"));
    QAction *propertyDoc =  toolBar->addAction(QIcon(":/images/articulo_property.png"),
                                               tr("Propiedades del artículo"));
    QAction *InsMedia =  toolBar->addAction(QIcon(":/images/edit-insert-media.png"),
                                                tr("Insertar Medias"));
    QAction *InsSlide =  toolBar->addAction(QIcon(":/images/slideshow.png"),
                                                tr("Insertar imágenes deslizantes"));

    QAction *actlink =  toolBar->addAction(QIcon(":/images/action_reference_insert.png"),
                                                tr("Referencia"));
    QAction *actRef =  toolBar->addAction(QIcon(":/images/action_reference_show.png"),
                                           tr("Mostrar todas las referencia"));

    QAction *spelling =  toolBar->addAction(QIcon(":/images/spelling.png"),
                                                tr("Corrector ortográfico"));
    QAction *refresh =  toolBar->addAction(QIcon(":/images/refresh.png"),
                                                tr("Recargar"));
    QAction *help =  toolBar->addAction(QIcon(":/images/edit-help.png"),tr("Ayuda"));


    connect(InsMedia, SIGNAL(triggered()), this, SLOT(insMedias()));
    connect(InsSlide, SIGNAL(triggered()), this, SLOT(showSlide()));
    connect(saveDoc, SIGNAL(triggered()), this, SLOT(saveDocument()));
    connect(propertyDoc, SIGNAL(triggered()), this, SLOT(showPropertyDocument()));
    connect(spelling, SIGNAL(triggered()), this, SLOT(spell()));
    connect(refresh, SIGNAL(triggered()), this, SLOT(refresh()));
    connect(help,SIGNAL(triggered()),this,SLOT(help()));

    connect(actlink, SIGNAL(triggered()), this, SLOT(showReference()));
    connect(actRef, SIGNAL(triggered()), this, SLOT(showAllReference()));
}

Editor::~Editor()
{

}

void Editor::showReference()
{
    Reference *refDlg = new Reference(this, itemID.getItem());
    refDlg->setMode(Reference::INSERT);
    refDlg->setSelectedText(webview->page()->selectedText());
    refDlg->show();
    connect(refDlg, SIGNAL(linked(QString)), this, SLOT(insertReference(QString)));
}

void Editor::showAllReference()
{
    Reference *refDlg = new Reference(this, itemID.getItem());
    refDlg->setMode(Reference::READONLY);
    refDlg->show();
    connect(refDlg, SIGNAL(unlinked(QString,QString)), this, SLOT(deleteReference(QString,QString)));
}

void Editor::spell()
{
    originalDoc = webview->page()->mainFrame()->evaluateJavaScript("CKEDITOR.instances.editor.getData()").toString();

    SpellerDialog *speller = new SpellerDialog(this);
    connect(speller, SIGNAL(textSpelled(QMap<QString,QString>)), this, SLOT(insertText(QMap<QString,QString>)));
    speller->setText(strHtmlDecode(originalDoc));
    speller->show();
}

void Editor::insertText(QMap<QString,QString> words)
{
    originalDoc.replace("&aacute;",QString::fromUtf8("á"));
    originalDoc.replace("&eacute;",QString::fromUtf8("é"));
    originalDoc.replace("&iacute;",QString::fromUtf8("í"));
    originalDoc.replace("&oacute;",QString::fromUtf8("ó"));
    originalDoc.replace("&uacute;",QString::fromUtf8("ú"));

    originalDoc.replace("&Aacute;",QString::fromUtf8("Á"));
    originalDoc.replace("&Eacute;",QString::fromUtf8("É"));
    originalDoc.replace("&Iacute;",QString::fromUtf8("Í"));
    originalDoc.replace("&Oacute;",QString::fromUtf8("Ó"));
    originalDoc.replace("&Uacute;",QString::fromUtf8("Ú"));

    originalDoc.replace("&uuml;",QString::fromUtf8("ü"));

    QMapIterator<QString, QString> iter(words);

    while (iter.hasNext())
    {
        iter.next();
        originalDoc.replace(iter.key(), iter.value());
    }

    if (!words.isEmpty())
    {
        originalDoc = originalDoc.simplified();
        QString jsCode = "CKEDITOR.instances.editor.setData('"+originalDoc+"')";
        webview->page()->mainFrame()->evaluateJavaScript(jsCode);
        webview->setModified(true);
    }

}

void Editor::showPropertyDocument()
{
    ItemProperty *itp = new ItemProperty(*webview->getItemId(),this);
    connect(itp, SIGNAL(refreshValues()), this, SLOT(refresh()));
    itp->show();
}

void Editor::saveDocument()
{
    QString document = webview->page()->mainFrame()->evaluateJavaScript("CKEDITOR.instances.editor.getData()").toString();

    HtmlManager htmlManager(itemID.getPath());
    document = htmlManager.prepareToSave(document);
    document.replace("'", "&#39;");

    if(document.contains("<body")) document = selectUndefineArea(document,"body");

    document.remove("<p> <style type=\"text/css\"> reference{color: blue; "
                    "text-decoration: underline;font-style: italic; font-weight: "
                    "bold;} </style> </p>",
                    Qt::CaseSensitive);

    if (sqlUpdateRecord_N("articulo",
                        QString("contenido=<&~%1~&>, fecha2=<&~%2~&>").
                                arg(document).arg(getActualDataTime()),
                        QString("WHERE id_articulo = %1").arg(itemID.getItem())))
    {
        webview->setModified(false);
        emit updateGridDetail(sqlGetIdValue("articulo", "nivel", QString("id_articulo = %1").arg(itemID.getItem())));
    }
}

void Editor::openDocument()
{
    loadDocument();
    qDebug() << "aqui " << line << endl;

    QString jsCode = QString("CKEDITOR.instances.editor.setData('<style type=\"text/css\">"
                             "reference{color: blue; text-decoration: underline;"
                             "font-style: italic; font-weight: bold;}</style>" + line + "')");

    webview->page()->mainFrame()->evaluateJavaScript(jsCode);

    webview->setModified(false);
}

void Editor::closeEvent(QCloseEvent *event)
{
    if (itemID.isOwner())
    {
        if(webview->isMod()){
            int ret = QMessageBox::warning(this, tr("Alarife-4"),
                                            tr("El documento ha sido modificado.\n"
                                               "¿Desea guardar los cambios?"),
                                            QMessageBox::Save | QMessageBox::Discard |
                                            QMessageBox::Cancel, QMessageBox::Save);

            switch(ret){
                case QMessageBox::Save:
                    saveDocument();
                    break;
                case QMessageBox::Cancel:
                    event->ignore();
                    return;
            };
        }
    }else
        QFile(__TMP_DIR__ + "/read.html").remove();


    event->accept();

    int id = sqlGetIdValue("articulo",
                           "nivel",
                           QString("id_articulo = %1").arg(itemID.getItem()));
    emit updateGridDetail(id);
}

void Editor::help()
{
    QMessageBox::about(this, tr("Acerca de ..."),
                       tr("<b>QJSEditor</b> es un editor de texto creado para <b>Alarife 4</b> por:\n"
                          "Herlin R. Matos Lastres - (hmatos@grm.uci.cu) y\n"
                          "Maikel E. Pernia Matos - (perniamatos@infomed.sld.cu)"));
}

void Editor::changeDocument()
{
    if (!webview->isMod())
    {
        webview->setModified(true);
        this->setWindowTitle(tr("Alarife-4 [*%1]").arg(titleDocument));
    }
}

void Editor::createWebView()
{
    webview = new WebView(itemID, this);
    this->setCentralWidget(webview);
    webview->page()->setContentEditable(true);

    progressBar = new QProgressBar(statusBar);
    progressBar->setMaximumSize(100,13);
    progressBar->setAlignment(Qt::AlignRight);
    statusBar->addWidget(progressBar);

    connect(webview,SIGNAL(loadProgress(int)),progressBar,SLOT(setValue(int)));
    connect(webview,SIGNAL(loadFinished(bool)),progressBar,SLOT(deleteLater()));
    connect(webview,SIGNAL(loadFirstTime()),this,SLOT(openDocument()));
    connect(webview->page(),SIGNAL(contentsChanged()),this,SLOT(changeDocument()));
}

void Editor::setItemId(ItemId i)
{
    itemID = i;
    toolBar->setVisible(itemID.isOwner());
    itemID.isOwner() ? createWebView() : showDocument();

}

void Editor::showDocument()
{
    QWebView *view = new QWebView;
    this->setCentralWidget(view);
    loadDocument();

    QFile file(__TMP_DIR__ + "/read.html");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    line.replace("<head></head>",
                 "<head><meta content=\"text/html; charset=utf-8\" http-equiv=\"content-type\" /></head>",
                 Qt::CaseSensitive);
    out << line;
    view->setUrl(QUrl::fromLocalFile(__TMP_DIR__ + "/read.html"));
}

void Editor::insMedias()
{
    InsertMediaDlg *media = new InsertMediaDlg(this, webview, encyclopedia);
    media->exec();
}

void Editor::insertReference(QString link)
{
    QString jsCode = QString("CKEDITOR.instances.editor.insertHtml('%1')").arg(link);
    webview->page()->mainFrame()->evaluateJavaScript(jsCode);
    saveDocument();
}

void Editor::deleteReference(QString link, QString text)
{
    originalDoc = webview->page()->mainFrame()->evaluateJavaScript("CKEDITOR.instances.editor.getData()").toString();
    originalDoc = originalDoc.simplified();
    originalDoc.replace(QRegExp(link), text);
    QString jsCode = QString("CKEDITOR.instances.editor.setData('%1')").arg(originalDoc);
    webview->page()->mainFrame()->evaluateJavaScript(jsCode);
    saveDocument();
}

void Editor::refresh()
{
    if(webview->isMod()){
        int ret = QMessageBox::warning(this, tr("Alarife-4"),
                                       tr("El documento ha sido modificado.\n"
                                          "¿Desea guardar los cambios antes de recargar el contenido?"),
                                       QMessageBox::Yes | QMessageBox::No);

        if (ret == QMessageBox::Yes) saveDocument();
    }

    delete webview;
    createWebView();
}

void Editor::showSlide()
{
    Slide *slide = new Slide(this, itemID.getItem(), 'a');
    slide->setMediaPath(encyclopedia->getMediaPath());
    slide->show();
}


void Editor::loadDocument()
{
    QSqlQuery *q = new QSqlQuery;
    q->exec(QString("SELECT contenido, nombre FROM articulo WHERE id_articulo = %1").
            arg(itemID.getItem()));
    q->first();

    if (q->isValid())
    {
        titleDocument = q->value(1).toString();
        this->setWindowTitle(tr("Alarife-4 [%1]").arg(titleDocument));

        HtmlManager *htmlManager = new HtmlManager(itemID.getPath());

        line = htmlManager->open(q->value(0).toString());

        line.replace("'", "&#39;");
    }
}
