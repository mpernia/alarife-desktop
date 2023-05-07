/****************************************************************************
**
** Copyleft: 2012. Maikel Enrique Pernia Matos.
** Contact: corba@grannet.grm.sld.cu
**
** This file is part of the Alarife-IV application.
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
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>

#include "../includes/wdgmedia.h"
#include "../includes/alarife.h"
#include "../includes/database.h"
#include "../includes/gridimage.h"
#include "../includes/sqlfunctions.h"

InsertMediaPanel::InsertMediaPanel(QWidget *parent, WebView *wv, Book *encyc) : QWidget(parent)
{
    encyclopedia = encyc;

    extImagen = QString("descripcion != 'SIN IMAGEN' AND (%1')").arg(encyclopedia->getExtImagen());
    extSonido = QString("(%1')").arg(encyclopedia->getExtSonido());
    extVideo = QString("(%1')").arg(encyclopedia->getExtVideo());
    extXMedia = QString("(%1')").arg(encyclopedia->getExtXMedia());

    clearFilter();

    lstOptions = new QListWidget;
    lstOptions->setViewMode(QListView::IconMode);
    lstOptions->setIconSize(QSize(96, 84));
    lstOptions->setMovement(QListView::Static);
    lstOptions->setMaximumWidth(128);
    lstOptions->setMinimumHeight(478);
    lstOptions->setSpacing(12);

    pageimage = new InsertImagePage;
    pagesound = new InsertSoundPage;
    pagevideo = new InsertVideoPage;
    pageother = new InsertOtherMediaPage;

    pageimage->setParam(extImagen);
    pagesound->setParam(extSonido);
    pagevideo->setParam(extVideo);
    pageother->setParam(extXMedia);

    pageimage->setMediaPath(encyclopedia->getMediaPath());
    pagesound->setMediaPath(encyclopedia->getMediaPath());
    pagevideo->setMediaPath(encyclopedia->getMediaPath());
    pageother->setMediaPath(encyclopedia->getMediaPath());

    pageimage->setWebView(wv);
    pagesound->setWebView(wv);
    pagevideo->setWebView(wv);
    pageother->setWebView(wv);

    PageControl = new QStackedWidget;
    PageControl->addWidget(pageimage);
    PageControl->addWidget(pagesound);
    PageControl->addWidget(pagevideo);
    PageControl->addWidget(pageother);

    createIcons();
    lstOptions->setCurrentRow(0);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(lstOptions);
    horizontalLayout->addWidget(PageControl, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    setLayout(mainLayout);

    setWindowTitle(tr("Medias de la enciclopedia"));
}

void InsertMediaPanel::createIcons()
{
    QListWidgetItem *btnAuthor = new QListWidgetItem(lstOptions);
    btnAuthor->setIcon(QIcon(":/images/imagen.png"));
    btnAuthor->setText(tr("Imágenes"));
    btnAuthor->setTextAlignment(Qt::AlignHCenter);
    btnAuthor->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *btnMedia = new QListWidgetItem(lstOptions);
    btnMedia->setIcon(QIcon(":/images/audio.png"));
    btnMedia->setText(tr("Sonidos"));
    btnMedia->setTextAlignment(Qt::AlignHCenter);
    btnMedia->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *btnTemplate = new QListWidgetItem(lstOptions);
    btnTemplate->setIcon(QIcon(":/images/video.png"));
    btnTemplate->setText(tr("Videos"));
    btnTemplate->setTextAlignment(Qt::AlignHCenter);
    btnTemplate->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *btnTemplate1 = new QListWidgetItem(lstOptions);
    btnTemplate1->setIcon(QIcon(":/images/xmedias.png"));
    btnTemplate1->setText(tr("X-Medias"));
    btnTemplate1->setTextAlignment(Qt::AlignHCenter);
    btnTemplate1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(lstOptions, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
}

void InsertMediaPanel::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current) current = previous;

    PageControl->setCurrentIndex(lstOptions->row(current));

    switch (lstOptions->currentRow())
    {
    case 0: pageimage->SqlExec("", 1);
        break;
    case 1: pagesound->SqlExec("", 2);
        break;
    case 2: pagevideo->SqlExec("", 3);
        break;
    case 3: pageother->SqlExec("", 4);
        break;
    }

}

void InsertMediaPanel::clearFilter()
{
    extImagen.replace("*.","extension = '");
    extImagen.replace(";","' OR ");
    extSonido.replace("*.","extension = '");
    extSonido.replace(";","' OR ");
    extVideo.replace("*.","extension = '");
    extVideo.replace(";","' OR ");
    extXMedia.replace("*.","extension = '");
    extXMedia.replace(";","' OR ");

}

TabPageInsertMedia::TabPageInsertMedia(QWidget *parent) : QWidget(parent)
{
    lblCaption = new QLabel;
    edtFilter = new QLineEdit;

    btnFilter = new QToolButton;
    btnFilter->setIcon(QIcon(":/images/action_find.png"));
    btnFilter->setToolTip(tr("Buscar"));

    btnInsert = new QToolButton;
    btnInsert->setIcon(QIcon(":/images/edit-insert-media.png"));
    btnInsert->setToolTip("Insertar");

    tblGridView = new QTableView;
    tblGridView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblGridView->setSortingEnabled(false);
    tblGridView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblGridView->setSelectionMode(QAbstractItemView::SingleSelection);
    tblGridView->setShowGrid(false);
    tblGridView->setAlternatingRowColors(true);
    tblGridView->setWordWrap(true);
    tblGridView->verticalHeader()->hide();
    tblGridView->horizontalHeader()->hide();

    QHBoxLayout *lytFilter = new QHBoxLayout;
    lblText = new QLabel(tr("Buscar:"));
    lytFilter->addWidget(lblText);
    lytFilter->addWidget(edtFilter);
    lytFilter->addWidget(btnFilter);
    lytFilter->addWidget(btnInsert);

    QHBoxLayout *lytTop = new QHBoxLayout;
    lytTop->addLayout(lytFilter);

    QVBoxLayout *lytPageControl = new QVBoxLayout;
    lytPageControl->addWidget(lblCaption);
    lytPageControl->addLayout(lytTop);
    lytPageControl->addWidget(tblGridView);

    setLayout(lytPageControl);

    connect(btnFilter, SIGNAL(clicked()), this, SLOT(FilterMediaExec()));
    connect(edtFilter,SIGNAL(textChanged(QString)),this, SLOT(EmptyFilter()));
    connect(edtFilter, SIGNAL(returnPressed()), this, SLOT(FilterMediaExec()));

    connect(tblGridView, SIGNAL(clicked(QModelIndex)), this, SLOT(GetIndex(QModelIndex)));

    sql_qry = QString(tr("SELECT 'Nombre: ' || adjuntos.id_adjunto || "
                         "'\n' || 'Extensión: ' || adjuntos.extension || "
                         "'\n' || 'Descripción: ' || adjuntos.descripcion AS description, "
                         "adjuntos.id_adjunto||'.'||adjuntos.extension AS mediaFile, "
                         "adjuntos.id_adjunto, adjuntos.descripcion FROM adjuntos WHERE %1 AND adjuntos.id_adjunto "
                         "> 0 ORDER BY adjuntos.id_adjunto"));
}

void TabPageInsertMedia::setWebView(WebView *view)
{
    webView = view;
}

void TabPageInsertMedia::setParam(QString p)
{
    param = p;
}

void TabPageInsertMedia::refresh()
{

}

void TabPageInsertMedia::GetIndex(QModelIndex index)
{
    media_index = index;
}

void TabPageInsertMedia::FilterMediaExec(){
    if(edtFilter->text() != ""){
        QString str(param);
        str.replace("' OR", QString("' AND descripcion LIKE '%%1%' OR").
                      arg(edtFilter->text()));
        SqlExec(str, opt);
    }
}

void TabPageInsertMedia::EmptyFilter()
{
    if (edtFilter->text() == "")
    {
        edtFilter->setStyleSheet(tr("background-color: rgb(255, 255, 255);"));
        SqlExec("", opt);
    }
}

void TabPageInsertMedia::setCaptionExec(QString caption)
{
    lblCaption->setText(QString("%1 de la enciclopedia").arg(caption));
}

void TabPageInsertMedia::OpenTable()
{
    tblGridView->setColumnWidth(0,390);
    tblGridView->setColumnWidth(1,85);
    tblGridView->hideColumn(2);
    tblGridView->hideColumn(3);
    tblGridView->selectRow(0);

}

void TabPageInsertMedia::SqlExec(QString filter, int mode)
{
    medias = new QSqlQueryModel;
    if (filter.isEmpty())
        medias->setQuery(sql_qry.arg(param));
    else
        medias->setQuery(sql_qry.arg(filter));

    while (medias->canFetchMore())
        medias->fetchMore();
    tblGridView->setModel(medias);

    OpenTable();
    for (int i=0; i < medias->rowCount(); i++)
    {
        tblGridView->setRowHeight(i,85);
    }

    opt = mode;

    tblGridView->setItemDelegate(new GridImg(tblGridView, mode, mediaPath));

    tblGridView->show();
}

InsertImagePage::InsertImagePage(TabPageInsertMedia *parent) : TabPageInsertMedia(parent)
{
    setCaptionExec(tr("Imágenes"));
    connect(tblGridView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(InsertMediaExec(QModelIndex)));

    connect(btnInsert, SIGNAL(clicked()), this, SLOT(InsertImage()));
}

InsertSoundPage::InsertSoundPage(TabPageInsertMedia *parent) : TabPageInsertMedia(parent)
{
    setCaptionExec(tr("Sonidos"));
    connect(tblGridView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(InsertMediaExec(QModelIndex)));

    connect(btnInsert, SIGNAL(clicked()), this, SLOT(InsertImage()));
}

InsertVideoPage::InsertVideoPage(TabPageInsertMedia *parent) : TabPageInsertMedia(parent)
{
    setCaptionExec(tr("Videos"));
    connect(tblGridView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(InsertMediaExec(QModelIndex)));

    connect(btnInsert, SIGNAL(clicked()), this, SLOT(InsertImage()));
}

InsertOtherMediaPage::InsertOtherMediaPage(TabPageInsertMedia *parent) : TabPageInsertMedia(parent)
{
    setCaptionExec(tr("X-Medias"));
    connect(tblGridView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(InsertMediaExec(QModelIndex)));

    connect(btnInsert, SIGNAL(clicked()), this, SLOT(InsertImage()));
}

void InsertImagePage::InsertMediaExec(QModelIndex index){
    QSqlRecord *record = new QSqlRecord(medias->record(index.row()));

    if (record->value("id_adjunto").toInt() != 0)
    {
        QString image = sqlGetImageName(record->value("id_adjunto").toInt());
        QString text = record->value("descripcion").toString();
        if (text.contains("'")) text.replace("'","&#39;");
        if(mediaPath.contains("'")) mediaPath.replace("'","&#39;");
        QString imgHtml = QString("<img alt=\"%1\" src=\"%2/%3\" title=\"%4\"/>").
                                 arg(image).
                                 arg(mediaPath).
                                 arg(image).
                                 arg(text);//record->value("descripcion").toString());

        QString jsCode = QString("CKEDITOR.instances.editor.insertHtml('%1')").arg(imgHtml);
        webView->page()->mainFrame()->evaluateJavaScript(jsCode);
    }
}

void InsertImagePage::InsertImage()
{
    InsertMediaExec(media_index);
}

void InsertSoundPage::InsertMediaExec(QModelIndex index)
{
    QSqlRecord *record = new QSqlRecord(medias->record(index.row()));

    if (record->value("id_adjunto").toInt() != 0)
    {
        QString path = qApp->applicationDirPath();
        if (path.contains("/bin")) path.remove("/bin", Qt::CaseSensitive);
        QString image = sqlGetImageName(record->value("id_adjunto").toInt());
        QString text = record->value("descripcion").toString();
        if (text.contains("'")) text.replace("'","&#39;");
        if(mediaPath.contains("'")) mediaPath.replace("'","&#39;");
        QString imgHtml = QString("<a type=\"audio\" href=\"%1/%2\"><img style=\"vertical-align: text-top;\" title=\"%3\" "
                                  "src=\"%4/qjseditor/images/audio.jpg\" alt=\"%5\" width=\"50\" "
                                  "height=\"50\" /></a>").
                                 arg(mediaPath).
                                 arg(image).
                                 arg(text).
                                 arg(path).
                                 arg(image);

        QString jsCode = QString("CKEDITOR.instances.editor.insertHtml('%1')").arg(imgHtml);
        webView->page()->mainFrame()->evaluateJavaScript(jsCode);

    }
}

void InsertSoundPage::InsertImage()
{
    InsertMediaExec(media_index);
}

void InsertVideoPage::InsertMediaExec(QModelIndex index)
{
    QSqlRecord *record = new QSqlRecord(medias->record(index.row()));

    if (record->value("id_adjunto").toInt() != 0)
    {
        QString path = qApp->applicationDirPath();
        if (path.contains("/bin")) path.remove("/bin", Qt::CaseSensitive);
        QString image = sqlGetImageName(record->value("id_adjunto").toInt());
        QString text = record->value("descripcion").toString();
        if (text.contains("'")) text.replace("'","&#39;");
        if(mediaPath.contains("'")) mediaPath.replace("'","&#39;");
        QString imgHtml = QString("<a type=\"video\" href=\"%1/%2\"><img style=\"vertical-align: text-top;\" title=\"%3\" "
                                  "src=\"%4/qjseditor/images/video.jpg\" alt=\"%5\" width=\"50\" "
                                  "height=\"50\" /></a>").
                                 arg(mediaPath).
                                 arg(image).
                                 arg(text).
                                 arg(path).
                                 arg(image);

        QString jsCode = QString("CKEDITOR.instances.editor.insertHtml('%1')").arg(imgHtml);
        webView->page()->mainFrame()->evaluateJavaScript(jsCode);
    }
}

void InsertVideoPage::InsertImage()
{
    InsertMediaExec(media_index);
}

void InsertOtherMediaPage::InsertMediaExec(QModelIndex index)
{
    QSqlRecord *record = new QSqlRecord(medias->record(index.row()));

    if (record->value("id_adjunto").toInt() != 0)
    {
        QString path = qApp->applicationDirPath();
        if (path.contains("/bin")) path.remove("/bin", Qt::CaseSensitive);
        QString image = sqlGetImageName(record->value("id_adjunto").toInt());
        QString text = record->value("descripcion").toString();
        if (text.contains("'")) text.replace("'","&#39;");
        if(mediaPath.contains("'")) mediaPath.replace("'","&#39;");
        QString imgHtml = QString("<a href=\"%1/%2\"><img style=\"vertical-align: text-top;\" title=\"%3\" "
                                  "src=\"%4/qjseditor/images/xmedias.jpg\" alt=\"%5\" width=\"50\" "
                                  "height=\"50\" /></a>").
                                 arg(mediaPath).
                                 arg(image).
                                 arg(text).
                                 arg(path).
                                 arg(image);

        QString jsCode = QString("CKEDITOR.instances.editor.insertHtml('%1')").arg(imgHtml);
        webView->page()->mainFrame()->evaluateJavaScript(jsCode);
    }
}

void InsertOtherMediaPage::InsertImage()
{
    InsertMediaExec(media_index);
}
