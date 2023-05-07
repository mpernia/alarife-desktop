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

#include <QtGui>
#include <QDebug>

#include "../includes/tableview.h"

TableView::TableView(QWidget *parent) : QTableView(parent)
{
}

void TableView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Down:
            selectRow(currentIndex().row() + 1);
            emit activated(currentIndex());
        break;
        case Qt::Key_Up:
            selectRow(currentIndex().row() - 1);
            emit activated(currentIndex());
        break;
        case Qt::Key_Backspace:
            emit backspacePressed();
        break;
        case Qt::Key_Return:
            emit returnPressed();
            emit returnPressed(currentIndex());
        break;
        case Qt::Key_Tab:
            emit tabPressed();
        break;
    }
    event->accept();
}

