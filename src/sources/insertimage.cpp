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

#include "../includes/insertimage.h"
#include "../includes/gridimage.h"

InsertImage::InsertImage(QWidget *parent) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    setMinimumSize(520, 500);
    setMaximumSize(520, 500);

    setWindowTitle(tr("Insertar imagen"));

    lblCaption = new QLabel;
    edtFilter = new QLineEdit;

    btnFilter = new QToolButton;
    btnFilter->setIcon(QIcon(":/images/find.png"));
    btnFilter->setToolTip(tr("Buscar"));

    btnInsert = new QToolButton;
    btnInsert->setIcon(QIcon(":/images/image.png"));
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
    connect(btnInsert, SIGNAL(clicked()), this, SLOT(insertMedia()));
    connect(edtFilter,SIGNAL(textChanged(QString)),this, SLOT(EmptyFilter()));
    connect(edtFilter, SIGNAL(returnPressed()), this, SLOT(FilterMediaExec()));
    connect(tblGridView, SIGNAL(clicked(QModelIndex)), this, SLOT(setModelIndex(QModelIndex)));
    connect(tblGridView, SIGNAL(activated(QModelIndex)), this, SLOT(setModelIndex(QModelIndex)));
    connect(tblGridView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(InsertMediaExec(QModelIndex)));

    num = -1;
    param = extImage;
    param.replace("*.","extension = '");
    param.replace(";","' OR ");

    sql_qry = QString(tr("SELECT 'Nombre: ' || adjuntos.id_adjunto || "
                         "'\n' || 'Extensión: ' || adjuntos.extension || "
                         "'\n' || 'Descripción: ' || adjuntos.descripcion AS description, "
                         "adjuntos.id_adjunto||'.'||adjuntos.extension AS mediaFile, "
                         "adjuntos.id_adjunto FROM adjuntos WHERE %1' AND adjuntos.id_adjunto "
                         "> 0 ORDER BY adjuntos.id_adjunto"));

SqlExec("");
}

void InsertImage::closeEvent(QCloseEvent *event)
{
    emit getSelectImage(fileName);
    emit compareSelectImage(num);
    event->accept();
}

void InsertImage::FilterMediaExec()
{
    if(edtFilter->text() != "")
    {
        QString str(param);
        str.replace("' OR", QString("' AND descripcion LIKE '%%1%' OR").
                      arg(edtFilter->text()));
        SqlExec(str);
    }
}

void InsertImage::EmptyFilter()
{
    if (edtFilter->text() == "")
    {
        edtFilter->setStyleSheet(tr("background-color: rgb(255, 255, 255);"));
        SqlExec("");
    }
}

void InsertImage::insertMedia()
{
    if (modelIndex.isValid())
        InsertMediaExec(modelIndex);
}

void InsertImage::setModelIndex(QModelIndex index)
{
    modelIndex = index;
}

void InsertImage::InsertMediaExec(QModelIndex index)
{
    QSqlRecord *record = new QSqlRecord(medias->record(index.row()));

    if (record->value("id_adjunto").toInt() != 0)
    {
        fileName = record->value("mediaFile").toString();
        num = record->value("id_adjunto").toInt();
        close();
    }
}

void InsertImage::OpenTable()
{
    tblGridView->setColumnWidth(0,390);
    tblGridView->setColumnWidth(1,85);
    tblGridView->hideColumn(2);
    tblGridView->hideColumn(3);
    tblGridView->selectRow(0);
}

void InsertImage::SqlExec(QString filter)
{
    medias = new QSqlQueryModel;
    if (filter.isEmpty())
        medias->setQuery(sql_qry.arg(param));
    else
        medias->setQuery(sql_qry.arg(filter));

    while (medias->canFetchMore()) medias->fetchMore();
    tblGridView->setModel(medias);

    OpenTable();
    for (int i=0; i < medias->rowCount(); i++)
    {
        tblGridView->setRowHeight(i,85);
    }

    tblGridView->setItemDelegate(new GridImg(tblGridView, 0, mediaPath + ".alf"));

  tblGridView->show();
}

