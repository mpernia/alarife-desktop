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

#include <QSqlRelationalDelegate>
#include <QtGui>
#include <QFile>
#include <QSqlRecord>
#include "../includes/gridimage.h"

#include <iostream>
using namespace std;

GridImg::GridImg(QObject *parent, int type, QString imgPath) : QSqlRelationalDelegate(parent)
{
    ext = type;
    path = imgPath;
}

void GridImg::paint(QPainter *painter, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    if (index.column() != 1)
    {
        QStyleOptionViewItemV3 opt = option;
        opt.rect.adjust(0, 0, -1, -1);
        QSqlRelationalDelegate::paint(painter, opt, index);
    } else {
        const QAbstractItemModel *model = index.model();
        QString tmpName,
                realName = model->data(index, Qt::DisplayRole).toString();
        QPixmap thumbnails;
        switch (ext)
        {
            case 0: tmpName = ":/images/no_user.png";
                break;
            case 1: tmpName = ":/images/no_image.png";
                break;
            case 2: tmpName =":/images/no_sound.png";
                break;
            case 3: tmpName =":/images/no_video.png";
                break;
            case 4: tmpName =":/images/no_xmedia.png";
                break;
        }
        if (!QFile(path + "/" + realName).exists() || ext > 1)
            thumbnails = QPixmap(tmpName);
        else
            thumbnails = QPixmap(path + "/" + realName);

        int height = 78;
        int x = option.rect.x();
        int y = option.rect.y() + (option.rect.height() / 2) - (height / 2);
        painter->drawPixmap(x, y, thumbnails.scaled(QSize(height,height),
                                              Qt::IgnoreAspectRatio,
                                              Qt::FastTransformation));
    }
}
