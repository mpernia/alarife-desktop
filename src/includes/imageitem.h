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

#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QtGui>
#include <QtCore>
#include <QtGui/QGraphicsPixmapItem>


//----------------------------------------------------------------------------------------------
class ImageItem : public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    explicit ImageItem(QString dir, const QPixmap &pixmap,
                       QGraphicsItem *parent = 0,  QGraphicsScene *scene = 0);
    void adjust();
    QString DirFiles();
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
signals:

private slots:
    void setFrame(int frame);
    void updateItemPosition();
private:
    QTimeLine timeLine;
    QString dirfiles;
    double z;
};

#endif // IMAGEITEM_H
