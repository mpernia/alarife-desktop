#include <QtGui>
#include <QDebug>
#include <QWebFrame>

#include "../includes/sqlfunctions.h"
#include "../includes/property.h"
#include "../includes/homepage.h"
#include "../includes/spellerdlg.h"
#include "../includes/slide.h"
#include "../includes/gallerydlg.h"
#include "../includes/strfunction.h"

HomePage::HomePage(QWidget *parent, Book **encyc) : QMainWindow(parent){
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    setWindowTitle(tr("Propiedades de la enciclopedia"));
    setMinimumSize(750,550);

    setWindowFlags(Qt::Dialog);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,
                                    Qt::AlignCenter,
                                    this->size(),
                                    qApp->desktop()->availableGeometry()));

    encyclopedia = (*encyc);
    id_enc = encyclopedia->getIndex();

    miniEditor = new MinEditor(this, (*encyc));

    connect(miniEditor,SIGNAL(loadFirstTime()),this,SLOT(loadValues()));
    connect(miniEditor->page(),SIGNAL(contentsChanged()),this,SLOT(modif()));

    actSave = new QAction(QIcon(":/images/edit-save.png"), tr("Guardar"), this);
    actSave->setStatusTip(tr("Guardar..."));
    actSave->setShortcut(Qt::CTRL + Qt::Key_S);
    connect(actSave, SIGNAL(triggered()), this, SLOT(saveDocument()));

    actInsertMedia = new QAction(QIcon(":/images/edit-insert-media.png"), tr("Insertar X-Medias"), this);
    actInsertMedia->setStatusTip(tr("Insertar X-Medias..."));
    connect(actInsertMedia, SIGNAL(triggered()), this, SLOT(showXMedias()));

    actInsertSlide = new QAction(QIcon(":/images/slideshow.png"), tr("Insertar imágen deslizante"), this);
    actInsertSlide->setStatusTip(tr("Insertar imágen deslizante..."));
    connect(actInsertSlide, SIGNAL(triggered()), this, SLOT(showSlide()));

    actSpell = new QAction(QIcon(":/images/spelling.png"), tr("Ortografía"), this);
    actSpell->setStatusTip(tr("Rectificar ortografía..."));
    connect(actSpell, SIGNAL(triggered()), this, SLOT(spell()));

    actProperty = new QAction(QIcon(":/images/property.png"), tr("Configuración"), this);
    actProperty->setStatusTip(tr("Preferencias de la enciclopedia..."));
    connect(actProperty, SIGNAL(triggered()), this, SLOT(showProperty()));

    actGallery = new QAction(QIcon(":/images/galeria.png"), tr("Galería de imágenes"), this);
    actGallery->setStatusTip(tr("Seleccionar imagenes para galería..."));
    connect(actGallery, SIGNAL(triggered()), this, SLOT(showGallery()));

    actHelp = new QAction(QIcon(":/images/edit-help.png"), tr("Ayuda"), this);
    actHelp->setStatusTip(tr("Manual de Ayuda..."));
    connect(actHelp, SIGNAL(triggered()), this, SLOT(showHelp()));

    edtTitle = new QLineEdit(this);

    QLabel *lblText = new QLabel(tr("Título: "));

    tlbStandard = new QToolBar(tr("Estándar"),this);

    tlbStandard->addWidget(lblText);
    tlbStandard->addWidget(edtTitle);
    tlbStandard->addAction(actSave);
    tlbStandard->addSeparator();
    tlbStandard->addAction(actInsertMedia);
    tlbStandard->addAction(actInsertSlide);
    tlbStandard->addAction(actGallery);
    tlbStandard->addSeparator();
    tlbStandard->addAction(actSpell);
    tlbStandard->addSeparator();
    tlbStandard->addAction(actProperty);
    tlbStandard->addAction(actHelp);

    addToolBar(tlbStandard);
    setCentralWidget(miniEditor);
}

void HomePage::closeEvent(QCloseEvent *event)
{
    if(isWindowModified() || edtTitle->isModified() || miniEditor->isModified()){
        int opt = QMessageBox::warning(this, tr("Propiedades de la enciclopedia"),
                                       tr("Las propiedades de la enciclopedia han sido modificadas.\n¿Desea guardar los cambios?"),
                                       QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        switch(opt)
        {
            case QMessageBox::Save: saveDocument();
                 event->accept();
                 break;
            case QMessageBox::Discard: event->accept();
                 break;
            case QMessageBox::Cancel: event->ignore();
                 break;
        }
    }
    emit updateEncInfo();
}

void HomePage::insertText(QMap<QString, QString> words)
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
        QString jsCode = "CKEDITOR.instances.editor.setData('" + originalDoc + "')";
        miniEditor->page()->mainFrame()->evaluateJavaScript(jsCode);
        miniEditor->setModified(true);
    }
}

void HomePage::spell()
{
    originalDoc = miniEditor->page()->mainFrame()->evaluateJavaScript("CKEDITOR.instances.editor.getData()").toString();

    SpellerDialog *speller = new SpellerDialog(this);
    connect(speller, SIGNAL(textSpelled(QMap<QString,QString>)), this, SLOT(insertText(QMap<QString,QString>)));
    speller->setText(strHtmlDecode(originalDoc));
    speller->show();
}

void HomePage::saveDocument()
{
    sqlUpdateRecord_N("enciclopedia",
                    QString("nombre = <&~%1~&>").arg(edtTitle->text()),
                    QString("WHERE id_enciclopedia = %1").arg(id_enc));
    miniEditor->saveData();

    setWindowModified(false);
    edtTitle->setModified(false);
}

void HomePage::showXMedias()
{
    miniEditor->insertMediaDlg();
}

void HomePage::showProperty()
{
    PropertyDlg *property = new PropertyDlg(this, &encyclopedia);
    property->show();
}

void HomePage::showGallery()
{
    GalleryDlg *gallery = new GalleryDlg(this, encyclopedia->getMediaPath());
    gallery->show();
}

void HomePage::showHelp()
{

}


void HomePage::showSlide()
{
    Slide *slide = new Slide(this, id_enc, 'i');
    slide->setMediaPath(encyclopedia->getMediaPath());
    slide->show();
}

void HomePage::loadValues()
{
    QSqlQuery *qryLoad = new QSqlQuery;
    qryLoad->exec("SELECT nombre, resumen FROM enciclopedia "
                  "WHERE id_enciclopedia = 1");
    qryLoad->first();

    edtTitle->setText(qryLoad->value(0).toString());
    miniEditor->insertData(qryLoad->value(1).toString());

    delete qryLoad;
}

void HomePage::modif()
{
    setWindowModified(true);
}
