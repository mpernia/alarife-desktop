/****************************************************************************
**
** Copyleft: 2014. Maikel Enrique Pernia Matos.
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
#include <QWebView>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSpacerItem>


#include "../includes/slide.h"
#include "../includes/alarife.h"
#include "../includes/sqlfunctions.h"
#include "../includes/strfunction.h"


Slide::Slide(QWidget *parent, int page, char type) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    slidePage = page;
    slideType = type;
    slideId = sqlGetIdValue("slide", "id_slide",
                           QString(" id_pagina = %1 AND tipo = '%2' ").
                                   arg(slidePage).
                                   arg(slideType));

    sqlFilter = "";

    setWindowTitle(tr("Imágenes deslizantes"));
    setMinimumSize(500, 400);

    CreateWidtgets();

    rowLeft = -1;
    rowRight = -1;
    connect(btnFilter, SIGNAL(clicked()), this, SLOT(FilterImage()));
    connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(btnPreview, SIGNAL(clicked()), this, SLOT(showPreview()));
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(moveRightToLeft()));
    connect(btnAddAll, SIGNAL(clicked()), this, SLOT(moveAllToLeft()));
    connect(btnDel, SIGNAL(clicked()), this, SLOT(moveLeftToRight()));
    connect(btnDelAll, SIGNAL(clicked()), this, SLOT(moveAllToRight()));
    connect(tblLeftItems, SIGNAL(clicked(QModelIndex)), this, SLOT(getCurrentLeftRow(QModelIndex)));
    connect(tblLeftItems, SIGNAL(activated(QModelIndex)), this, SLOT(getCurrentLeftRow(QModelIndex)));
    connect(tblRightItems, SIGNAL(clicked(QModelIndex)), this, SLOT(getCurrentRightRow(QModelIndex)));
    connect(tblRightItems, SIGNAL(activated(QModelIndex)), this, SLOT(getCurrentRightRow(QModelIndex)));
    connect(edtFilter,SIGNAL(textChanged(QString)),this, SLOT(EmptyFilter()));

}
void Slide::setMediaPath(QString path)
{
    mediaPath = path;
    LoadSlideShowFile();
    LoadCurrentItems();
    LoadSlideShowItems();
    ReadHtmlFile();
}

void Slide::closeEvent(QCloseEvent *event)
{
    if (QFile::exists(QString("%1/slide.htm").arg(QDir().tempPath())))
    {
        QFile::remove(QString("%1/slide.htm").arg(QDir().tempPath()));
    }
    event->accept();
}

void Slide::showPreview()
{
    LoadSlideShowFile();
    ReadHtmlFile();
}

void Slide::moveLeftToRight()
{
    if (rowLeft != -1)
    {
        sqlDeleteRecord_N("adjuntos_slide",
                        QString("id_adjunto = %1 AND id_slide = %2").
                                arg(rowLeft).
                                arg(slideId));

        if (sqlAutoCountRow("adjuntos_slide", QString("id_slide = %1").arg(slideId)) == 0){
            sqlDeleteRecord_N("slide",
                            QString("id_slide = %1 AND tipo = <&~%2~&>").
                                    arg(slideId).
                                    arg(slideType));
            slideId = 0;
        }

        LoadCurrentItems();
        LoadSlideShowItems();
        rowLeft = -1;
    }
}

void Slide::moveAllToRight()
{
    if (rowLeft != -1)
    {
        sqlDeleteRecord_N("adjuntos_slide",
                        QString("id_adjunto = %1 AND id_slide = %2").
                                arg(rowLeft).
                                arg(slideId));

        sqlDeleteRecord_N("slide",
                        QString("id_slide = %1 AND tipo = <&~%2~&>").
                                arg(slideId).
                                arg(slideType));

        slideId = 0;
        sqlCustomerQuery(sqlTmp);
        LoadCurrentItems();
        LoadSlideShowItems();
        rowLeft = -1;
    }
}

void Slide::moveRightToLeft()
{
    if (rowRight != -1)
    {
        if (slideId == 0)
        {
            slideId = sqlAutoIncremental("id_slide", "slide");

            sqlInsertRecord_N("slide",
                            "id_slide, id_pagina, tipo",
                            QString("%1, %2, <&~%3~&>").
                                    arg(slideId).
                                    arg(slidePage).
                                    arg(slideType));
        }

        slideOrder = sqlAutoCountRow("adjuntos_slide",
                                    QString("id_slide = %1").arg(slideId)) + 1;

        sqlInsertRecord_N("adjuntos_slide",
                        "id_adjunto, id_slide, orden",
                        QString("%1, %2, %3").
                                arg(rowRight).
                                arg(slideId).
                                arg(slideOrder));

        LoadCurrentItems();
        LoadSlideShowItems();
        rowRight = -1;
    }
}

void Slide::moveAllToLeft()
{
    if (rowRight != -1){
        if (slideId == 0)
        {
            slideId = sqlAutoIncremental("id_slide", "slide");

            sqlInsertRecord_N("slide",
                            "id_slide, id_pagina, tipo",
                            QString("%1, %2, <&~%3~&>").
                                    arg(slideId).
                                    arg(slidePage).
                                    arg(slideType));
        }
        QString sql = SQL_Listers;
        QSqlQuery *qry = new QSqlQuery;
        qry->exec(sql.arg(slideId));
        while (qry->next())
        {
            slideOrder = sqlAutoCountRow("adjuntos_slide",
                                        QString("id_slide = %1").arg(slideId)) + 1;

            sqlInsertRecord_N("adjuntos_slide",
                            "id_adjunto, id_slide, orden",
                            QString("%1, %2, %3").
                                    arg(qry->value(1).toInt()).
                                    arg(slideId).
                                    arg(slideOrder));
        }
        LoadCurrentItems();
        LoadSlideShowItems();
        rowRight = -1;
    }
}

void Slide::getCurrentLeftRow(QModelIndex index)
{
    QSqlRecord record = mdlLeftItems->record(index.row());
    rowLeft =  record.value("id_adjunto").toInt();

    btnDel->setEnabled(true);
    btnDelAll->setEnabled(true);
    btnAdd->setEnabled(false);
    btnAddAll->setEnabled(false);
}

void Slide::getCurrentRightRow(QModelIndex index)
{
    QSqlRecord record = mdlRightItems->record(index.row());
    rowRight =  record.value("id_adjunto").toInt();

    btnDel->setEnabled(false);
    btnDelAll->setEnabled(false);
    btnAdd->setEnabled(true);
    btnAddAll->setEnabled(true);
}

void Slide::FilterImage()
{
    if (!edtFilter->text().isEmpty())
        sqlFilter = QString(" AND adjuntos.descripcion LIKE '%%1%' ").arg(edtFilter->text());

    LoadSlideShowItems();

    if (mdlRightItems->query().first() == false)
        edtFilter->setStyleSheet(tr("background-color: rgb(255, 140, 140);"));
}

void Slide::EmptyFilter()
{
    btnDel->setEnabled(false);
    btnDelAll->setEnabled(false);
    btnAdd->setEnabled(false);
    btnAddAll->setEnabled(false);

    if (edtFilter->text().isEmpty()) sqlFilter.clear();

    LoadSlideShowItems();

    edtFilter->setStyleSheet(tr("background-color: rgb(255, 255, 255);"));
}

void Slide::LoadCurrentItems()
{
    QString sql = SQL_Candidates;
    tblLeftItems->close();
    mdlLeftItems->setQuery(sql.arg(slideId));
    tblLeftItems->setColumnWidth(0,tblLeftItems->width());
    tblLeftItems->hideColumn(1);
    tblLeftItems->hideColumn(2);
    tblLeftItems->show();
}

void Slide::LoadSlideShowItems()
{
    QString sql = SQL_Listers;
    tblRightItems->close();
    mdlRightItems->setQuery(sql.arg(slideId).arg(sqlFilter));
    tblRightItems->setColumnWidth(0,tblRightItems->width());
    tblRightItems->hideColumn(1);
    tblRightItems->show();
}

void Slide::LoadSlideShowFile()
{
    QFile htmlResFile(":/html/slide.html");
    htmlResFile.open(QIODevice::ReadOnly);

    htmlCode.clear();
    strLinks.clear();
    strItems.clear();

    QTextStream htmlStream(&htmlResFile);
    while (!htmlStream.atEnd()) htmlCode.append(QString("%1\n").arg(htmlStream.readLine()));

    QSqlQuery *qryImage = new QSqlQuery;
    qryImage->exec(SQL_Candidates.arg(slideId));
    QString swap,
            imgSrc = QString("%1").arg(mediaPath);

    int row = 0;
    while (qryImage->next())
    {
        swap = selectArea(htmlCode, "image-group");
        if (row == 0)
        {
            swap.replace("<item>", "item active", Qt::CaseSensitive);
            strLinks.append(QString("<li data-target=\"#slide\" data-slide-to=\"0\" class=\"active\"></li>"));
        }else{
            swap.replace("<item>", "item", Qt::CaseSensitive);
            strLinks.append(QString("<li data-target=\"#slide\" data-slide-to=\"%1\"></li>").arg(row));
        }

        swap.replace("<path>", imgSrc, Qt::CaseSensitive);
        swap.replace("<file>", qryImage->value(2).toString(), Qt::CaseSensitive);
        swap.replace("<alt-text>", qryImage->value(0).toString(), Qt::CaseSensitive);
        swap.replace("<text>", qryImage->value(0).toString(), Qt::CaseSensitive);

        swap.remove("<image-group>");
        swap.remove("</image-group>");

        strItems.append(swap+"\n");
        row++;
    }

    QString path = __APP_PATH__;
    if (path.contains("/bin")) path.remove("/bin");

    htmlCode.replace("<indicator>", strLinks, Qt::CaseSensitive);
    htmlCode.replace(selectArea(htmlCode,"image-group"), strItems, Qt::CaseSensitive);
    htmlCode.replace("<path>", path + "/qjseditor/adapters/slide/", Qt::CaseSensitive);

    if (QFile::exists(QString("%1/slide.htm").arg(QDir().tempPath())))
    {
        QFile::remove(QString("%1/slide.htm").arg(QDir().tempPath()));
    }

    QFile htmlFile(QString("%1/slide.htm").arg(QDir().tempPath()));
    htmlFile.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream outCode(&htmlFile);
    outCode.setCodec("UTF-8");
    outCode << htmlCode;
}

void Slide::ReadHtmlFile()
{
    if (slideId > 0)
        wbvSlideShow->setUrl(QUrl(QString("%1/slide.htm").arg(QDir().tempPath())));
    else
        wbvSlideShow->setUrl(QUrl(""));
}

void Slide::CreateWidtgets()
{
    btnAdd = new QToolButton;
    btnAddAll = new QToolButton;
    btnDel = new QToolButton;
    btnDelAll = new QToolButton;

    btnAdd->setIcon(QIcon(QPixmap(":/images/go-previous.png")));
    btnAddAll->setIcon(QIcon(QPixmap(":/images/go-first.png")));
    btnDel->setIcon(QIcon(QPixmap(":/images/go-next.png")));
    btnDelAll->setIcon(QIcon(QPixmap(":/images/go-last.png")));

    btnAdd->setToolTip(tr(""));
    btnAddAll->setToolTip(tr(""));
    btnDel->setToolTip(tr(""));
    btnDelAll->setToolTip(tr(""));

    btnDel->setEnabled(false);
    btnDelAll->setEnabled(false);
    btnAdd->setEnabled(false);
    btnAddAll->setEnabled(false);

    btnClose = new QPushButton(tr("Cerrar"));
    btnPreview = new QPushButton(tr("Actualizar vista previa"));

    edtFilter = new QLineEdit;

    btnFilter = new QToolButton;

    btnFilter->setIcon(QIcon(":/images/action_find.png"));
    btnFilter->setToolTip(tr("Buscar"));

    lblFilter = new QLabel(tr("Buscar:"));

    lblLeft = new QLabel(tr("Imagenes seleccionadas:"));
    lblRight = new QLabel(tr("Imagenes disponibles:"));

    mdlLeftItems = new QSqlQueryModel;
    mdlRightItems = new QSqlQueryModel;

    tblLeftItems = new QTableView;
    tblLeftItems->setToolTip(tr("Listado de imagenes seleccionadas."));
    tblLeftItems->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblLeftItems->setSortingEnabled(false);
    tblLeftItems->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblLeftItems->setSelectionMode(QAbstractItemView::SingleSelection);
    tblLeftItems->setShowGrid(false);
    tblLeftItems->setAlternatingRowColors(true);
    tblLeftItems->setWordWrap(true);
    tblLeftItems->verticalHeader()->hide();
    tblLeftItems->horizontalHeader()->hide();
    tblLeftItems->setModel(mdlLeftItems);

    tblRightItems = new QTableView;
    tblRightItems->setToolTip(tr("Listado de imagenes disponibles."));
    tblRightItems->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblRightItems->setSortingEnabled(false);
    tblRightItems->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblRightItems->setSelectionMode(QAbstractItemView::SingleSelection);
    tblRightItems->setShowGrid(false);
    tblRightItems->setAlternatingRowColors(true);
    tblRightItems->setWordWrap(true);
    tblRightItems->verticalHeader()->hide();
    tblRightItems->horizontalHeader()->hide();
    tblRightItems->setModel(mdlRightItems);

    wbvSlideShow = new QWebView;

    QSpacerItem *hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *hSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QVBoxLayout *lytButtons = new QVBoxLayout;
    lytButtons->addWidget(btnAddAll);
    lytButtons->addWidget(btnAdd);
    lytButtons->addWidget(btnDel);
    lytButtons->addWidget(btnDelAll);

    QVBoxLayout *lytLeft = new QVBoxLayout;
    lytLeft->addWidget(lblLeft);
    lytLeft->addWidget(tblLeftItems);

    QVBoxLayout *lytRight = new QVBoxLayout;
    lytRight->addWidget(lblRight);
    lytRight->addWidget(tblRightItems);

    QHBoxLayout *lytCentral = new QHBoxLayout;
    lytCentral->addLayout(lytLeft);
    lytCentral->addLayout(lytButtons);
    lytCentral->addLayout(lytRight);

    QHBoxLayout *lytFoother = new QHBoxLayout;
    lytFoother->addWidget(btnPreview);
    lytFoother->addItem(hSpacer);
    lytFoother->addWidget(btnClose);
    lytFoother->setAlignment(Qt::AlignRight);

    QHBoxLayout *lytFilter = new QHBoxLayout;
    lytFilter->addItem(hSpacer2);
    lytFilter->addWidget(lblFilter);
    lytFilter->addWidget(edtFilter);
    lytFilter->addWidget(btnFilter);

    QVBoxLayout *lytMain = new QVBoxLayout;
    lytMain->addWidget(wbvSlideShow);
    lytMain->addLayout(lytFilter);
    lytMain->addLayout(lytCentral);
    lytMain->addLayout(lytFoother);
    setLayout(lytMain);
}


