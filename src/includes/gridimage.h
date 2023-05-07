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

#ifndef GRIDIMAGE_H
#define GRIDIMAGE_H


#include <QModelIndex>
#include <QPixmap>
#include <QSize>
#include <QSqlRelationalDelegate>
#include <QPointer>
#include <QStyleOptionViewItem>

class GridImg : public QSqlRelationalDelegate{
    public:
        GridImg(QObject *parent, int type = 0, QString imgPath = "");
        void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
    private:
        int ext;
        QString path;
};

#endif // GRIDIMAGE_H
