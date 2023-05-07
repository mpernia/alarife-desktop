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

#ifndef TEMPLATEVIEW_H
#define TEMPLATEVIEW_H

#include <QtGui>
#include <QtGui/QGraphicsPixmapItem>

#include "imageitem.h"

//----------------------------------------------------------------------------------------------
class TemplateView : public QGraphicsView{
Q_OBJECT
public:
    explicit TemplateView(QWidget *parent = 0, int width = 0, int height = 0);
    void refreshItems();
signals:
    void selectItems(QString);
protected:
    void mouseReleaseEvent(QMouseEvent *event);
private:
    void addItems();
    void getSelectionItem(ImageItem *image);
    void getNumRow(int size = 0);
    void resizeGraphic();
    QStringList lstAllName(QString path, QDir::Filter, QDir::SortFlags);

    QGraphicsScene *scene;

    int numRow,
        qrealH;

    QString strName,
            imgPath;
};

#endif // TEMPLATEVIEW_H
