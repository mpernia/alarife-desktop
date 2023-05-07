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

#include "../includes/alarife.h"
#include "../includes/imageitem.h"
#include "../includes/templateview.h"


TemplateView::TemplateView(QWidget *parent, int width, int height) : QGraphicsView(parent)
{
    qrealH = 200;
    imgPath = __APP_PATH__;
    numRow = 1;
    if (imgPath.contains("/bin")) imgPath.remove("/bin");

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 340, qrealH);

    setScene(scene);

    addItems();

    setMinimumSize(width, height);
    setMaximumSize(width, height);
}

void TemplateView::refreshItems()
{
    scene->clear();
    addItems();
}

QStringList TemplateView::lstAllName(QString path, QDir::Filter tmpFilter, QDir::SortFlags tmpFlags)
{
    QDir dir(imgPath + path);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QStringList stringlist = dir.entryList(tmpFilter, tmpFlags);
    return stringlist;
}

void TemplateView::getNumRow(int size)
{
    int c = size-2;

    if (c >= 2)
    {
        if (c % 2 == 0) numRow = c/2; else numRow = (c/2)+1;
        qrealH = 170 * numRow;
    }
}

void TemplateView::resizeGraphic()
{
    scene->setSceneRect(0, 0, 340, qrealH);
}

void TemplateView::addItems()
{
    int id = 0;
    int imageOffset = 160;
    int leftMargin = 40;
    int topMargin = 40;

    QStringList list = lstAllName(__DIR_TEMPLATE__, QDir::AllDirs, QDir::Name);

    getNumRow(list.size());
    resizeGraphic();

    for (int i=2; i < list.size(); i++)
    {

        QString strName = list.at(i);
        qDebug() << "item: " << strName << endl;

        ImageItem *image;
        QGraphicsTextItem *label;

        int columnOffset = ((id / 10) * 2);
        int x = ((id / numRow) * imageOffset) + leftMargin + columnOffset;
        int y = ((id % numRow) * imageOffset) + topMargin;

        image = new ImageItem(strName, QPixmap(imgPath + __DIR_TEMPLATE__ + strName + "/thumbnails.jpg"));

        image->setData(0, id);
        image->setPos(x, y);
        scene->addItem(image);

        label = scene->addText(strName);
        QPointF labelOffset((120 - label->boundingRect().width()) / 2, 120.0);
        label->setPos(QPointF(x, y) + labelOffset);

        id++;
    }
}

void TemplateView::mouseReleaseEvent(QMouseEvent *event)
{
    if (QGraphicsItem *item = itemAt(event->pos()))
    {
        if (ImageItem *image = qgraphicsitem_cast<ImageItem *>(item))
            getSelectionItem(image);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void TemplateView::getSelectionItem(ImageItem *image)
{
    emit selectItems(image->DirFiles());
}
