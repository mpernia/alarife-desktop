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

#ifndef STADISTIC_H
#define STADISTIC_H

#include <QDialog>
#include <QAbstractItemView>
#include <QFont>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QRect>
#include <QSize>
#include <QPoint>
#include <QWidget>
#include <QSqlQuery>

#include "alarife.h"
#include "database.h"
#include "diagram.h"

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QAbstractItemView;
class QItemSelectionModel;
QT_END_NAMESPACE


//----------------------------------------------------------------------------------------------
class Stadistic : public QDialog{
    Q_OBJECT
public:
    Stadistic(QWidget *parent = 0, Book *encyc = 0);
private:
    void clearFilter();

    QAbstractItemModel *model;
    QAbstractItemView *pieChart;
    QItemSelectionModel *selectionModel;
    QSqlQuery *qryTmp;
    QString extImagen,
            extSonido,
            extVideo,
            extXMedia,
            color1,
            color2,
            color3,
            color4,
            color5,
            color6,
            color7,
            color8,
            color9;
    Book *encyclopedia;
};
//----------------------------------------------------------------------------------------------
#endif
