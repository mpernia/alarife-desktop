/****************************************************************************
**
** Copyleft: 2012. Maikel Enrique Pernia Matos.
** Contact: corba@grannet.grm.sld.cu
**
** This file is part of the Alarife-4 application.
**
** GNU General Public License Usage
**
** This file may be used under the terms of the GNU General Public License;
** either version 2 of the License, or (at your option) any later version,
** as published by the Free Software Foundation and appearing in the file
** LICENSE.GPL included in the packaging of this file.  Please review the
** following information to ensure the GNU General Public License version 2
** requirements will be met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/

#include <QtGui>
#include <QApplication>
#include <QProgressDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>
#include <QFrame>
#include <QDateTime>
#include <QString>
#include <string>

#include "../includes/alarife.h"
#include "../includes/about.h"
#include "../includes/authors.h"
#include "../includes/config.h"
#include "../includes/chapter.h"
#include "../includes/database.h"
#include "../includes/export.h"
#include "../includes/genalf.h"
#include "../includes/import.h"
#include "../includes/login.h"
#include "../includes/mainwindow.h"
#include "../includes/media.h"
#include "../includes/movechapterdlg.h"
#include "../includes/movearticledlg.h"
#include "../includes/newbook.h"
#include "../includes/orders.h"
#include "../includes/property.h"
#include "../includes/stadistic.h"
#include "../includes/editor.h"
#include "../includes/itemid.h"
#include "../includes/sqlfunctions.h"
#include "../includes/homepage.h"
#include "../includes/dlgwaitproc.h"
#include "../includes/itemproperty.h"
#include "../includes/moveitem.h"
#include "../includes/strfunction.h"

#include "../includes/dlgadminuser.h"
#include "../includes/dlgadmintemplates.h"
#include "../includes/dlgadminstyles.h"

#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    AlarifeDb = new DbConnect();
    encyclopedia = new Book();

    master = new QSqlQueryModel;
    detail = new QSqlQueryModel;
    stack = new StackId;
    stack->PushId(0,0);
    visibleTraceBar = -1;

    trace.push("");

    edit = NULL;
    isFull = false;

    CreateActions();
    CreateMenus();
    CreateToolBars();
    CreateStatusBar();
    ReadSettings();
    windowtitle = "";
    setWindowTitle(tr(__APP_NAME__));
    CreatePanels();
    EnabledAccion(false);
    EnabledArticlesEdit(false);
    EnabledLevelsEdit(false);
    createContextMenu();
    updateRecentFileActions();
}

void MainWindow::setLoatFile(QString loadFile)
{
    if (!loadFile.isEmpty())
    {
        AlarifeDb->ConnectionString(loadFile);
        AlarifeDb->Active(true);

        if (AlarifeDb->Connected() == true)
        {
            ShowLoginDlg();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isFull == true)
    {
        QMessageBox::question(this, tr("Cerrar Alarife"),
                              tr("Actualmente se está realizando un proceso que "
                                 "aún no ha finalizado. Por favor espere..."));
        event->ignore();
    }else{
        if (AlarifeDb->Connected() == true)
        {
            int btn = QMessageBox::question(this, tr("Cerrar Alarife"),
                                            tr("Actualmente esta en edición la enciclopedia "
                                               "\"%1\". ¿Desea salir?").arg(encyclopedia->getTitle()),
                                            QMessageBox::Yes | QMessageBox::No);
            if (btn == QMessageBox::Yes)
            {
                AlarifeDb->Active(false);
                WriteSettings();
                event->accept();
            }else event->ignore();
        }else{
            WriteSettings();
            event->accept();
        }
    }

}

void MainWindow::createContextMenu()
{
    MasterView->addAction(actChapterAdd);
    MasterView->addAction(actChapterDelete);
    MasterView->addAction(actChapterDelAll);
    MasterView->addAction(actChapterEdit);
    MasterView->addAction(actChapterOrder);
    MasterView->addAction(actChapterByName);

    DetailView->addAction(actArticleAdd);
    DetailView->addAction(actArticleDelete);
    DetailView->addAction(actArticleDelAll);
    DetailView->addAction(actArticleEdit);
    DetailView->addAction(actArticleOrder);
    DetailView->addAction(actArticleByName);

    DetailView->addAction(actArticleProperty);
    DetailView->addAction(actMoveElement);

    MasterView->setContextMenuPolicy(Qt::ActionsContextMenu);
    DetailView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::CreateActions()
{
    actNew = new QAction(QIcon(":/images/action_new.png"), tr("Nuevo"), this);
    actNew->setShortcuts(QKeySequence::New);
    actNew->setStatusTip(tr("Crea una nueva enciclopedi­a."));
    connect(actNew, SIGNAL(triggered()), this, SLOT(NewExec()));

    actOpen = new QAction(QIcon(":/images/open.png"), tr("Abrir..."), this);
    actOpen->setShortcuts(QKeySequence::Open);
    actOpen->setStatusTip(tr("Abrir  una enciclopedi­a existente."));
    connect(actOpen, SIGNAL(triggered()), this, SLOT(OpenExec()));

    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        actRecent[i] = new QAction(this);
        actRecent[i]->setVisible(false);
        connect(actRecent[i], SIGNAL(triggered()), this, SLOT(fileRecentOpen()));
    }

    actClearRecent = new QAction(QIcon::fromTheme("edit-clear", QIcon(":/images/edit-clear.png")),
                                 tr("Borrar historial"), this);
    actClearRecent->setPriority(QAction::LowPriority);
    actClearRecent->setStatusTip(tr("Borrar historial de archivos recientes."));
    connect(actClearRecent, SIGNAL(triggered()), this, SLOT(clearRecents()));

    actStadistic = new QAction(QIcon(":/images/stadistic.png"), tr("Estadística"), this);
    actStadistic->setStatusTip(tr("Estadística de la enciclopedia."));
    actStadistic->setShortcut(tr("F4"));
    connect(actStadistic, SIGNAL(triggered()), this, SLOT(StadisticExec()));

    actHomePage = new QAction(QIcon(":/images/action_home.png"), tr("Propiedades"), this);
    actHomePage->setStatusTip(tr("Propiedades de la enciclopedia."));
    actHomePage->setShortcut(tr("F2"));
    connect(actHomePage, SIGNAL(triggered()), this, SLOT(HomePageExec()));

    actProperty = new QAction(QIcon(":/images/property.png"), tr("Configuración"), this);
    actProperty->setStatusTip(tr("Configuración de la enciclopedia."));
    actProperty->setShortcut(tr("F3"));
    connect(actProperty, SIGNAL(triggered()), this, SLOT(PropertyExec()));

    actAuthor = new QAction(QIcon(":/images/actores.png"), tr("Autores"), this);
    actAuthor->setStatusTip(tr("Actores de la enciclopedia."));
    actAuthor->setShortcut(tr("F5"));
    connect(actAuthor, SIGNAL(triggered()), this, SLOT(AuthorsExec()));

    actMedia = new QAction(QIcon(":/images/medias.png"), tr("Medias"), this);
    actMedia->setStatusTip(tr("Medias de la enciclopedia."));
    actMedia->setShortcut(tr("F6"));
    connect(actMedia, SIGNAL(triggered()), this, SLOT(MediaExec()));

    actExit = new QAction(QIcon(":/images/exit.png"), tr("Salir"), this);
    actExit->setShortcut(tr("Ctrl+Q"));
    actExit->setStatusTip(tr("Terminar la aplicación."));
    connect(actExit, SIGNAL(triggered()), qApp, SLOT(quit()));

    actChapterAdd = new QAction(QIcon(":/images/nivel_new.png"), tr("Agregar tema"), this);
    actChapterAdd->setStatusTip(tr("Agregar nuevo tema."));
    connect(actChapterAdd, SIGNAL(triggered()), this, SLOT(ChapterAddExec()));

    actChapterEdit = new QAction(QIcon(":/images/nivel_edit.png"), tr("Editar tema"), this);
    actChapterEdit->setStatusTip(tr("Editar tema actual."));
    connect(actChapterEdit, SIGNAL(triggered()), this, SLOT(ChapterEditExec()));

    actChapterDelete = new QAction(QIcon(":/images/nivel_delete.png"), tr("Eliminar tema"), this);
    actChapterDelete->setStatusTip(tr("Eliminar tema actual."));
    connect(actChapterDelete, SIGNAL(triggered()), this, SLOT(ChapterDeleteExec()));

    actChapterDelAll = new QAction(QIcon(":/images/nivel_delete.png"), tr("Eliminar todos"), this);
    actChapterDelAll->setStatusTip(tr("Eliminar todos los temas."));
    connect(actChapterDelAll, SIGNAL(triggered()), this, SLOT(ChapterDeleteAll()));

    actChapterOrder = new QAction(QIcon(":/images/action_order_level.png"), tr("Cambiar posición"), this);
    actChapterOrder->setStatusTip(tr("Cambia la posición en que se ordenan los temas."));
    connect(actChapterOrder, SIGNAL(triggered()), this, SLOT(ChapterOrderExec()));

    actChapterByName = new QAction(QIcon(":/images/check_order_level.png"), tr("Ordenar alfabéticamente"), this);
    actChapterByName->setStatusTip(tr("Mostrar temas ordenados alfabéticamente."));
    actChapterByName->setCheckable(true);
    connect(actChapterByName, SIGNAL(triggered(bool)), this, SLOT(ChapterOrder(bool)));

    actArticleAdd = new QAction(QIcon(":/images/articulo_new.png"), tr("Agregar artículo"), this);
    actArticleAdd->setStatusTip(tr("Agregar nuevo artículo."));
    connect(actArticleAdd, SIGNAL(triggered()), this, SLOT(ArticleAddExec()));

    actArticleEdit = new QAction(QIcon(":/images/articulo_edit.png"), tr("Editar artículo"), this);
    actArticleEdit->setStatusTip(tr("Editar artículo actual."));
    connect(actArticleEdit, SIGNAL(triggered()), this, SLOT(ArticleEditExec()));

    actArticleDelete = new QAction(QIcon(":/images/articulo_delete.png"), tr("Eliminar artículo"), this);
    actArticleDelete->setStatusTip(tr("Eliminar artículo actual."));
    connect(actArticleDelete, SIGNAL(triggered()), this, SLOT(ArticleDeleteExec()));

    actArticleDelAll = new QAction(QIcon(":/images/articulo_delete.png"), tr("Eliminar todos"), this);
    actArticleDelAll->setStatusTip(tr("Eliminar todos los artículo."));
    connect(actArticleDelAll, SIGNAL(triggered()), this, SLOT(ArticleDeleteAll()));

    actArticleProperty= new QAction(QIcon(":/images/articulo_property.png"), tr("Propiedades del  artículo"), this);
    actArticleProperty->setStatusTip(tr("Propiedeades del artículo."));
    connect(actArticleProperty, SIGNAL(triggered()), this, SLOT(ActArticlePropertyExec()));

    actArticleOrder = new QAction(QIcon(":/images/action_order_articles.png"), tr("Cambiar posición"), this);
    actArticleOrder->setStatusTip(tr("Cambia la posición en que se ordenan los artículos en el tema actual."));
    connect(actArticleOrder, SIGNAL(triggered()), this, SLOT(ArticleOrderExec()));

    actArticleByName = new QAction(QIcon(":/images/check_order_articles.png"), tr("Ordenar alfabéticamente"), this);
    actArticleByName->setStatusTip(tr("Mostrar artículos ordenados alfabéticamente."));
    actArticleByName->setCheckable(true);
    connect(actArticleByName, SIGNAL(triggered(bool)), this, SLOT(ArticleOrder(bool)));

    actSearch = new QAction(QIcon(":/images/action_find.png"), tr("Buscar"), this);
    actSearch->setStatusTip(tr("Buscar un contenido."));
    actSearch->setCheckable(true);
    actSearch->setShortcut(Qt::CTRL + Qt::Key_F);
    connect(actSearch, SIGNAL(triggered()), this, SLOT(ToolBarSearchShow()));

    actMoveElement = new QAction(QIcon(":/images/articulo_nivel.png"), tr("Mover artículo y tema"), this);
    actMoveElement->setStatusTip(tr("Mover artículos y temas."));
    connect(actMoveElement, SIGNAL(triggered()), this, SLOT(MoveElementExec()));

    actBookExport = new QAction(QIcon(":/images/action_export.png"), tr("Exportar enciclopedia"), this);
    actBookExport->setStatusTip(tr("Exporta encicplopedia."));
    actBookExport->setShortcut(tr("F7"));
    connect(actBookExport, SIGNAL(triggered()), this, SLOT(BookExportExec()));

    actAddBookImport = new QAction(QIcon(":/images/action_import_1.png"), tr("Importar enciclopedia como tema"), this);
    actAddBookImport->setStatusTip(tr("Importa enciclopedia como un nuevo tema."));
    actAddBookImport->setShortcut(tr("F9"));
    connect(actAddBookImport, SIGNAL(triggered()), this, SLOT(AddBookImportExec()));

    actMergeBookImport = new QAction(QIcon(":/images/action_import_2.png"), tr("Mezclar temas"), this);
    actMergeBookImport->setStatusTip(tr("Importa los temas de la enciclopedia."));
    actMergeBookImport->setShortcut(tr("F10"));
    connect(actMergeBookImport, SIGNAL(triggered()), this, SLOT(MergeBookImportExec()));

    actNewBookImport = new QAction(QIcon(":/images/action_import_0.png"), tr("Importar enciclopedia"), this);
    actNewBookImport->setStatusTip(tr("Importa enciclopedia completamente."));
    actNewBookImport->setShortcut(tr("F8"));
    connect(actNewBookImport, SIGNAL(triggered()), this, SLOT(NewBookImportExec()));

    actGenerated = new QAction(QIcon(":/images/generate.png"), tr("Generar enciclopedia"), this);
    actGenerated->setStatusTip(tr("Generar enciclopedia."));
    actGenerated->setShortcut(tr("F11"));
    connect(actGenerated, SIGNAL(triggered()), this, SLOT(GeneratedHtmlExec()));

    actBenchmark = new QAction(QIcon(":/images/btnAction.png"), tr("Integridad referencial"), this);
    actBenchmark->setStatusTip(tr("Analizar integridad referencial."));
    connect(actBenchmark, SIGNAL(triggered()), this, SLOT(RestructuringData()));

    actContents = new QAction(QIcon(":/images/help.png"), tr("Manual de usuario"), this);
    actContents->setStatusTip(tr("Muestra el manual de ayuda al usuario."));
    actContents->setShortcut(tr("F1"));
    connect(actContents, SIGNAL(triggered()), this, SLOT(ContentsExec()));

    actConfiguration = new QAction(QIcon(":/images/action_aparence.png"), tr("Preferencias"), this);
    actConfiguration->setStatusTip(tr("Preferencias."));
    actConfiguration->setShortcut(tr("F12"));
    connect(actConfiguration, SIGNAL(triggered()), this, SLOT(CheckedBoss()));

    actAbout = new QAction(QIcon(":/images/about.png"), tr("Acerca de..."), this);
    actAbout->setStatusTip(tr("Mostrar realizadores de Alarife."));
    connect(actAbout, SIGNAL(triggered()), this, SLOT(AboutExec()));


    actAdminUser  = new QAction(QIcon(":/images/conf_user.png"), tr("Usuarios"), this);
    actAdminUser->setStatusTip(tr("Administrar usuarios."));
    connect(actAdminUser, SIGNAL(triggered()), this, SLOT(adminUser()));

    actAdminTemplate = new QAction(QIcon(":/images/conf_template.png"), tr("Plantillas"), this);
    actAdminTemplate->setStatusTip(tr("Administrar las plantillas."));
    connect(actAdminTemplate, SIGNAL(triggered()), this, SLOT(adminTemplate()));

    actAdminStyles = new QAction(QIcon(":/images/conf_aparence.png"), tr("Apariencia"), this);
    actAdminStyles->setStatusTip(tr("Modificar la apariencia de Alarife."));
    connect(actAdminStyles, SIGNAL(triggered()), this, SLOT(adminStyles()));
}

void MainWindow::CreateMenus()
{
    menuRecent = new QMenu(tr("Documentos recientes..."));
    menuRecent->setIcon(QIcon::fromTheme("document-open-recent",
                                         QIcon(":/images/document-recent.png")));
    for (int i = 0; i < MaxRecentFiles; ++i) menuRecent->addAction(actRecent[i]);

    mnBook = menuBar()->addMenu(tr("Enciclopedia"));
    mnBook->addAction(actNew);
    mnBook->addAction(actOpen);
    mnBook->addMenu(menuRecent);
    mnBook->addSeparator();
    mnBook->addAction(actClearRecent);
    mnBook->addSeparator();
    mnBook->addAction(actStadistic);
    mnBook->addAction(actHomePage);
    mnBook->addAction(actProperty);
    mnBook->addSeparator();
    mnBook->addAction(actAuthor);
    mnBook->addAction(actMedia);
    mnBook->addSeparator();
    mnBook->addAction(actExit);

    menuBar()->addSeparator();

    mnArticle = new QMenu(tr("Artículos"), this);
    mnArticle->addAction(actArticleAdd);
    mnArticle->addAction(actArticleEdit);
    mnArticle->addAction(actArticleDelete);
    mnArticle->addAction(actArticleProperty);
    mnArticle->addAction(actArticleOrder);
    mnArticle->addAction(actArticleByName);

    mnChapter = new QMenu(tr("Temas"),this);
    mnChapter->addAction(actChapterAdd);
    mnChapter->addAction(actChapterEdit);
    mnChapter->addAction(actChapterDelete);
    mnChapter->addAction(actChapterOrder);
    mnChapter->addAction(actChapterByName);

    mnEdit = menuBar()->addMenu(tr("Edición"));
    mnEdit->addSeparator();
    mnEdit->addMenu(mnArticle);
    mnEdit->addMenu(mnChapter);
    mnEdit->addAction(actMoveElement);
    mnEdit->addSeparator();
    mnEdit->addAction(actSearch);

    mnImport = new QMenu(tr("Importación de enciclopedia"),this);
    mnImport->addAction(actNewBookImport);
    mnImport->addAction(actAddBookImport);
    mnImport->addAction(actMergeBookImport);

    mnTool = menuBar()->addMenu(tr("Herramientas"));
    mnTool->addAction(actBookExport);
    mnTool->addMenu(mnImport);
    mnTool->addAction(actBenchmark);
    mnTool->addSeparator();
    mnTool->addAction(actGenerated);

    menuBar()->addSeparator();

    mnHelp = menuBar()->addMenu(tr("Ayuda"));
    mnHelp->addAction(actAdminUser);
    mnHelp->addAction(actAdminTemplate);
    mnHelp->addAction(actAdminStyles);
    mnHelp->addSeparator();
    mnHelp->addAction(actContents);
    mnHelp->addAction(actAbout);
}

void MainWindow::CreateToolBars()
{
    tlbBook = addToolBar(tr("Enciclopedia"));
    tlbBook->addAction(actNew);
    tlbBook->addAction(actOpen);
    tlbBook->addSeparator();
    tlbBook->addAction(actStadistic);
    tlbBook->addAction(actHomePage);
    tlbBook->addAction(actProperty);
    tlbBook->addSeparator();
    tlbBook->addAction(actAuthor);
    tlbBook->addAction(actMedia);
    tlbBook->addSeparator();
    tlbBook->addAction(actExit);

    tlbTool = addToolBar(tr("Herramientas"));
    tlbTool->addAction(actBookExport);
    tlbTool->addSeparator();
    tlbTool->addAction(actNewBookImport);
    tlbTool->addAction(actAddBookImport);
    tlbTool->addAction(actMergeBookImport);
    tlbTool->addSeparator();
    tlbTool->addAction(actGenerated);

    addToolBarBreak();

    tlbEdit = addToolBar(tr("Editar"));
    tlbEdit->addAction(actSearch);
    tlbEdit->addSeparator();
    tlbEdit->addAction(actArticleAdd);
    tlbEdit->addAction(actArticleEdit);
    tlbEdit->addAction(actArticleDelete);
    tlbEdit->addAction(actArticleProperty);
    tlbEdit->addAction(actArticleOrder);
    tlbEdit->addAction(actArticleByName);
    tlbEdit->addSeparator();
    tlbEdit->addAction(actChapterAdd);
    tlbEdit->addAction(actChapterEdit);
    tlbEdit->addAction(actChapterDelete);
    tlbEdit->addAction(actChapterOrder);
    tlbEdit->addAction(actChapterByName);
    tlbEdit->addSeparator();
    tlbEdit->addAction(actMoveElement);

    tlbHelp = addToolBar(tr("Ayuda"));
    tlbHelp->addAction(actAdminUser);
    tlbHelp->addAction(actAdminTemplate);
    tlbHelp->addAction(actAdminStyles);
    tlbHelp->addSeparator();
    tlbHelp->addAction(actContents);
    tlbHelp->addAction(actAbout);
}

void MainWindow::openTraceBar()
{
    lblTrace = new QLabel;

    tlbTrace = addToolBar(tr("Ruta"));
    tlbTrace->setContentsMargins(10,2,2,0);
    tlbTrace->addWidget(lblTrace);

    addToolBarBreak();
    addToolBar(Qt::TopToolBarArea,tlbTrace);

    visibleTraceBar = 1;
}

void MainWindow::closeTraceBar()
{
    if (visibleTraceBar == 1)
    {
        delete tlbTrace;
        visibleTraceBar = -1;
    }
}


void MainWindow::PushTraceBar(int parent)
{
    QString name;
    int id;

    QSqlQuery *qry = new QSqlQuery;
    qry->exec(QString("SELECT padre, nombre FROM nivel WHERE id_nivel = %1").arg(parent));
    qry->first();
    id = qry->value(0).toInt();
    name = qry->value(1).toString();
    delete qry;

    if (parent != 0)
    {
        Reg reg;
        reg.father = id;
        reg.number = parent;
        lstName.prepend(name);
        lstLevel.prepend(reg);
        PushTraceBar(id);        
    }
}

void MainWindow::CreateStatusBar()
{
    QLabel *lbl = new QLabel;
    lbl->setText(tr("Desarrollado en Manzanillo de Cuba."));
    statusBar()->insertPermanentWidget(0,lbl,0);
}

void MainWindow::ReadSettings()
{

    QSettings preference("Alarife", "Preference");

    QPoint pos = preference.value("pos", QPoint(200, 200)).toPoint();
    QSize size = preference.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);

    if (preference.value("init").toInt() == 0){
        preference.setValue("init", 1);
        preference.setValue("style", "GTK+");
        preference.setValue("stylesheet", "alarife.qss");
        preference.setValue("activeuser", 0);
        encyclopedia->setStyle("GTK+");
        encyclopedia->setStyleSheet("alarife.qss");
        encyclopedia->setActiveUsers(0);
    }else{
        encyclopedia->setStyle(preference.value("style").toString());
        encyclopedia->setStyleSheet(preference.value("stylesheet").toString());
        encyclopedia->setActiveUsers(preference.value("activeuser").toInt());
    }

    qApp->setStyle(encyclopedia->getStyle());
    QString path = QString("%1/styles/%2").arg(__APP_PATH__).arg(encyclopedia->getStyleSheet());
    if (path.contains("/bin")) path.remove("/bin", Qt::CaseSensitive);
    QFile file(path);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    styleSheet.replace("$APP_PATH", __APP_PATH__, Qt::CaseSensitive);
    qApp->setStyleSheet(styleSheet);
}

void MainWindow::WriteSettings()
{
    QSettings preference("Alarife", "Preference");
    preference.setValue("pos", pos());
    preference.setValue("size", size());
    preference.setValue("width", width());
    preference.setValue("lang","es");
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings("Alarife", "RecentFiles");
    QStringList files = settings.value("recentFileList").toStringList();

    if (files.size() < 1)
        actClearRecent->setEnabled(false);
    else
        actClearRecent->setEnabled(true);

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0, j=1; i < numRecentFiles; ++i)
    {
        if (!QFileInfo(files[i]).fileName().isEmpty())
        {
            QString text = tr("&%1 %2").arg(j).arg(QFileInfo(files[i]).fileName());
            actRecent[j]->setText(text);
            actRecent[j]->setStatusTip(tr("%1").arg(files[i]));
            actRecent[j]->setData(files[i]);
            actRecent[j]->setVisible(true);
            j++;
        }
    }

    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        actRecent[j]->setVisible(false);
}

void MainWindow::clearRecents()
{
    QSettings settings("Alarife", "RecentFiles");
    QStringList recentFiles = settings.value("recentFileList").toStringList();

    recentFiles.clear();

    settings.setValue("recentFileList", recentFiles);
    updateRecentFileActions();
}

void MainWindow::writeRecents()
{
    QSettings settings("Alarife", "RecentFiles");
    QStringList files = settings.value("recentFileList").toStringList();

    files.removeAll(AlarifeDb->getPathFile());
    files.prepend(AlarifeDb->getPathFile());

    while (files.size() > MaxRecentFiles) files.removeLast();

    settings.setValue("recentFileList", files);
    updateRecentFileActions();
}

void MainWindow::fileRecentOpen()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) setLoatFile(action->data().toString());
}

void MainWindow::CreatePanels()
{
    dckTreeView = new QDockWidget(tr("Temas de la Enciclopedia"), this);
    dckTreeView->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dckTreeView->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dckTreeView->setMaximumWidth(200);
    cntTreeView = new QWidget;

    btnFirst = new QToolButton;
    btnPrior = new QToolButton;

    btnFirst->setIcon(QIcon(":/images/go-home.png"));
    btnPrior->setIcon(QIcon(":images/go-previous.png"));
    btnFirst->setEnabled(false);
    btnPrior->setEnabled(false);
    btnFirst->setToolTip(tr("Ir al inicio."));
    btnPrior->setToolTip(tr("Atrás..."));

    QGridLayout *lytButtonPanel = new QGridLayout;
    lytButtonPanel->setAlignment(Qt::AlignCenter);
    lytButtonPanel->addWidget(btnFirst,0,1);
    lytButtonPanel->addWidget(btnPrior,0,2);

    MasterView = new TableView;
    MasterView->setToolTip(tr("Listado de temas."));
    MasterView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    MasterView->setSortingEnabled(false);
    MasterView->setSelectionBehavior(QAbstractItemView::SelectRows);
    MasterView->setSelectionMode(QAbstractItemView::SingleSelection);
    MasterView->setShowGrid(false);
    MasterView->setAlternatingRowColors(true);
    MasterView->setWordWrap(true);
    MasterView->verticalHeader()->hide();
    MasterView->horizontalHeader()->hide();

    QVBoxLayout *lytTree = new QVBoxLayout(cntTreeView);
    lytTree->addLayout(lytButtonPanel);
    lytTree->addWidget(MasterView);
    dckTreeView->setWidget(cntTreeView);
    addDockWidget(Qt::LeftDockWidgetArea, dckTreeView);

    dckMaster = new QDockWidget(tr("Descripción del Tema"), this);
    dckMaster->setAllowedAreas(Qt::RightDockWidgetArea);

    cntMaster = new QWidget;

    previewMaster = new QWebView;
    previewMaster->setUrl(QUrl(""));

    QHBoxLayout *lytMaster = new QHBoxLayout(cntMaster);
    lytMaster->addWidget(previewMaster);

    dckMaster->setWidget(cntMaster);
    addDockWidget(Qt::RightDockWidgetArea, dckMaster);


    dckDetail = new QDockWidget(tr("Artículos  asociados"), this);
    dckDetail->setAllowedAreas(Qt::RightDockWidgetArea);

    cntDetail = new QWidget;

    DetailView = new TableView;
    DetailView->setToolTip(tr("Listado de articulos asociados al tema actual."));
    DetailView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    DetailView->setSortingEnabled(false);
    DetailView->setSelectionBehavior(QAbstractItemView::SelectRows);
    DetailView->setSelectionMode(QAbstractItemView::SingleSelection);
    DetailView->setShowGrid(false);
    DetailView->setAlternatingRowColors(true);
    DetailView->setWordWrap(true);
    DetailView->verticalHeader()->hide();
    DetailView->horizontalHeader()->hide();

    QSpacerItem *vSpacer = new QSpacerItem(1, 470, QSizePolicy::Minimum, QSizePolicy::Expanding);

    QHBoxLayout *lytDetail = new QHBoxLayout(cntDetail);
    lytDetail->addItem(vSpacer);
    lytDetail->addWidget(DetailView);
    dckDetail->setWidget(cntDetail);
    addDockWidget(Qt::RightDockWidgetArea, dckDetail);

    connect(MasterView, SIGNAL(clicked(QModelIndex)), this, SLOT(ShowDetails(QModelIndex)));
    connect(MasterView, SIGNAL(pressed(QModelIndex)), this, SLOT(ShowDetails(QModelIndex)));
    connect(MasterView, SIGNAL(activated(QModelIndex)), this, SLOT(ShowDetails(QModelIndex)));
    connect(MasterView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(MasterOnDoubleClick(QModelIndex)));
    connect(MasterView, SIGNAL(returnPressed(QModelIndex)), this, SLOT(MasterOnDoubleClick(QModelIndex)));
    connect(MasterView, SIGNAL(backspacePressed()), this, SLOT(PriorFatherLevel()));
    connect(MasterView, SIGNAL(tabPressed()), this, SLOT(setFocusDetailView()));

    connect(DetailView, SIGNAL(clicked(QModelIndex)), this, SLOT(ShowArticle(QModelIndex)));
    connect(DetailView, SIGNAL(pressed(QModelIndex)), this, SLOT(ShowArticle(QModelIndex)));
    connect(DetailView, SIGNAL(activated(QModelIndex)), this, SLOT(ShowArticle(QModelIndex)));
    connect(DetailView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(ArticleEditExec()));
    connect(DetailView, SIGNAL(returnPressed()), this, SLOT(ArticleEditExec()));
    connect(DetailView, SIGNAL(tabPressed()), this, SLOT(setFocusMasterView()));

    connect(btnFirst,SIGNAL(clicked()), this, SLOT(FirstFatherLevel()));
    connect(btnPrior,SIGNAL(clicked()), this, SLOT(PriorFatherLevel()));

    loadImage("");
}

void MainWindow::updateTitle()
{
    if (windowtitle != "") setWindowTitle(tr("%1  -  [%2]").
                                          arg(__APP_NAME__).
                                          arg(windowtitle));
    EnabledAccion(AlarifeDb->Connected());
}

void MainWindow::UpdateBookInfo()
{
    qryTemp = new QSqlQuery;
    qryTemp->exec("SELECT id_enciclopedia, nombre, plantilla, ordenar_articulo, ordenar_nivel, audio_embebido, video_embebido FROM enciclopedia");
    qryTemp->first();
    encyclopedia->setTitle(qryTemp->value(1).toString());
    encyclopedia->setTemplate(qryTemp->value(2).toString());
    encyclopedia->setIndex(qryTemp->value(0).toInt());
    encyclopedia->setMediaPath(AlarifeDb->getDirPath() + "/" + AlarifeDb->getFileName());
    encyclopedia->setArticleOrder(qryTemp->value(3).toInt());
    encyclopedia->setLevelOrder(qryTemp->value(4).toInt());
    encyclopedia->setAudioMode(qryTemp->value(5).toInt());
    encyclopedia->setVideoMode(qryTemp->value(6).toInt());


    if (qryTemp->value(1).toString() != "")
    {
        setWindowTitle(tr("%1  -  [%2]").arg(__APP_NAME__).
                       arg(encyclopedia->getTitle()));
    }

    qryTemp->exec(QString("SELECT extension FROM enc_tipo_ext WHERE enciclopedia = %1").
                  arg(encyclopedia->getIndex()));
    qryTemp->first();
    encyclopedia->setExtImagen(qryTemp->value(0).toString());
    qryTemp->next();
    encyclopedia->setExtSonido(qryTemp->value(0).toString());
    qryTemp->next();
    encyclopedia->setExtVideo(qryTemp->value(0).toString());
    qryTemp->next();
    encyclopedia->setExtXMedia(qryTemp->value(0).toString());

    delete qryTemp;
    writeRecents();

    encyclopedia->getLevelOrder() == 1 ? actChapterByName->setChecked(true) : actChapterByName->setChecked(false);
    encyclopedia->getArticleOrder() == 1 ? actArticleByName->setChecked(true) : actArticleByName->setChecked(false);
}

void MainWindow::ClearBookInfo()
{
    encyclopedia->setTitle("");
    encyclopedia->setTemplate("");
    encyclopedia->setIndex(0);
    encyclopedia->setMediaPath("");
    encyclopedia->setExtImagen("");
    encyclopedia->setExtSonido("");
    encyclopedia->setExtVideo("");
    encyclopedia->setExtXMedia("");

    encyclopedia->setArticleOrder(1);
    encyclopedia->setLevelOrder(0);
    encyclopedia->setAudioMode(0);
    encyclopedia->setVideoMode(0);

    actChapterByName->setChecked(false);
    actArticleByName->setChecked(false);
}

void MainWindow::AdjustHeader()
{
    MasterView->resizeRowsToContents();
    MasterView->setColumnWidth(0,MasterView->width());
    MasterView->hideColumn(1);
    MasterView->hideColumn(2);
    MasterView->hideColumn(3);
    MasterView->hideColumn(4);
    MasterView->hideColumn(5);

    DetailView->resizeRowsToContents();
    DetailView->setColumnWidth(0,DetailView->width());
    DetailView->hideColumn(1);
    DetailView->hideColumn(2);
    DetailView->hideColumn(3);
    DetailView->hideColumn(4);
}

void MainWindow::FirstFatherLevel()
{
    while (stack->getNumber() != 0) stack->PopId();
    id_master = 0;
    refreshGrids(id_master);
    trace.clear();
    lblTrace->clear();
    dckDetail->setWindowTitle(tr("Artículos del tema."));
}

void MainWindow::PriorFatherLevel(){
    int i = stack->getFather();
    refreshGrids(i);

    stack->PopId();

    QString str = lblTrace->text();

    if (!trace.isEmpty()&& stack->getNumber() != 0)
    {
        id_master = stack->getFather();
        str.truncate(str.length() - trace.pop().length() - 3);
        lblTrace->setText(str);
        UpdatePanelCaption(i);
    } else {
        lblTrace->clear();
        id_master = 0;
        UpdatePanelCaption();
    }
}

void MainWindow::MasterOnDoubleClick(QModelIndex index)
{
    if(!btnFirst->isEnabled())
    {
        btnFirst->setEnabled(true);
        btnPrior->setEnabled(true);
    }

    QSqlRecord record = master->record(index.row());
    id_master = record.value("id_nivel").toInt();

    stack->PushId(record.value("padre").toInt(),
                  record.value("id_nivel").toInt());

    trace.push(record.value("nombre").toString());

    if (lblTrace->text().isEmpty()){
        lblTrace->setText(tr("%1 ").arg(record.value("nombre").toString()));
    }else{
        QString str = lblTrace->text();
        lblTrace->setText(tr("%1 » %2").
                          arg(str).
                          arg(record.value("nombre").toString()));
    }

    OpenArticleList(id_master,"");
    OpenRelationTable(id_master, "");
    AdjustHeader();
    UpdatePanelCaption(id_master);
    ShowDetails(index);
}

void MainWindow::ShowDetails(QModelIndex index)
{
    MasterId = index;

    QSqlRecord record = master->record(MasterId.row());

    QString swap = record.value("resumen").toString();
    if(swap.contains("<body")) swap = selectUndefineArea(swap,"body");

    QString path = QString(__HTTP__ + __APP_PATH__);
    if (path.contains("/bin")) path.remove("/bin", Qt::CaseSensitive);

    if (swap.contains("../imagenes/audio.jpg", Qt::CaseSensitive))
        swap.replace("../imagenes/audio.jpg",
                     QString("%1/%2").arg(path).arg("qjseditor/images/audio.jpg"),
                     Qt::CaseSensitive);
    if (swap.contains("../imagenes/video.jpg", Qt::CaseSensitive))
        swap.replace("../imagenes/video.jpg",
                     QString("%1/%2").arg(path).arg("qjseditor/images/video.jpg"),
                     Qt::CaseSensitive);
    if (swap.contains("../imagenes/xmedias.jpg", Qt::CaseSensitive))
        swap.replace("../imagenes/xmedias.jpg",
                     QString("%1/%2").arg(path).arg("qjseditor/images/xmedias.jpg"),
                     Qt::CaseSensitive);

    swap.replace("../medias/", __HTTP__ + encyclopedia->getMediaPath() + "/", Qt::CaseSensitive);

    QString html = QString("<html><head><meta content=\"text/html; "
                           "charset=utf-8\" http-equiv=\"content-type\" />"
                           "</head><body>%1</body></html>").arg(swap);

    previewMaster->setHtml(html);
    previewMaster->show();

    current_master = record.value("id_nivel").toInt();

    AdjustHeader();

    EnabledArticlesEdit(false);
    if (sqlAutoCountRow("nivel", "") > 0) EnabledLevelsEdit(true);
    actArticleAdd->setEnabled(true);
}

void MainWindow::ShowArticle(QModelIndex index)
{
    DetailId = index;

    QSqlRecord record = detail->record(DetailId.row());

    current_detail =record.value("id_articulo").toInt();
    EnabledArticlesEdit(true);
    EnabledLevelsEdit(false);
}

void MainWindow::OpenArticleList(int father, QString find)
{
    StrSql.clear();

    StrSql = QString("SELECT DISTINCT articulo.nombre || '\n' || articulo.resumen || '\n' "
                     "AS Articulo, articulo.id_articulo, articulo.nombre, usuario, nivel FROM articulo ");

    if(find.isEmpty())
    {
        QString order;
        encyclopedia->getArticleOrder() == 0 ? order = "orden" : order = "nombre";
        StrSql.append("WHERE articulo.nivel = %1 ORDER BY articulo.%2");
        detail->setQuery(StrSql.arg(father).arg(order));
    }else{
        StrSql.append(find);

        detail->setQuery(StrSql);
        if(detail->query().first()==false)
            edtSearch->setStyleSheet(tr("background-color: rgb(255, 140, 140);"));
    }
    qDebug() << "buscar: " << detail->query().lastQuery() << endl;
    DetailView->setModel(detail);
    DetailView->show();
}

void MainWindow::OpenRelationTable(int father, QString find)
{
    StrSql.clear();

    StrSql = QString("SELECT DISTINCT nombre, id_nivel, resumen, padre, usuario FROM nivel "
                     "WHERE enciclopedia = '%1' ");

    if(find.isEmpty())
    {
        QString order;
        encyclopedia->getLevelOrder() == 0 ? order = "orden" : order = "nombre";
        StrSql.append("AND padre = '%2' ORDER BY %3");
        master->setQuery(StrSql.arg(encyclopedia->getIndex()).arg(father).arg(order));
    }else{
        StrSql.append("AND %2");
        master->setQuery(StrSql.arg(encyclopedia->getIndex()).arg(find));
        if(master->query().first()==false)
            edtSearch->setStyleSheet(tr("background-color: rgb(255, 140, 140);"));
    }

    MasterView->setModel(master);
    MasterView->show();
    MasterView->selectRow(0);

    ShowDetails(MasterView->indexAt(QPoint(0,0)));

    AdjustHeader();
}

void MainWindow::openConection(bool enabled)
{
    closeTraceBar();
    if (enabled == true)
    {
        UpdateBookInfo();
        EnabledAccion(enabled);
        id_master = 0;
        refreshGrids(id_master);
        AdjustHeader();
        openTraceBar();
        UpdatePanelCaption();
    }else if (enabled == false){
        AlarifeDb->Active(enabled);
        OpenRelationTable(-2, "");
        OpenArticleList(-2, "");
        setWindowTitle(tr(__APP_NAME__));
        ClearBookInfo();
    }

}

void MainWindow::refreshGrids(int father)
{
    OpenRelationTable(father);
    OpenArticleList(father);
    AdjustHeader();    
}

void MainWindow::setFocusMasterView()
{
    MasterView->setFocus();
}

void MainWindow::setFocusDetailView()
{
    DetailView->setFocus();
}

void MainWindow::refreshTraceBar(int id)
{
    lblTrace->clear();
    lstLevel.clear();
    lstName.clear();
    strTrace.clear();

    while (!trace.isEmpty()) trace.pop();
    while (stack->getNumber() != 0) stack->PopId();

    if (id != 0)
    {
        PushTraceBar(id);

        for (int i = 0; i < lstLevel.count(); i++)
        {
            Reg reg = lstLevel.at(i);
            stack->PushId(reg.father, reg.number);
        }

        for (int j = 0; j < lstName.count(); j++)
        {
            trace.push(lstName.at(j));
            if (j == 0)
                strTrace.append(lstName.at(j));
            else
                strTrace.append(tr(" » %1").arg(lstName.at(j)));
        }

        lblTrace->setText(strTrace);
        UpdatePanelCaption(id);
    }
    UpdatePanelCaption();
}

void MainWindow::EnabledAccion(bool actived)
{
    actStadistic->setEnabled(actived);
    actHomePage->setEnabled(actived);
    actProperty->setEnabled(actived);
    actAuthor->setEnabled(actived);
    actMedia->setEnabled(actived);
    actConfiguration->setEnabled(actived);

    actArticleAdd->setEnabled(actived);
    actChapterAdd->setEnabled(actived);
    actBookExport->setEnabled(actived);
    actAddBookImport->setEnabled(actived);
    actMergeBookImport->setEnabled(actived);
    actGenerated->setEnabled(actived);
    actBenchmark->setEnabled(actived);
    actMoveElement->setEnabled(actived);

    btnFirst->setEnabled(actived);
    btnPrior->setEnabled(actived);
    actSearch->setEnabled(actived);

    MasterView->setEnabled(actived);
    DetailView->setEnabled(actived);

    actAdminUser->setEnabled(actived);
    actAdminTemplate->setEnabled(actived);
    actAdminStyles->setEnabled(actived);
}

void MainWindow::EnabledLevelsEdit(bool actived)
{
    actChapterEdit->setEnabled(actived);
    actChapterDelete->setEnabled(actived);
    actChapterOrder->setEnabled(actived);
    actChapterByName->setEnabled(actived);
}

void MainWindow::EnabledArticlesEdit(bool actived)
{
    actArticleEdit->setEnabled(actived);
    actArticleDelete->setEnabled(actived);
    actArticleProperty->setEnabled(actived);
    actArticleOrder->setEnabled(actived);
    actArticleByName->setEnabled(actived);
}

void MainWindow::NewExec()
{
    if(AlarifeDb->getPathFile() != "" && AlarifeDb->Connected() == true)
    {
        int btn = QMessageBox::question(this, tr("Abrir enciclopedia"),
                                        tr("Actualmente esta en edición la enciclopedia \"%1\". "
                                           "¿Desea cerrarla y crear una nueva?")
                                        .arg(encyclopedia->getTitle()), QMessageBox::Yes | QMessageBox::No);
        if (btn == QMessageBox::Yes)
        {
            OpenRelationTable(-2,"");
            OpenArticleList(-2,"");
            EnabledAccion(false);
            EnabledArticlesEdit(false);
            EnabledLevelsEdit(false);
            AlarifeDb->Active(false);
            setWindowTitle(tr(__APP_NAME__));
        }else return;
    }

    NewBook *wizard = new NewBook(this, &AlarifeDb, &encyclopedia);
    wizard->show();

    connect(wizard,SIGNAL(closeDlg(bool)),this, SLOT(openConection(bool)));
}

void MainWindow::ShowLoginDlg()
{
    loginDlg *login = new loginDlg(this, &encyclopedia);
    login->show();
    connect(login,SIGNAL(closeDlg(bool)),this, SLOT(openConection(bool)));

}

void MainWindow::OpenExec()
{
    if(AlarifeDb->getPathFile() != "" && AlarifeDb->Connected() == true)
    {
        int btn = QMessageBox::question(this, tr("Abrir enciclopedia"),
                                        tr("Actualmente esta en edición la enciclopedia "
                                           "\"%1\". ¿Desea cerrarla y abrir otra?")
                                        .arg(encyclopedia->getTitle()), QMessageBox::Yes | QMessageBox::No);
        if (btn == QMessageBox::Yes)
        {
            OpenRelationTable(-2,"");
            OpenArticleList(-2,"");
            EnabledAccion(false);
            EnabledArticlesEdit(false);
            EnabledLevelsEdit(false);
            AlarifeDb->Active(false);
            setWindowTitle(tr(__APP_NAME__));
        }else return;
    }
    QString fn = QFileDialog::getOpenFileName(this, tr("Abrir enciclopedia..."),
                                              QDir::homePath()+"/Alarife/",
                                              tr("Base de datos Alarife (*.alf);;Todos los archivos (*)"));
    if (!fn.isEmpty())
    {
        AlarifeDb->ConnectionString(fn);
        AlarifeDb->Active(true);
        if (AlarifeDb->Connected() == true)
        {
            if (!AlarifeDb->ChechkCompatibility())
            {
                QMessageBox::warning(0, qApp->tr("Comprobando compatibilidad"),
                                     qApp->tr("La enciclopedia seleccionada no es totalmente compatible con esta versión de Alarife.\n"
                                              "Por favor consulte el manual de ayuda para más detalles"), QMessageBox::Ok);
            }
            ShowLoginDlg();
        }
    }
}

void MainWindow::StadisticExec()
{
    Stadistic *stadistic = new Stadistic(this, encyclopedia);
    stadistic->show();
}

void MainWindow::HomePageExec()
{
    HomePage *home = new HomePage(this, &encyclopedia);
    connect(home, SIGNAL(updateEncInfo()), this, SLOT(UpdateBookInfo()));
    home->show();
}

void MainWindow::PropertyExec()
{
    PropertyDlg *property = new PropertyDlg(this, &encyclopedia);
    property->title = &windowtitle;
    property->show();
}

void MainWindow::AuthorsExec()
{
    AuthorOption *author = new AuthorOption(this, &encyclopedia);
    author->exec();
}

void MainWindow::MediaExec()
{
    MediaOption *media = new MediaOption(this, encyclopedia);

    QVBoxLayout *lytMedia = new QVBoxLayout;
    lytMedia->addWidget(media);

    QDialog *dlgMedia = new QDialog(this);
    dlgMedia->setWindowTitle(tr("Medias de la enciclopedia"));
    dlgMedia->setLayout(lytMedia);
    dlgMedia->setMinimumSize(680, 500);
    dlgMedia->setMaximumSize(680, 500);
    dlgMedia->show();
}

void MainWindow::ToolBarSearchExec()
{
    if(edtSearch->text() != "")
    {
        QString strFind;
        if(rbtMaster->isChecked() == true)
        {
            sqlBuildLevelQuery(edtSearch->text(), strFind);
            strFind.replace("% ", "%", Qt::CaseSensitive);
            strFind.replace(" %", "%", Qt::CaseSensitive);
            OpenRelationTable(0, strFind);
            OpenArticleList(-2,"");
        }else if (rbtDetail->isChecked() == true){
            sqlBuildArticleQuery(edtSearch->text(), strFind);
            strFind.replace("% ", "%", Qt::CaseSensitive);
            strFind.replace(" %", "%", Qt::CaseSensitive);
            OpenRelationTable(-2,"");
            qDebug() << "buscar: " << strFind << endl;
            OpenArticleList(0, QString(" WHERE %1 ORDER BY id_articulo").arg(strFind));
        }else if(rbtAuthor->isChecked() == true){
            OpenRelationTable(-2,"");
            OpenArticleList(0, QString(" INNER JOIN articuloautores ON articuloautores.articulo = articulo.id_articulo"
                                       " INNER JOIN  autor ON autor.id_autor = articuloautores.autor"
                                       " WHERE autor.nombre LIKE '%%1%' ORDER BY id_articulo").arg(edtSearch->text()));
        }
    }
}

void MainWindow::ClearSearch()
{
    edtSearch->clear();
}

void MainWindow::EmptySearch()
{
    if (edtSearch->text() == ""){
        edtSearch->setStyleSheet(tr("background-color: rgb(255, 255, 255);"));
        refreshGrids(0);
    } else
        edtSearch->setStyleSheet(tr("background-color: rgb(255, 255, 255);"));
}

void MainWindow::ToolBarSearchShow()
{
    if(!actSearch->isChecked())
    {
        ToolBarSearchHide();
        return;
    }
    edtSearch = new QLineEdit;
    edtSearch->setStyleSheet(tr("background-color: rgb(255, 255, 255);"));
    edtSearch->clear();
    edtSearch->setMaximumWidth(205);

    btnSearch  = new QPushButton(tr("Buscar"));

    rbtMaster = new QRadioButton(tr("Buscar en temas"));
    rbtDetail = new QRadioButton(tr("Buscar en artículos"));
    rbtAuthor = new QRadioButton(tr("Buscar por autores relacionados"));

    rbtMaster->setChecked(true);

    QSpacerItem *hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding,
                                           QSizePolicy::Minimum);
    QWidget *widget = new QWidget;

    btnClear = new QToolButton;
    btnClear->setIcon(QIcon(":/images/edit-clear.png"));
    btnClear->setIconSize(QSize(22,22));
    btnClear->setShortcut(Qt::Key_Escape);
    btnClear->setToolTip(tr("Limpiar la búsqueda."));

    QHBoxLayout *lytFind = new QHBoxLayout(widget);
    lytFind->setContentsMargins(5, 5, 0, 5);
    lytFind->addWidget(btnClear);
    lytFind->addWidget(edtSearch);
    lytFind->addWidget(btnSearch);
    lytFind->addWidget(rbtMaster);
    lytFind->addWidget(rbtDetail);
    lytFind->addWidget(rbtAuthor);
    lytFind->addItem(hSpacer);

    tlbFind = new QToolBar;
    tlbFind = addToolBar(tr("Buscar"));
    tlbFind->addWidget(widget);
    tlbFind->setFloatable(false);
    tlbFind->setMovable(false);
    addToolBar(Qt::BottomToolBarArea,tlbFind);

    edtSearch->setFocus(Qt::OtherFocusReason);

    connect(btnSearch, SIGNAL(clicked()), this, SLOT(ToolBarSearchExec()));
    connect(btnClear, SIGNAL(clicked()), this, SLOT(ClearSearch()));
    connect(edtSearch,SIGNAL(textChanged(QString)),this, SLOT(EmptySearch()));
    connect(edtSearch, SIGNAL(returnPressed()), this, SLOT(ToolBarSearchExec()));
    connect(rbtAuthor, SIGNAL(toggled(bool)), this, SLOT(resetSearch(bool)));
    connect(rbtDetail, SIGNAL(toggled(bool)), this, SLOT(resetSearch(bool)));
    connect(rbtMaster, SIGNAL(toggled(bool)), this, SLOT(resetSearch(bool)));
}

void MainWindow::ToolBarSearchHide()
{
    delete tlbFind;
    OpenRelationTable(0,"");
}

void MainWindow::resetSearch(bool push)
{
    if (push == true)
    {
        edtSearch->setStyleSheet(tr("background-color: rgb(255, 255, 255);"));
        refreshGrids(0);
    }
}

void MainWindow::ChapterAddExec()
{
    int id_nivel;

    sqlAutoCountRow("nivel","") > 0 ? id_nivel = sqlAutoIncremental("id_nivel","nivel") : id_nivel = 1;

    int id_father = stack->getNumber();

    Chapter *chapter = new Chapter(this, id_father, id_nivel, 0, &encyclopedia);
    chapter->show();
    connect(chapter,SIGNAL(updateGrid(int)),this, SLOT(refreshGrids(int)));
}

void MainWindow::ChapterEditExec()
{
    if(MasterId.isValid())
    {
        QSqlRecord record = master->record(MasterId.row());
        QString reg(record.value("nombre").toString());
        if (isOwner(record))
        {
            Chapter *chapter = new Chapter(this, record.value("padre").toInt(), record.value("id_nivel").toInt(), 2, &encyclopedia);
            chapter->show();
            connect(chapter,SIGNAL(updateGrid(int)),this, SLOT(refreshGrids(int)));
        }else
            QMessageBox::critical(this,
                                  tr("Editar tema"),
                                  tr("El tema \"%1\" no se puede editar porque pertenece a otro Editor.").arg(reg));
    }
}

void MainWindow::ChapterDeleteExec()
{
    QSqlRecord record = master->record(MasterId.row());
    QString reg(record.value("nombre").toString());

    if (isOwner(record))
    {
        if (sqlAutoCountRow("nivel", QString("padre = %1").arg(current_master)) != 0)
        {
            int delAll = QMessageBox::warning(this, tr("Confirmar eliminación"),
                                              tr("El tema \"%1\", tiene subtemas. \n\nSUGERENCIAS\n"
                                                 "- Para eliminar los subtemas clic en \"SI\".\n"
                                                 "- Para asignarlos a otro tema clic en \"NO\".").arg(reg),
                                              QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            switch (delAll)
            {
            case QMessageBox::Yes :
                sqlDeleteRecord_N("nivel", QString("id_nivel = %1").arg(current_master));
                wait(1000);
                RestructuringData();
                break;
            case QMessageBox::No :
                changeChapterFather(record.value("id_nivel").toInt());
                break;
            case QMessageBox::Cancel :
                break;
            }
        }else
            if (!sqlDeleteRecord_N("nivel", QString("id_nivel = %1").arg(current_master)))
            {
                QMessageBox::information(this, tr("Eliminación Fallida"),
                                         tr("El tema \"%1\" no se ha eliminado \nde la "
                                            "enciclopedia.").arg(reg));
            }else{
                QMessageBox::information(this, tr("Eliminación Satisfactoria"),
                                         tr("El tema \"%1\" se ha eliminado \nde la "
                                            "enciclopedia.").arg(reg));
                refreshGrids(id_master);
            }
    }else{
        QMessageBox::critical(this, tr("Eliminación de tema"),
                              tr("El tema \"%1\" no se ha eliminado de la Enciclopedia "
                                 "porque pertenecen a otro Editor.").arg(reg));
    }
}

void MainWindow::ChapterDeleteAll()
{
    QSqlRecord record = master->record(MasterId.row());
    int user = record.value("usuario").toInt();
    int padre  = record.value("padre").toInt();
    if (encyclopedia->getUserIndex() == user || encyclopedia->getUserIndex() == 1)
    {
        if (sqlAutoCountRow("nivel", QString("padre = %1").arg(padre)) != 0)
        {
            sqlDeleteRecord_N("nivel", QString("padre = %1").arg(padre));
            refreshGrids(id_master);
            RestructuringData();
        }
    }else{
        QMessageBox::critical(this, tr("Eliminación de tema"),
                              tr("Los temas no se han eliminado de la Enciclopedia "
                                 "porque pertenecen a otro Editor."));
    }
}

void MainWindow::ChapterOrderExec()
{
    QSqlRecord record = master->record(MasterId.row());
    int id = record.value("padre").toInt();
    OrdersDlg *order = new OrdersDlg(this, id, encyclopedia->getUserIndex(), 0);
    order->show();
    connect(order,SIGNAL(updateGrid(int)),this, SLOT(refreshGrids(int)));

}

void MainWindow::ChapterOrder(bool alpha)
{
    sqlUpdateRecord_N("enciclopedia", QString("ordenar_nivel = %1").arg(alpha));
    encyclopedia->setLevelOrder(alpha);

    OpenArticleList(id_master,"");
    OpenRelationTable(id_master, "");
    AdjustHeader();
}

void MainWindow::ChapterDeleteMsg(bool supr)
{
    QSqlRecord record = master->record(MasterId.row());
    QString reg(record.value("nombre").toString());

    if (supr == false)
    {
        QMessageBox::information(this, tr("Eliminación Fallida"),
                                 tr("El tema \"%1\" no se ha eliminado \nde la "
                                    "enciclopedia.").arg(reg));
    }else{
        QMessageBox::information(this, tr("Eliminación Satisfactoria"),
                                 tr("El tema \"%1\" se ha eliminado \nde la "
                                    "enciclopedia.").arg(reg));
        OpenRelationTable(stack->getFather(),"");
    }
}

void MainWindow::changeChapterFather(int index)
{
    MoveChapterDlg *movedlg = new MoveChapterDlg(this, 1, index);
    movedlg->show();
    connect(movedlg, SIGNAL(moveFatherLevel(int)), this, SLOT(refreshGrids(int)));
    connect(movedlg, SIGNAL(moveFatherLevel(int)), this, SLOT(refreshTraceBar(int)));
}

void MainWindow::ChapterMoveExec()
{
    if (sqlAutoCountRow("nivel", "padre = 0") > 1){
        MoveChapterDlg *movedlg = new MoveChapterDlg(this, 0, current_master);
        movedlg->show();
        connect(movedlg, SIGNAL(moveFatherLevel(int)), this, SLOT(refreshGrids(int)));
        connect(movedlg, SIGNAL(moveFatherLevel(int)), this, SLOT(refreshTraceBar(int)));
    }else
        QMessageBox::critical(this, tr("Mover temas"),
                              tr("Error. Imposible mover un tema.cuando existe solo un tema inical"));
}

void MainWindow::ChapterMoveExecAll()
{
    if (id_master == 0)
        QMessageBox::critical(this, tr("Mover temas"),
                              tr("Error. Imposible mover todos los temas.inicales"));
    else{
        MoveChapterDlg *movedlg = new MoveChapterDlg(this, 1, id_master);
        movedlg->show();
        connect(movedlg, SIGNAL(moveFatherLevel(int)), this, SLOT(refreshGrids(int)));
        connect(movedlg, SIGNAL(moveFatherLevel(int)), this, SLOT(refreshTraceBar(int)));
    }

}

void MainWindow::ArticleAddExec()
{qDebug() << "id_master: " << id_master << endl;
    ItemId ii;
    ii.setEnciclopedia(encyclopedia->getIndex());
    ii.setFather(id_master);
    ii.setItem(0);
    ii.setUser(encyclopedia->getUserIndex());
    ii.setPath(encyclopedia->getMediaPath());
    ii.setMode(ItemId::INSERT);

    ItemProperty *itpNewDoc = new ItemProperty(ii,this);
    connect(itpNewDoc, SIGNAL(inserted(ItemId)), this, SLOT(showEditor(ItemId)));
    itpNewDoc->show();
}

void MainWindow::ArticleEditExec()
{
    QSqlRecord record = detail->record(DetailId.row());
    ItemId ii;
    ii.setEnciclopedia(encyclopedia->getIndex());
    ii.setFather(id_master);
    ii.setItem(record.value("id_articulo").toInt());
    ii.setUser(encyclopedia->getUserIndex());
    ii.setPath(encyclopedia->getMediaPath());
    ii.setMode(ItemId::EDIT);
    ii.setOwner(isOwner(record));

    Editor *edit = new Editor(this, &encyclopedia);
    connect(edit,SIGNAL(updateGridDetail(int)),this,SLOT(refreshGrids(int)));
    edit->setItemId(ii);
    edit->showMaximized();
}

void MainWindow::ArticleDeleteExec()
{

    QSqlRecord record = detail->record(DetailId.row());
    QString reg(record.value("nombre").toString());

    if (isOwner(record))
    {
        if (!sqlDeleteRecord_N("articulo",
                               QString("id_articulo = %1").arg(current_detail)))
        {
            QMessageBox::information(this, tr("Eliminación Fallida"),
                                     tr("El artículo \"%1\" no se ha eliminado de la "
                                        "enciclopedia.").arg(reg));
        }else{
            QMessageBox::information(this, tr("Eliminación Satisfactoria"),
                                     tr("El artículo \"%1\" se ha eliminado de la "
                                        "enciclopedia.").arg(reg));
        }
        OpenArticleList(id_master);
        AdjustHeader();
    }else{
        QMessageBox::information(this, tr("Eliminación de artículo"),
                                 tr("El artículo \"%1\" no se ha eliminado de la Enciclopedia "
                                    "porque pertenecen a otro Editor.").arg(reg));
    }
}

void MainWindow::ArticleDeleteAll()
{
    QSqlRecord record = detail->record(DetailId.row());
    int user = record.value("usuario").toInt();
    int nivel = record.value("nivel").toInt();
    if (encyclopedia->getUserIndex() == user || encyclopedia->getUserIndex() == 1)
    {
        if (sqlAutoCountRow("articulo", QString("nivel = %1").arg(nivel)) != 0)
        {
            sqlDeleteRecord_N("articulo", QString("nivel = %1").arg(nivel));
            OpenArticleList(id_master);
            RestructuringData();
        }
    }else{
        QMessageBox::information(this, tr("Eliminación de artículo"),
                                 tr("Los artículos no se han eliminado de la Enciclopedia "
                                    "porque pertenecen a otro Editor."));
    }
}

void MainWindow::ActArticlePropertyExec()
{
    QSqlRecord record = detail->record(DetailId.row());
    ItemId tmp;
    tmp.setEnciclopedia(encyclopedia->getIndex());
    tmp.setFather(record.value("id_nivel").toInt());
    tmp.setItem(record.value("id_articulo").toInt());
    tmp.setUser(encyclopedia->getUserIndex());
    tmp.setPath(encyclopedia->getMediaPath());
    tmp.setMode(ItemId::READONLY);

    ItemProperty *itp = new ItemProperty(tmp,this);
    itp->show();
}

void MainWindow::ArticleOrderExec()
{
    QSqlRecord record = detail->record(DetailId.row());
    int id = record.value("nivel").toInt();
    OrdersDlg *order = new OrdersDlg(this, id, encyclopedia->getUserIndex(), 1);
    order->show();
    connect(order,SIGNAL(updateGrid(int)),this, SLOT(refreshGrids(int)));
}

void MainWindow::ArticleMoveExec()
{
    if(DetailId.isValid())
    {
        QSqlRecord record = detail->record(DetailId.row());
        MoveArticleDlg *movedlg = new MoveArticleDlg(this, 0, record.value("id_articulo").toInt());
        connect(movedlg, SIGNAL(updateGrid(int)), this, SLOT(refreshGrids(int)));
        connect(movedlg, SIGNAL(updateGrid(int)), this, SLOT(refreshTraceBar(int)));
        movedlg->show();
    }
}

void MainWindow::ArticleMoveAllExec()
{
    if(DetailId.isValid())
    {
        QSqlRecord record = detail->record(DetailId.row());
        MoveArticleDlg *movealldlg = new MoveArticleDlg(this, 1, record.value("id_articulo").toInt());
        connect(movealldlg, SIGNAL(updateGrid(int)), this, SLOT(refreshGrids(int)));
        connect(movealldlg, SIGNAL(updateGrid(int)), this, SLOT(refreshTraceBar(int)));
        movealldlg->show();
    }
}

void MainWindow::ArticleOrder(bool alpha)
{
    sqlUpdateRecord_N("enciclopedia", QString("ordenar_articulo = %1").arg(alpha));
    encyclopedia->setArticleOrder(alpha);

    OpenArticleList(id_master,"");
    OpenRelationTable(id_master, "");
    AdjustHeader();
}

void MainWindow::showEditor(ItemId item)
{
    Editor *edit = new Editor(this, &encyclopedia);
    connect(edit,SIGNAL(updateGridDetail(int)),this,SLOT(refreshGrids(int)));
    connect(edit, SIGNAL(closeEditor()), this, SLOT(refreshGrids(int)));
    edit->setItemId(item);
    edit->showMaximized();
}

void MainWindow::MoveElementExec()
{
    MoveElement *dlgMove = new MoveElement(this);
    dlgMove->show();
}

void MainWindow::BookExportExec()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks |
            QFileDialog::ShowDirsOnly;

    QString directory = QFileDialog::getExistingDirectory(this,
                                                          tr("Generar enciclopedia"),
                                                          QDir::home().absolutePath(),
                                                          options);
    if (!directory.isEmpty())
    {
        DirPath = directory;
        ProgressPanelActive(true);

        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(Exec_BookExport()));
        timer->start(1500);
    }
}

void MainWindow::Exec_BookExport()
{
    timer->stop();

    QString path = __SYS_DIR__;
    if (!path.contains("/bin")) path = __SYS_DIR__ + "bin/";

    if (QFile::exists(path + "7z" + __EXT__))
    {
        if (!QFileInfo(DirPath).isWritable()){
            QMessageBox::critical(this, tr("Exportar enciclopedia"),
                                  tr("La enciclopedia \"%1\" no se puede exportar "
                                     "en el directorio seleccionado; permiso denegado.").arg(encyclopedia->getTitle()));
        }else{
            isFull = true;

            wait(1000);

            dbExport *expdb = new dbExport(this, encyclopedia);
            UpdateProgressPanel(2, tr("Creando directorios..."));
            expdb->createDir(DirPath);
            UpdateProgressPanel(5, tr("Exportando datos de la enciclopedia..."));
            expdb->exportEncyclopedia();
            UpdateProgressPanel(15, tr("Exportando relaciones entre artículos..."));
            expdb->exportRelationArticles();
            UpdateProgressPanel(20, tr("Exportando colaboradores y equipo..."));
            expdb->exportTeamColaborator();
            UpdateProgressPanel(25, tr("Exportando medias..."));
            expdb->exportMedias();
            UpdateProgressPanel(40, tr("Exportando niveles..."));
            expdb->exportLevels();
            UpdateProgressPanel(65, tr("Exportando artículos..."));
            expdb->exportArticles();
            UpdateProgressPanel(90, tr("Exportando autores..."));
            expdb->exportAuthor();

            UpdateProgressPanel(92, tr("Exportando Slideshow..."));
            expdb->exportSlideShow();
            UpdateProgressPanel(93, tr("Exportando galeria..."));
            expdb->exportGallery();
            UpdateProgressPanel(94, tr("Exportando referencias..."));
            expdb->exportReference();

            UpdateProgressPanel(95, tr("Comprimiendo ficheros..."));
            expdb->compressEncyclopedia();
            UpdateProgressPanel(98, tr("Eliminando temporales..."));
            expdb->cleanTemporalFiles();

            UpdateProgressPanel(100, tr("Proceso finalizado..."));

            QMessageBox::information(this, tr("Exportar enciclopedia"),
                                     tr("La enciclopedia \"%1\" se ha exportado "
                                        "correctamente.").arg(encyclopedia->getTitle()));
            isFull = false;
        }

        ProgressPanelActive(false);
    }else
        QMessageBox::critical(this, tr("Exportar enciclopedia"),
                              tr("Operación cancelada, falta el software, "
                                 "7Z, necesario para la exportación."));
}

void MainWindow::ProgressPanelActive(bool active)
{
    if (active == true)
    {
        prgBar = new QProgressBar;
        prgBar->setMaximumHeight(16);
        prgBar->setMaximumWidth(200);
        prgBar->setValue(0);

        prgLbl = new QLabel;
        prgLbl->setMinimumWidth(400);

        statusBar()->insertWidget(0,prgBar,0);
        statusBar()->insertWidget(1,prgLbl,0);
    }else if(active == false){
        delete prgBar;
        delete prgLbl;

    }
}

void MainWindow::UpdateProgressPanel(int value, QString text)
{
    prgBar->setValue(value);
    prgLbl->setText(text);
    wait(2000);
}

void MainWindow::UpdatePanelCaption(int row)
{
    QString caption = "";
    if (row > 0)
        caption.append(tr("al tema «%1»").
                       arg(sqlGetTextValue("nivel",
                                           "nombre",
                                           QString("id_nivel = %1").arg(row))));

    int i = sqlAutoCountRow("articulo", QString("nivel = %1").arg(row));

    if (i > 0) caption.append(tr("    Total de artículos: %1").arg(i));

    dckDetail->setWindowTitle(tr("Artículos asociados %1").arg(caption));
}

void MainWindow::NewBookImportExec()
{
    if(AlarifeDb->getPathFile() != "" && AlarifeDb->Connected() == true)
    {
        int btn = QMessageBox::question(this, tr("Importar enciclopedia"),
                                        tr("Actualmente esta en edición la enciclopedia \"%1\". "
                                           "¿Desea cerrarla e importar una nueva?")
                                        .arg(encyclopedia->getTitle()), QMessageBox::Yes | QMessageBox::No);
        if (btn == QMessageBox::Yes)
        {
            OpenRelationTable(-2,"");
            OpenArticleList(-2,"");
            EnabledAccion(false);
            EnabledArticlesEdit(false);
            EnabledLevelsEdit(false);
            AlarifeDb->Active(false);
            setWindowTitle(tr(__APP_NAME__));
            NewBookImportPrepare();
        }
    }else NewBookImportPrepare();
}

void MainWindow::NewBookImportPrepare()
{
    curFile = QFileDialog::getOpenFileName(this, tr("Importar enciclopedia..."),
                                           QDir::homePath(),
                                           tr("Datos exportados de Alarife (*.alfe)"));
    if (!curFile.isEmpty())
    {
        bool ok;
        DirPath = QInputDialog::getText(this, tr("Importar enciclopedia"),
                                        tr("Nombre de la Base datos:                    "),
                                        QLineEdit::Normal,"", &ok);
        
        while (QFile::exists(QString("%1/Alarife/%2.alf").arg(__DIR_FILES__).arg(DirPath)))
        {
            DirPath = QInputDialog::getText(this, tr("Importar enciclopedia"),
                                            tr("Existe una base datos con igual nombre, \n"
                                               "escriba otro:                    "),
                                            QLineEdit::Normal,"", &ok);
        }
        
        if (ok && !DirPath.isEmpty() && !curFile.isEmpty())
        {
            ProgressPanelActive(true);
            timer = new QTimer(this);
            connect(timer, SIGNAL(timeout()), this, SLOT(Exec_NewBookImport()));
            timer->start(1000);
        }else
            QMessageBox::critical(this, tr("Importar enciclopedia"),
                                  tr("Operación cancelada por el usuario."));
    }else
        QMessageBox::critical(this, tr("Importar enciclopedia"),
                              tr("Operación cancelada por el usuario."));
}

void MainWindow::Exec_NewBookImport()
{
    timer->stop();

    QString path = __SYS_DIR__;
    if (!path.contains("/bin")) path = __SYS_DIR__ + "bin/";

    if (QFile::exists(path + "7z" + __EXT__)){

        wait(7000);

        if (!QDir(QString("%1/Alarife").arg(QDir::homePath())).exists())
            QDir(QDir::homePath()).mkdir("Alarife");

        AlarifeDb->ConnectionString(QString("%1/Alarife/%2.alf").arg(__DIR_FILES__).arg(DirPath));
        AlarifeDb->Active(true);
        AlarifeDb->CreateDb();

        isFull = true;

        UpdateProgressPanel(5, tr("Extrayendo datos..."));

        wait(2000);

        impdb = new dbImport(this, curFile);
        impdb->setMediaDir(DirPath);
        impdb->uncompress();

        wait(2000);
        if (!QFile::exists(__DIR_CACHE__ + "/orden.nl.alfe"))
        {
            QMessageBox::critical(this, tr("Importar enciclopedia"),
                                  tr("Operación cancelada, el fichero de "
                                     "importación contiene errores."));
            impdb->cleanTemporalFiles();
            isFull = false;
            ProgressPanelActive(isFull);
            AlarifeDb->Active(isFull);
            delete impdb;
            return;
        }

        UpdateProgressPanel(10, tr("Creando la base de datos..."));
        impdb->createFileSystem();
        UpdateProgressPanel(25, tr("Importando medias..."));
        impdb->importMedias();
        UpdateProgressPanel(30, tr("Importando datos de la enciclopedia..."));
        impdb->newEncyclopedia();

        BookImportCommon();
        delete impdb;

        loginDlg *lgn = new loginDlg(this, &encyclopedia);
        lgn->show();
        connect(lgn,SIGNAL(closeDlg(bool)),this, SLOT(openConection(bool)));
    }else QMessageBox::critical(this, tr("Importar enciclopedia"),
                                tr("Operación cancelada, falta el software, "
                                   "7z, necesario para la importación."));

}

void MainWindow:: AddBookImportExec()
{
    curFile = QFileDialog::getOpenFileName(this, tr("Importar enciclopedia..."),
                                           QDir::homePath(),
                                           tr("Datos exportados de Alarife (*.alfe)"));

    if (!curFile.isEmpty())
    {
        ProgressPanelActive(true);
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(Exec_AddBookImport()));
        timer->start(1000);
    }else
        QMessageBox::critical(this, tr("Importar enciclopedia"),
                              tr("Operación cancelada por el usuario."));
}

void MainWindow::Exec_AddBookImport(){
    timer->stop();

    QString path = __SYS_DIR__;
    if (!path.contains("/bin")) path = __SYS_DIR__ + "bin/";

    if (QFile::exists(path + "7z" + __EXT__))
    {

        wait(1000);

        QFileInfo dbinfo(AlarifeDb->getPathFile());
        isFull = true;

        impdb = new dbImport(this, curFile);
        impdb->setMediaDir(dbinfo.baseName());
        UpdateProgressPanel(5, tr("Extrayendo datos..."));
        impdb->uncompress();

        if (!QFile::exists(__DIR_CACHE__ + "/orden.nl.alfe"))
        {
            QMessageBox::critical(this, tr("Importar enciclopedia"),
                                  tr("Operación cancelada, el fichero de "
                                     "importación contiene errores."));
            impdb->cleanTemporalFiles();
            isFull = false;
            ProgressPanelActive(isFull);
            AlarifeDb->Active(isFull);

            delete impdb;
            return;
        }

        UpdateProgressPanel(25, tr("Importando medias..."));
        impdb->importMedias();
        UpdateProgressPanel(30, tr("Importando datos de la enciclopedia..."));
        impdb->addEncyclopedia(id_master);

        BookImportCommon();
        delete impdb;
    }else QMessageBox::critical(this, tr("Importar enciclopedia"),
                                tr("Operación cancelada, falta el software, "
                                   "7z, necesario para la importación."));

}

void MainWindow::MergeBookImportExec()
{
    curFile = QFileDialog::getOpenFileName(this, tr("Importar enciclopedia..."),
                                           QDir::homePath(),
                                           tr("Datos exportados de Alarife (*.alfe)"));

    if (!curFile.isEmpty())
    {
        ProgressPanelActive(true);
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(Exec_MergeBookImport()));
        timer->start(1000);
    }else
        QMessageBox::critical(this, tr("Importar enciclopedia"),
                              tr("Operación cancelada por el usuario."));
}

void MainWindow::Exec_MergeBookImport()
{
    timer->stop();

    QString path = __SYS_DIR__;
    if (!path.contains("/bin")) path = __SYS_DIR__ + "bin/";

    if (QFile::exists(path + "7z" + __EXT__))
    {

        wait(1000);

        int father;
        QSqlRecord record = master->record(MasterId.row());

        if (record.value("id_nivel").toInt() != 0)
            father = record.value("padre").toInt();
        else
            father = stack->getNumber();

        QFileInfo dbinfo(AlarifeDb->getPathFile());
        isFull = true;

        impdb = new dbImport(this, curFile);
        impdb->setMediaDir(dbinfo.baseName());
        UpdateProgressPanel(5, tr("Extrayendo datos..."));
        impdb->uncompress();

        if (!QFile::exists(__DIR_CACHE__ + "/orden.nl.alfe"))
        {
            QMessageBox::critical(this, tr("Importar enciclopedia"),
                                  tr("Operación cancelada, el fichero de "
                                     "importación contiene errores."));
            impdb->cleanTemporalFiles();
            isFull = false;
            ProgressPanelActive(isFull);
            AlarifeDb->Active(isFull);
            delete impdb;
            return;
        }

        UpdateProgressPanel(25, tr("Importando medias..."));
        impdb->importMedias();
        UpdateProgressPanel(30, tr("Importando datos de la enciclopedia..."));
        impdb->mergeLevels(father);
        BookImportCommon();
        delete impdb;
    }else QMessageBox::critical(this, tr("Importar enciclopedia"),
                                tr("Operación cancelada, falta el software, "
                                   "7z, necesario para la importación."));
}

void MainWindow::BookImportCommon()
{
    QString name = impdb->getName();

    UpdateProgressPanel(40, tr("Importando niveles..."));
    impdb->importLevels();
    UpdateProgressPanel(55, tr("Importando artículos..."));
    impdb->importArticles();
    UpdateProgressPanel(70,tr("Importando artículos relacionados..."));
    impdb->importRelationArticles();
    UpdateProgressPanel(75, tr("Importando autores..."));
    impdb->importAuthor();
    UpdateProgressPanel(80, tr("Importando colaboradores y equipo..."));
    impdb->importTeamColaborator();
    UpdateProgressPanel(85, tr("Relacionando artículos y autores..."));
    impdb->importRelationAuthors();

    UpdateProgressPanel(90, tr("Importando imagenes deslizantes..."));
    impdb->importSlideShow();
    UpdateProgressPanel(94, tr("Importando galería de imagenes..."));
    impdb->importGallery();
    UpdateProgressPanel(97, tr("Importando referencias..."));
    impdb->importReference();

    UpdateProgressPanel(98, tr("Eliminando temporales..."));
    impdb->cleanTemporalFiles();
    UpdateProgressPanel(100, tr("Proceso finalizado..."));

    UpdateBookInfo();
    QMessageBox::information(this, tr("Importar enciclopedia"),
                             tr("La enciclopedia \"%1\" se ha importado "
                                "correctamente.").arg(name));
    isFull = false;
    ProgressPanelActive(false);
    OpenRelationTable(0,"");
    EnabledAccion(true);
}

void MainWindow::GeneratedHtmlExec()
{
    UpdateBookInfo();
    if (encyclopedia->getTemplate().isEmpty())
    {
        QMessageBox::information(this, tr("Generar enciclopedia"),
                                 tr("Seleccione en las propiedades de la "
                                    "enciclopedia una plantilla."));
    }else{
        QFileDialog::Options options = QFileDialog::DontResolveSymlinks |
                QFileDialog::ShowDirsOnly;

        QString directory = QFileDialog::getExistingDirectory(this, tr("Generar enciclopedia"),
                                                              QDir::home().absolutePath(),
                                                              options);
        if (!directory.isEmpty())
        {
            DirPath = directory;

            ProgressPanelActive(true);

            timer = new QTimer(this);
            connect(timer, SIGNAL(timeout()), this, SLOT(Exec_Generation()));
            timer->start(1500);
        }
    }
}

void MainWindow::Exec_Generation()
{
    timer->stop();

    if (!QFileInfo(DirPath).isWritable())
    {
        QMessageBox::critical(this, tr("Generar enciclopedia"),
                              tr("La enciclopedia \"%1\" no se puede generar "
                                 "en el directorio seleccionado; permiso denegado.").arg(encyclopedia->getTitle()));
    }else{

        isFull = true;
        GenAlf *gen = new GenAlf(this, encyclopedia);
        gen->setTargetPath(DirPath);

        gen->setIndex(encyclopedia->getIndex());

        UpdateProgressPanel(5,tr("Creando directorios..."));
        gen->createFileSystem();
        UpdateProgressPanel(10, tr("Generando Página índice..."));
        gen->generateIndex();
        UpdateProgressPanel(12, tr("Generando galería de imágenes..."));
        gen->generateGallery();
        UpdateProgressPanel(15, tr("Generando Página de autores..."));
        gen->generateAuthor();
        UpdateProgressPanel(20, tr("Generando Página de créditos..."));
        gen->generateCredits();
        UpdateProgressPanel(25, tr("Generando Página de ayuda..."));
        gen->generateHelp();
        UpdateProgressPanel(30, tr("Generando Página de colaboradores..."));
        gen->generateTeam();
        UpdateProgressPanel(35, tr("Generando Páginas de niveles..."));
        gen->generateLevels();
        UpdateProgressPanel(55, tr("Generando Páginas de artículos..."));
        gen->generateArticles();
        UpdateProgressPanel(75,tr("Generando buscador..."));
        gen->generateSearch();
        UpdateProgressPanel(80,tr("Copiando medias..."));
        gen->generateMedia();
        UpdateProgressPanel(98,tr("Generando estadísticas..."));
        gen->generateStadistic();
        UpdateProgressPanel(100, tr("Proceso finalizado..."));

        QMessageBox::information(this, tr("Generar enciclopedia"),
                                 tr("La enciclopedia \"%1\" se ha generado correctamente.").
                                    arg(encyclopedia->getTitle()));
        ProgressPanelActive(false);
        isFull = false;
        delete gen;
    }
}

void MainWindow::BuilderAlarifeExec(){

}

void MainWindow::RestructuringData()
{

    wait(1000);
    int i = sqlAutoCountRow("nivel", "padre NOT IN (SELECT id_nivel FROM nivel) AND padre != 0");

    if (i > 0)
    {
        sqlClearOrphanLevels();
        sqlClearOrphanArticles();
        sqlClearArticlesAuthorRelations();
        RestructuringData();
    }else
        QMessageBox::information(0, tr("Integridad referencia"),
                                 tr("La integridad referencial se ha analizado con éxito"),
                                 QMessageBox::Ok);
}


void MainWindow::ContentsExec()
{
    QString path = __SYS_DIR__;
    if (path.contains("/bin")) path.remove("/bin", Qt::CaseSensitive);
    QString helpPath = QString("%1/help-browser%2 %3/doc/").
                       arg(QCoreApplication::applicationDirPath()).
                       arg(__EXT__).
                       arg(path);
    if (QFile(QString("%1/doc/index.htm").arg(path)).exists())
        helpPath.append("index.htm");
    else
        helpPath.append("index2.htm");

    QProcess *helpProcess = new QProcess(this);
    helpProcess->start(helpPath);
}

void MainWindow::CheckedBoss()
{
    ConfigDialog *config = new ConfigDialog(this, &encyclopedia);
    config->show();
}

void MainWindow::ConfigurationExec(bool access)
{
    if (access == true)
    {
        ConfigDialog *config = new ConfigDialog(this, &encyclopedia);
        config->show();
    }
}

void MainWindow::AboutExec()
{
    AboutBox *credits = new AboutBox(this);
    credits->show();
}

void MainWindow::changeUser()
{
    loginDlg *login2 = new loginDlg(this, &encyclopedia);
    login2->show();
    connect(login2,SIGNAL(closeDlg(bool)),this, SLOT(openConection(bool)));
}

void MainWindow::editClose()
{
    delete edit;
    edit = NULL;
}

void MainWindow::changeLevelImage()
{
    int btn = QMessageBox::question(this, tr("Cambiar imagen"),
                                    tr("¿Desea cambiar la imagen actual?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if (btn == QMessageBox::Yes)
    {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Seleccionar foto de autor"),
                                                        QDir::home().absolutePath(),
                                                        tr("Archivo de imagen JPG (*.jpg)"));
        if (!fileName.isEmpty()) loadImage(fileName);
    }

}

void MainWindow::loadImage(QString fileName)
{
    if (QFile(fileName).exists())
    {
        pixmap = QPixmap(fileName);
    }else{
        pixmap = QPixmap(":/images/no_image1.png");
    }
}

QString MainWindow::selectImage(int index)
{
    QSqlQuery *qryTemp = new QSqlQuery;
    qryTemp->exec(QString("SELECT adjuntos.id_adjunto||'.'||adjuntos.extension AS mediaFile "
                          "FROM adjuntos WHERE adjuntos.id_adjunto = %1").arg(index));
    qryTemp->first();

    QString text("");
    text = qryTemp->value(0).toString();
    delete qryTemp;
    return text;
}

void MainWindow::adminUser()
{
    bool access;
    QString passwd = QInputDialog::getText(this,
                                           tr("Contraseña de administración"),
                                           tr("Escriba la contraseña del Editor Jefe:"),
                                           QLineEdit::Password, "", &access);
    if (access && !passwd.isEmpty())
    {
        QCryptographicHash *md5 = new QCryptographicHash(QCryptographicHash::Md5);
        QByteArray *byte = new QByteArray;

        md5->addData(byte->append(passwd));
        QSqlQuery *qry = new QSqlQuery;
        qry->exec(QString("SELECT tipousuario, clave FROM usuario WHERE tipousuario = 1"));
        qry->first();

        if (qry->value(1).toString() != md5->result().data())
        {
            QMessageBox::critical(this, tr("Autenticación"),
                                  tr("Error. Contraseña incorrecta. Inténtelo otra vez"));
        }
        else{
            dlgAdminUser *user = new dlgAdminUser(this);
            user->show();
        }
     }
}

void MainWindow::adminTemplate()
{
    dlgAdminTemplates *templates = new dlgAdminTemplates(this);
    templates->show();
}

void MainWindow::adminStyles()
{
    dlgAdminStyles *styleDlg = new dlgAdminStyles(this);
    styleDlg->show();
}

bool MainWindow::isOwner(QSqlRecord record)
{
    int user = record.value("usuario").toInt();
    if (encyclopedia->getUserIndex() == user || encyclopedia->getUserIndex() == 1)
        return true;
    else
        return false;
}
