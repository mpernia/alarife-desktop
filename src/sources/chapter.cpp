#include <QtGui>
#include <QDebug>
#include <QWebFrame>

#include "../includes/sqlfunctions.h"
#include "../includes/property.h"
#include "../includes/chapter.h"
#include "../includes/spellerdlg.h"
#include "../includes/slide.h"
#include "../includes/baseditor.h"
#include "../includes/strfunction.h"

Chapter::Chapter(QWidget *parent, int father, int index, int opt, Book **encyc) : QMainWindow(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    encyclopedia = (*encyc);

    valueBook = encyclopedia->getIndex();
    valueFather = father;
    valueUser = encyclopedia->getUserIndex();
    valueIndex = index;
    valueOpt = opt;

    valueOpt == 0 ? setWindowTitle(tr("Insertar tema")) : setWindowTitle(tr("Editar tema"));

    setMinimumSize(650,400);

    setWindowFlags(Qt::Dialog);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,
                                    Qt::AlignCenter,
                                    this->size(),
                                    qApp->desktop()->availableGeometry()));

    basEditor = new BasEditor(this, valueIndex, encyc);
    connect(basEditor,SIGNAL(loadFirstTime()),this,SLOT(loadValues()));
    connect(basEditor->page(),SIGNAL(contentsChanged()),this,SLOT(modif()));

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

    actGallery = new QAction(QIcon(":/images/galeria.png"), tr("Galería de imágenes"), this);
    actGallery->setStatusTip(tr("Seleccionar imagenes para galería..."));
    connect(actGallery, SIGNAL(triggered()), this, SLOT(showGallery()));

    edtTitle = new QLineEdit(this);

    QLabel *lblText = new QLabel(tr("Título: "));

    tlbStandard = new QToolBar(tr("Estándar"),this);

    tlbStandard->addWidget(lblText);
    tlbStandard->addWidget(edtTitle);
    tlbStandard->addAction(actSave);

    if (valueOpt == 0)
    {
        actNew = new QAction(QIcon(":/images/nivel_new.png"), tr("Nuevo"), this);
        actNew->setStatusTip(tr("Crear nuevo tema..."));
        connect(actNew, SIGNAL(triggered()), this, SLOT(newChapter()));
        tlbStandard->addAction(actNew);
    }

    tlbStandard->addSeparator();
    tlbStandard->addAction(actInsertMedia);
    tlbStandard->addAction(actInsertSlide);
    tlbStandard->addSeparator();
    tlbStandard->addAction(actSpell);
    tlbStandard->addSeparator();


    addToolBar(tlbStandard);
    setCentralWidget(basEditor);
}
//----------------------------------------------------------------------------------------------
void Chapter::closeEvent(QCloseEvent *event){
    if(isWindowModified() || edtTitle->isModified() || basEditor->isModified()){
        int mess = QMessageBox::warning(this, this->windowTitle(),
                                        tr("El contenido del tema ha sido modificado.\n¿Desea guardar los cambios?"),
                                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        switch(mess){
            case QMessageBox::Save: saveDocument();
                 event->accept();
                 break;
            case QMessageBox::Discard: event->accept();
                 break;
            case QMessageBox::Cancel: event->ignore();
                 break;
        }
    }
}
//----------------------------------------------------------------------------------------------
void Chapter::insertText(QMap<QString, QString> words)
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

    QMapIterator<QString, QString> iter(words); // maikel

    while (iter.hasNext())                      // maikel
    {
        iter.next();
        originalDoc.replace(iter.key(), iter.value());
    }

    if (!words.isEmpty())
    {
        originalDoc = originalDoc.simplified();
        QString jsCode = "CKEDITOR.instances.editor.setData('"+originalDoc+"')";
        basEditor->page()->mainFrame()->evaluateJavaScript(jsCode);
        basEditor->setModified(true);
    }
}
//----------------------------------------------------------------------------------------------
void Chapter::spell()
{
    originalDoc = basEditor->page()->mainFrame()->evaluateJavaScript("CKEDITOR.instances.editor.getData()").toString();

    SpellerDialog *speller = new SpellerDialog(this);
    connect(speller, SIGNAL(textSpelled(QMap<QString,QString>)), this, SLOT(insertText(QMap<QString,QString>)));
    speller->setText(strHtmlDecode(originalDoc));
    speller->show();
}

void Chapter::saveDocument()
{
    if (!edtTitle->text().isEmpty())
    {
        if (valueOpt == 0)
            sqlInsertRecord_N("nivel",
                            "id_nivel, nombre, padre, usuario, enciclopedia, orden",
                            QString("%1, <&~%2~&>, %3, %4, %5, %6").
                                    arg(valueIndex).
                                    arg(edtTitle->text()).
                                    arg(valueFather).
                                    arg(valueUser).
                                    arg(valueBook).
                                    arg(sqlAutoOrder(valueFather, valueBook)));
        else
            sqlUpdateRecord_N("nivel",
                            QString("nombre = <&~%1~&>").arg(edtTitle->text()),
                            QString("WHERE id_nivel = %1 AND enciclopedia = %2").
                            arg(valueIndex).arg(valueBook));
        basEditor->saveData();

        setWindowModified(false);
        edtTitle->setModified(false);
        emit updateGrid(valueFather);
        valueOpt = 1;
    }else
        QMessageBox::critical(this,
                              this->windowTitle(),
                              tr("Debe definir un título para el nivel actual."));

}

void Chapter::newChapter()
{
    edtTitle->clear();
    valueIndex++;
    valueOpt = 0;

    basEditor->newData(valueIndex);
}

void Chapter::showXMedias()
{
    basEditor->insertMediaDlg();
}

void Chapter::showGallery()
{

}

void Chapter::showSlide()
{
    Slide *slide = new Slide(this, valueIndex, 'n');
    slide->setMediaPath(encyclopedia->getMediaPath());
    slide->show();
}

void Chapter::loadValues()
{
    if (valueOpt > 0)
    {
        QSqlQuery *qryLoad = new QSqlQuery;
        qryLoad->exec(QString("SELECT nombre, resumen FROM nivel WHERE id_nivel = %1").arg(valueIndex));
        qryLoad->first();

        edtTitle->setText(qryLoad->value(0).toString());
        basEditor->insertData(qryLoad->value(1).toString());

        delete qryLoad;
    }
}

void Chapter::modif()
{
    setWindowModified(true);
}
