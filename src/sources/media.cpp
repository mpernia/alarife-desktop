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
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>
#include "../includes/media.h"
#include "../includes/alarife.h"
#include "../includes/loadimagedlg.h"
#include "../includes/gridimage.h"
#include "../includes/sqlfunctions.h"



MediaOption::MediaOption(QWidget *parent, Book *encic) : QWidget(parent)
{
    encyclopedia = encic;

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

    pageImage = new ImagePage;
    pageSound = new SoundPage;
    pageVideo = new VideoPage;
    pageOtyher = new OtherMediaPage;

    pageImage->setParam(extImagen);
    pageSound->setParam(extSonido);
    pageVideo->setParam(extVideo);
    pageOtyher->setParam(extXMedia);

    pageImage->setMediaPath(encyclopedia->getMediaPath());
    pageSound->setMediaPath(encyclopedia->getMediaPath());
    pageVideo->setMediaPath(encyclopedia->getMediaPath());
    pageOtyher->setMediaPath(encyclopedia->getMediaPath());

    pageImage->setUser(encyclopedia->getUserIndex());
    pageSound->setUser(encyclopedia->getUserIndex());
    pageVideo->setUser(encyclopedia->getUserIndex());
    pageOtyher->setUser(encyclopedia->getUserIndex());

    pageImage->setExtensionFiles(encyclopedia->getExtImagen());
    pageSound->setExtensionFiles(encyclopedia->getExtSonido());
    pageVideo->setExtensionFiles(encyclopedia->getExtVideo());
    pageOtyher->setExtensionFiles(encyclopedia->getExtXMedia());

    PageControl = new QStackedWidget;
    PageControl->addWidget(pageImage);
    PageControl->addWidget(pageSound);
    PageControl->addWidget(pageVideo);
    PageControl->addWidget(pageOtyher);

    pageImage->SqlExec("", 1);
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

    connect(PageControl, SIGNAL(currentChanged(int)), this, SLOT(showData(int)));
}

void MediaOption::createIcons()
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

void MediaOption::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current) current = previous;

    PageControl->setCurrentIndex(lstOptions->row(current));
}

void MediaOption::showData(int page)
{
    switch (page)
    {
    case 0 : pageImage->SqlExec("", page + 1);
        break;
    case 1 : pageSound->SqlExec("", page + 1);
        break;
    case 2 : pageVideo->SqlExec("", page + 1);
        break;
    case 3 : pageOtyher->SqlExec("", page + 1);
        break;
    }
}

void MediaOption::clearFilter()
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

TabPageMedia::TabPageMedia(QWidget *parent/*, int id*/) : QWidget(parent){
    lblCaption = new QLabel;
    edtFilter = new QLineEdit;

    btnFilter = new QToolButton;
    btnEdit = new QToolButton;
    btnInsert = new QToolButton;
    btnDelete = new QToolButton;

    chkDeleteAll = new QCheckBox;

    btnFilter->setIcon(QIcon(":/images/action_find.png"));
    btnEdit->setIcon(QIcon(":/images/action_edit.png"));
    btnDelete->setIcon(QIcon(":/images/action_delete.png"));
    btnInsert->setIcon(QIcon(":/images/medias.png"));

    btnFilter->setToolTip(tr("Buscar"));
    btnEdit->setToolTip(tr("Editar"));
    btnDelete->setToolTip(tr("Eliminar "));
    btnInsert->setToolTip(tr("Insertar"));

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

    QGridLayout *lytButtonPanel = new QGridLayout;
    lytButtonPanel->setAlignment(Qt::AlignRight);
    lytButtonPanel->addWidget(btnInsert,0,1);
    lytButtonPanel->addWidget(btnEdit,0,2);
    lytButtonPanel->addWidget(btnDelete,0,3);

    QHBoxLayout *lytFilter = new QHBoxLayout;
    lblText = new QLabel(tr("Buscar:"));
    lytFilter->addWidget(lblText);
    lytFilter->addWidget(edtFilter);
    lytFilter->addWidget(btnFilter);

    QHBoxLayout *lytTop = new QHBoxLayout;
    lytTop->addLayout(lytFilter);
    lytTop->addLayout(lytButtonPanel);

    QVBoxLayout *lytPageControl = new QVBoxLayout;
    lytPageControl->addWidget(lblCaption);
    lytPageControl->addLayout(lytTop);
    lytPageControl->addWidget(tblGridView);
    lytPageControl->addWidget(chkDeleteAll);

    setLayout(lytPageControl);

    connect(btnFilter, SIGNAL(clicked()), this, SLOT(FilterMediaExec()));
    connect(edtFilter,SIGNAL(textChanged(QString)),this, SLOT(EmptyFilter()));
    connect(edtFilter, SIGNAL(returnPressed()), this, SLOT(FilterMediaExec()));

    connect(btnEdit, SIGNAL(clicked()), this, SLOT(EditRecordExec()));
    connect(btnDelete, SIGNAL(clicked()), this, SLOT(DeleteRecordExec()));
    connect(btnInsert, SIGNAL(clicked()), this, SLOT(InsertRecordExec()));

    connect(tblGridView, SIGNAL(clicked(QModelIndex)), this, SLOT(GetIndex(QModelIndex)));
    connect(tblGridView, SIGNAL(pressed(QModelIndex)), this, SLOT(GetIndex(QModelIndex)));
    connect(tblGridView, SIGNAL(activated(QModelIndex)), this, SLOT(GetIndex(QModelIndex)));
    connect(tblGridView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(EditRecordExec()));

    sql_qry = QString(tr("SELECT 'Nombre: ' || adjuntos.id_adjunto || "
                         "'\n' || 'Extensión: ' || adjuntos.extension || "
                         "'\n' || 'Descripción: ' || adjuntos.descripcion AS description, "
                         "adjuntos.id_adjunto||'.'||adjuntos.extension AS mediaFile, "
                         "adjuntos.id_adjunto FROM adjuntos WHERE %1 AND adjuntos.id_adjunto "
                         "> 0 ORDER BY adjuntos.id_adjunto"));
}

void TabPageMedia::setParam(QString p)
{
    param = p;
}

void TabPageMedia::setExtensionFiles(QString e)
{
    ext = e;
}

void TabPageMedia::setUser(int id)
{
    idUser = id;
    if (idUser != 1){
        btnDelete->setVisible(false);
        chkDeleteAll->setVisible(false);
    }
}

void TabPageMedia::FilterMediaExec()
{
    if(edtFilter->text() != "")
    {
        QString str(param);
        str.append(QString(" AND descripcion LIKE '%%1%'").arg(edtFilter->text()));
        SqlExec(str, opt);
    }
}

void TabPageMedia::EmptyFilter()
{
    if (edtFilter->text() == "")
    {
        SqlExec("", opt);
        edtFilter->setStyleSheet(tr("background-color: rgb(255, 255, 255);"));
    }else
        edtFilter->setStyleSheet(tr("background-color: rgb(255, 255, 255);"));
}

void TabPageMedia::GetIndex(QModelIndex index)
{
    media_index = index;
}

void TabPageMedia::setCaptionExec(QString caption)
{
    lblCaption->setText(QString("%1 de la enciclopedia").arg(caption));
    chkDeleteAll->setText(tr("Eliminar todas las medias del tipo \"%1\".").arg(caption));
}

void TabPageMedia::EditRecordExec()
{
    QSqlRecord *record = new QSqlRecord(medias->record(media_index.row()));
    if (record->value("id_adjunto").toInt() != 0)
    {
        LoadImageDlg *editImgdlg = new LoadImageDlg(this, record->value("id_adjunto").toInt());
        editImgdlg->setFileExtension(ext);
        editImgdlg->setFileType(opt);
        editImgdlg->setMediaPath(mediaPath);
        editImgdlg->loadValues();
        editImgdlg->show();
        connect(editImgdlg,SIGNAL(refreshQuery()),this, SLOT(refreshGrid()));
    } else
        QMessageBox::critical(this, tr("Editar X-Media"),
                              tr("Debe seleccionar el archivo X-Media que "
                                 "desea editar."));
}

void TabPageMedia::InsertRecordExec()
{
    LoadImageDlg *imagedlg = new LoadImageDlg(this, 0);
    imagedlg->setFileExtension(ext);
    imagedlg->setMediaPath(mediaPath);
    imagedlg->setFileType(opt);
    imagedlg->show();
    connect(imagedlg,SIGNAL(refreshQuery()),this, SLOT(refreshGrid()));
}

void TabPageMedia::DeleteRecordExec()
{
    if (!chkDeleteAll->isChecked())
    {
        QSqlRecord *record = new QSqlRecord(medias->record(media_index.row()));

        QString reg(record->value("mediaFile").toString());

        int button = QMessageBox::question(this, tr("Confirmar eliminación"),
                                           tr("Está seguro que desea eliminar el "
                                              "archivo \"%1\"?").arg(reg),
                                           QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::Yes)
        {
            if(!sqlDeleteRecord_N("adjuntos",
                                QString("id_adjunto = %1").arg(record->value("id_adjunto").toInt())))
            {
                QMessageBox::information(this, tr("Eliminación Fallida"),
                                         tr("El archivo \"%1\" no se ha eliminado de la "
                                            "enciclopedia.").arg(reg));
            }else{
                QMessageBox::information(this, tr("Eliminación Satisfactoria"),
                                         tr("El archivo \"%1\" se ha eliminado de la "
                                            "enciclopedia.").arg(reg));
                QFile().remove(QString("%1/%2").arg(mediaPath).arg(reg));
                refreshGrid();
            }
            delete record;
        }
    }else{
        int button = QMessageBox::warning(this,
                                          tr("Confirmar eliminación"),
                                          tr("ATENCIÓN: Está activa la casilla para eliminar "
                                             "todas las medias registradas de este tipo. Está "
                                             "seguro que desea eliminar todas las medias?"),
                                          QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::Yes)
        {
            QSqlQuery *qryFiles = new QSqlQuery(SQL_ListMedia.arg(param));

            qryFiles->first();
            QFile().remove(QString("%1/%2").arg(mediaPath).arg(qryFiles->value(0).toString()));

            while (qryFiles->next())
            {
                QFile().remove(QString("%1/%2").arg(mediaPath).arg(qryFiles->value(0).toString()));
            }

            sqlDeleteRecord_N("adjuntos", param);
            refreshGrid();
            chkDeleteAll->setChecked(false);
        }else QMessageBox::critical(this, tr("Confirmar eliminación"),
                                       tr("Operación cancelada por el usuario"));
    }
}

void TabPageMedia::OpenTable()
{
    tblGridView->setColumnWidth(0,390);
    tblGridView->setColumnWidth(1,85);
    tblGridView->hideColumn(2);
    tblGridView->selectRow(0);
}

void TabPageMedia::SqlExec(QString filter, int mode)
{
    medias = new QSqlQueryModel;
    if (filter.isEmpty())
        medias->setQuery(sql_qry.arg(param));
    else{
        medias->setQuery(sql_qry.arg(filter));
        if(medias->query().first()==false)
            edtFilter->setStyleSheet(tr("background-color: rgb(255, 140, 140);"));
    }

    while (medias->canFetchMore()) medias->fetchMore();
    tblGridView->setModel(medias);

    OpenTable();
    for (int i=0; i < medias->rowCount(); i++){
        tblGridView->setRowHeight(i,85);
    }

    opt = mode;

    tblGridView->setItemDelegate(new GridImg(tblGridView, mode, mediaPath));
    tblGridView->show();

    QString text = lblCaption->text();

    if (text.contains(":")){
        int pos = lblCaption->text().toStdString().find(":");
        text.truncate(pos);
    }

    lblCaption->setText(QString("%1: %2").arg(text).arg(medias->rowCount(media_index)));
}

void TabPageMedia::refreshGrid()
{
    QString sql = medias->query().lastQuery();

    medias->setQuery(sql);

    while (medias->canFetchMore()) medias->fetchMore();
    tblGridView->setModel(medias);

    OpenTable();
    for (int i=0; i < medias->rowCount(); i++)
    {
        tblGridView->setRowHeight(i,85);
    }

    tblGridView->setItemDelegate(new GridImg(tblGridView, opt, mediaPath));
    tblGridView->show();
}

ImagePage::ImagePage(TabPageMedia *parent) : TabPageMedia(parent)
{
    setCaptionExec(tr("Imágenes"));
}

SoundPage::SoundPage(TabPageMedia *parent) : TabPageMedia(parent)
{
    setCaptionExec(tr("Sonidos"));
}

VideoPage::VideoPage(TabPageMedia *parent) : TabPageMedia(parent)
{
    setCaptionExec(tr("Videos"));
}

OtherMediaPage::OtherMediaPage(TabPageMedia *parent) : TabPageMedia(parent)
{
    setCaptionExec(tr("X-Medias"));
}

