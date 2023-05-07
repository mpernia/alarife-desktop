#include <QtGui>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>

#include "../includes/gallery.h"
#include "../includes/sqlfunctions.h"

GalleryItem::GalleryItem(QWidget *parent, QString image, QString title) : QWidget(parent)
{
    m_image = image;
    m_title = title;
    m_select = false;


    lblImage = new QLabel;
    lblImage->setPixmap(QPixmap(m_image).scaled(QSize(150, 100), Qt::IgnoreAspectRatio, Qt::FastTransformation));
    lblImage->setAlignment(Qt::AlignLeft);
    lblImage->setStyleSheet("padding : 6px;");

    QString aux = m_title;

    lblTitle = new QLabel(tr("%1").arg(aux));
    lblTitle->setAlignment(Qt::AlignJustify | Qt::AlignTop);
    lblTitle->setWordWrap(true);

    QVBoxLayout *lytItem = new QVBoxLayout(this);
    lytItem->addWidget(lblImage);
    lytItem->addWidget(lblTitle);

    this->setLayout(lytItem);
    this->setToolTip(m_title);
}

void GalleryItem::mousePressEvent(QMouseEvent *event)
 {
     if (event->button() == Qt::LeftButton)
     {
         emit selected(QFileInfo(m_image).baseName());
         if (!m_select)
         {
             lblImage->setStyleSheet("background-color: rgb(85, 170, 255); padding : 6px;");
             m_select = true;
         }else{
             lblImage->setStyleSheet("padding : 6px;");
             m_select = false;
         }
     }
 }

void GalleryItem::setSelected()
{
    lblImage->setStyleSheet("background-color: rgb(85, 170, 255); padding : 6px;");
    m_select = true;
}

Gallery::Gallery(QWidget *parent) : QScrollArea(parent)
{
    predicate = "";
}

void Gallery::selectAll()
{
    int id = sqlAutoIncremental("id_galeria", "galeria");

    QSqlQuery *qryImages = new QSqlQuery;

    qryImages->exec(SQL_GalleryAdd.arg(predicate));

    qryImages->first();

    sqlInsertRecord_N("galeria",
                    "id_galeria, id_adjunto, id_enciclopedia",
                    QString("%1, %2, %3").arg(id++).
                    arg(qryImages->value(0).toInt()).arg(1));

    while (qryImages->next())
    {
        sqlInsertRecord_N("galeria",
                        "id_galeria, id_adjunto, id_enciclopedia",
                        QString("%1, %2, %3").arg(id++).
                        arg(qryImages->value(0).toInt()).arg(1));

    }

    delete qryImages;
    clear();
    prepare(predicate);
}

void Gallery::clearAll()
{
    sqlCustomerQuery(SQL_GalleryDel.arg(predicate));
    clear();
    prepare(predicate);
}

void Gallery::prepare(QString str)
{
    predicate = str;
    int i = 0;
    int x = 0;
    QString filename;

    lytMain = new QGridLayout;

    QSqlQuery *qryImages = new QSqlQuery;
    qryImages->exec(SQL_GalleryItem.arg(predicate));

    while (qryImages->next())
    {
        filename = QString("%1/%2").
                           arg(m_mediaPath).
                           arg(qryImages->value(0).toString());
        if (QFile(filename).exists())
        {
            GalleryItem *item = new GalleryItem(this, filename,qryImages->value(2).toString());

            if (qryImages->value(1).toBool())
            {
                item->setSelected();
                m_selection.append(qryImages->value(3).toString());
            }

            items.append(item);
            lytMain->addWidget(item, i, x);
            connect(item, SIGNAL(selected(QString)), this, SLOT(selectedItems(QString)));

            if (x == m_col -1) i++;

            x < m_col - 1 ? x++ : x = 0;
        }
    }

    widget = new QWidget;

    widget->setLayout(lytMain);

    this->setWidgetResizable(true);
    this->setWidget(widget);
}

void Gallery::selectedItems(QString item)
{
    if (m_selection.contains(item, Qt::CaseSensitive))
    {
        int i = m_selection.lastIndexOf(item);
        m_selection.removeAt(i);
        sqlDeleteRecord_N("galeria", QString(" (id_adjunto = %1) AND (id_enciclopedia = %2) ").arg(item).arg(1));
    }else{
        int id = sqlAutoIncremental("id_galeria", "galeria");
        m_selection.append(item);
        sqlInsertRecord_N("galeria",
                        "id_galeria, id_adjunto, id_enciclopedia",
                        QString("%1, %2, %3").arg(id).arg(item).arg(1));
    }
}

void Gallery::clear()
{
    for (int i = items.count(); i > 0; i--) {
        items.removeAt(i);
    }
    delete lytMain;
    delete widget;
}

