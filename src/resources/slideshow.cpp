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
#include <QtSql>
#include <QSqlQuery>
#include <QSpacerItem>


#include "includes/slideshow.h"
#include "includes/sqlfunctions.h"

//----------------------------------------------------------------------------------------------
SlideShow::SlideShow(QWidget *parent, int idKey, char type) : QDialog(parent){
    rowId = idKey;
    rowType = type;
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
    Qt::WindowModality modality = Qt::WindowModal;
    QWidget::setWindowModality(modality);

    setWindowTitle(tr("Imágenes deslizantes"));
    setMinimumSize(500, 400);

    CreateWidtgets();
    LoadCurrentItems();
    LoadSlideShowItems();
    LoadSlideShowFile();

    connect(btnPreview, SIGNAL(clicked()), this, SLOT(showPreview()));
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(moveRightToLeft()));
    connect(btnAddAll, SIGNAL(clicked()), this, SLOT(moveAllToLeft()));
    connect(btnDel, SIGNAL(clicked()), this, SLOT(moveLeftToRight()));
    connect(btnDelAll, SIGNAL(clicked()), this, SLOT(moveAllToRight()));
    connect(tblLeftItems, SIGNAL(clicked(QModelIndex)), this, SLOT(getCurrentLeftRow(QModelIndex)));
    connect(tblLeftItems, SIGNAL(activated(QModelIndex)), this, SLOT(getCurrentLeftRow(QModelIndex)));
    connect(tblRightItems, SIGNAL(clicked(QModelIndex)), this, SLOT(getCurrentRightRow(QModelIndex)));
    connect(tblRightItems, SIGNAL(activated(QModelIndex)), this, SLOT(getCurrentRightRow(QModelIndex)));

}
//----------------------------------------------------------------------------------------------
SlideShow::~SlideShow(){

}
//----------------------------------------------------------------------------------------------
void SlideShow::showPreview(){
    QSqlQuery *qryImage = new QSqlQuery;
    qryImage->exec(QString(""));

    if (qryImage->isValid()){
        qryImage->first();
        for (int i = 0; i < qryImage->numRowsAffected(); i++) {
            if (qryImage->numRowsAffected() == 0){
                strItems.append(QString("<div class=\"item active\"><img src=\"%1/%2\" alt=\"%3\">"
                                        "<div class=\"carousel-caption\"><p>%4</p></div></div>").
                                arg("path").
                                arg("image").
                                arg("descr").
                                arg("text"));
                strLinks.append(QString("<li data-target=\"#slide\" data-slide-to=\"0\" class=\"active\"></li>"));
            }else{
                strItems.append(QString("<div class=\"item\"><img src=\"%1/%2\" alt=\"%3\">"
                                        "<div class=\"carousel-caption\"><p>%4</p></div></div>").
                                arg("path").
                                arg("image").
                                arg("descr").
                                arg("text"));
                strLinks.append(QString("<li data-target=\"#slide\" data-slide-to=\"%1\"></li>").arg(i));
            }
            qryImage->next();
        }
    }
}
//----------------------------------------------------------------------------------------------
void SlideShow::moveLeftToRight(){
    if (rowRight != -1){
        rowLeft = -1;
    }
}
//----------------------------------------------------------------------------------------------
void SlideShow::moveAllToRight(){    
    if (rowRight != -1){
        sqlDeleteRecord("adjuntos_slide", "adjuntos_slide.*", QString("id_slide = %1").arg(1));
        sqlDeleteRecord("slide", "slide.*", QString("%1 AND %2").arg(1).arg(2));
        //DELETE adjuntos_slide.* FROM adjuntos_slide WHERE id_slide = %1;
        //DELETE slide.* FROM slide WHERE id_pagina = %1 and tipo='%2');
        rowLeft = -1;
    }
}
//----------------------------------------------------------------------------------------------
void SlideShow::moveRightToLeft(){
    if (rowRight != -1){
        int row = sqlGetIdValue("slide", "id_slide",
                                QString(" id_pagina = %1 AND tipo = '%2' ").arg(rowId).arg(rowType));
        if (row < 1){
            int row = sqlAutoIncremental("id_slide", "slide");
            sqlInsertRecord("slide", "id_slide, id_pagina, tipo", QString("%1, %2, '%3'").arg(row).arg(rowId).arg(rowType));
        }else{
        }

        rowRight = -1;
    }
}
//----------------------------------------------------------------------------------------------
void SlideShow::moveAllToLeft(){
    if (rowRight != -1){
        //sqlDeleteRecord("adjuntos_slide", "fields", "value");
        rowRight = -1;
    }
}
//----------------------------------------------------------------------------------------------
void SlideShow::getCurrentLeftRow(QModelIndex index){
    QSqlRecord record = mdlLeftItems->record(index.row());
    rowLeft =  record.value("id_adjunto").toInt();
    qDebug() << "izquierdo " << rowLeft;
}
//----------------------------------------------------------------------------------------------
void SlideShow::getCurrentRightRow(QModelIndex index){
    QSqlRecord record = mdlRightItems->record(index.row());
    rowRight =  record.value("id_adjunto").toInt();
    qDebug() << "derecho" << rowRight;
}
//----------------------------------------------------------------------------------------------
void SlideShow::LoadCurrentItems(){
    QString sql = SQL_Candidates;
    tblLeftItems->close();
    mdlLeftItems->setQuery(sql.arg(rowId));
    tblLeftItems->setColumnWidth(0,tblLeftItems->width());
    tblLeftItems->hideColumn(1);
    tblLeftItems->show();
}
//----------------------------------------------------------------------------------------------
void SlideShow::LoadSlideShowItems(){
    QString sql = SQL_Listers;
    tblRightItems->close();
    mdlRightItems->setQuery(sql.arg(rowId));
    tblRightItems->setColumnWidth(0,tblRightItems->width());
    tblRightItems->hideColumn(1);
    tblRightItems->show();
}
//----------------------------------------------------------------------------------------------
void SlideShow::LoadSlideShowFile(){
    QFile htmlFile(":/html/slide.html");
    htmlFile.open(QIODevice::ReadOnly);

    QTextStream htmlStream(&htmlFile);
    while (!htmlStream.atEnd()) qDebug() << htmlStream.readLine(); // htmlCode.append(htmlStream.readLine() + "\n");
    qDebug() << "FIN";
    htmlCode.replace("<PATH>", "/home/maikel/PRUEBAS/slideshow/tmp/", Qt::CaseSensitive);
}
//----------------------------------------------------------------------------------------------
void SlideShow::CreateWidtgets(){
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

    btnClose = new QPushButton(tr("Cerrar"));
    btnPreview = new QPushButton(tr("Actualizar vista previa"));
    btnSave = new QPushButton(tr("Guardar"));

    mdlLeftItems = new QSqlQueryModel;
    mdlRightItems = new QSqlQueryModel;


    tblLeftItems = new QTableView;
    tblLeftItems->setToolTip(tr("Listado de temas."));
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
    tblRightItems->setToolTip(tr("Listado de temas."));
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

    qDebug() << htmlCode;
    wbvSlideShow = new QWebView;
    wbvSlideShow->setHtml(htmlCode);

    //wbvSlideShow->setUrl(QUrl("file:///home/maikel/PRUEBAS/slideshow/tmp/slide.html"));

    QSpacerItem *hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QVBoxLayout *lytButtons = new QVBoxLayout;
    lytButtons->addWidget(btnAddAll);
    lytButtons->addWidget(btnAdd);
    lytButtons->addWidget(btnDel);
    lytButtons->addWidget(btnDelAll);

    QHBoxLayout *lytCentral = new QHBoxLayout;
    lytCentral->addWidget(tblLeftItems);
    lytCentral->addLayout(lytButtons);
    lytCentral->addWidget(tblRightItems);

    QHBoxLayout *lytFoother = new QHBoxLayout;
    lytFoother->addWidget(btnPreview);
    lytFoother->addItem(hSpacer);
    lytFoother->addWidget(btnSave);
    lytFoother->addWidget(btnClose);
    lytFoother->setAlignment(Qt::AlignRight);

    QVBoxLayout *lytMain = new QVBoxLayout;
    lytMain->addWidget(wbvSlideShow);
    lytMain->addLayout(lytCentral);
    lytMain->addLayout(lytFoother);
    setLayout(lytMain);
}
//----------------------------------------------------------------------------------------------
//
