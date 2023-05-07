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

#include "../includes/imageitem.h"


ImageItem::ImageItem(QString dir, const QPixmap &pixmap, QGraphicsItem *parent,
                     QGraphicsScene *scene) : QGraphicsPixmapItem(pixmap, parent, scene)
{
    dirfiles = dir;
    setAcceptsHoverEvents(true);

    timeLine.setDuration(120);
    timeLine.setFrameRange(0, 120);

    connect(&timeLine, SIGNAL(frameChanged(int)), this, SLOT(setFrame(int)));
    connect(&timeLine, SIGNAL(finished()), this, SLOT(updateItemPosition()));

    adjust();
}

void ImageItem::hoverEnterEvent(QGraphicsSceneHoverEvent * /*event*/)
{
    timeLine.setDirection(QTimeLine::Forward);

    if (z != 1.0) {
        z = 1.0;
        updateItemPosition();
    }

    if (timeLine.state() == QTimeLine::NotRunning)
        timeLine.start();
}

void ImageItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * /*event*/)
{
    timeLine.setDirection(QTimeLine::Backward);
    if (z != 0.0)
        z = 0.0;

    if (timeLine.state() == QTimeLine::NotRunning)
        timeLine.start();
}

void ImageItem::setFrame(int frame)
{
    adjust();
    QPointF center = boundingRect().center();

    translate(center.x(), center.y());
    scale(1 + frame / 300.0, 1 + frame / 300.0);
    translate(-center.x(), -center.y());
}

void ImageItem::adjust()
{
    QMatrix matrix;
    matrix.scale(100/ boundingRect().width(), 100/ boundingRect().height());
    setMatrix(matrix);
}

QString ImageItem::DirFiles()
{
    return dirfiles;
}

void ImageItem::updateItemPosition()
{
    setZValue(z);
}


