/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** LAST MODFICATIONS BY: 2010. Maikel Enrique Pernia Matos
****************************************************************************/

#include <math.h>
#include <QtGui>

#ifndef M_PI
#define M_PI 3.1415927
#endif

#include "../includes/diagram.h"


Diagram::Diagram(QWidget *parent) : QAbstractItemView(parent)
{
    margin = 25;
    totalSize = 300;
    pieSize = totalSize - 2*margin;
    validItems = 0;
    totalValue = 0.0;
    rubberBand = 0;
}

void Diagram::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    QAbstractItemView::dataChanged(topLeft, bottomRight);

    validItems = 0;
    totalValue = 0.0;

    for (int row = 0; row < model()->rowCount(rootIndex()); ++row)
    {
        QModelIndex index = model()->index(row, 1, rootIndex());
        double value = model()->data(index).toDouble();
        if (value > 0.0)
        {
            totalValue += value;
            validItems++;
        }
    }
    viewport()->update();
}

QModelIndex Diagram::indexAt(const QPoint &point) const
{
    if (validItems == 0) return QModelIndex();

    int wx = point.x() + horizontalScrollBar()->value();
    int wy = point.y() + verticalScrollBar()->value();
    if (wx < totalSize)
    {
        double cx = wx - totalSize/2;
        double cy = totalSize/2 - wy;
        double d = pow(pow(cx, 2) + pow(cy, 2), 0.5);
        if (d == 0 || d > pieSize/2) return QModelIndex();

        double angle = (180 / M_PI) * acos(cx/d);
        if (cy < 0) angle = 360 - angle;

        double startAngle = 0.0;

        for (int row = 0; row < model()->rowCount(rootIndex()); ++row)
        {
            QModelIndex index = model()->index(row, 1, rootIndex());
            double value = model()->data(index).toDouble();
            if (value > 0.0) {
                double sliceAngle = 360*value/totalValue;
                if (angle >= startAngle && angle < (startAngle + sliceAngle))
                    return model()->index(row, 1, rootIndex());
                startAngle += sliceAngle;
            }
        }
    } else {
        double itemHeight = QFontMetrics(viewOptions().font).height();
        int listItem = int((wy - margin) / itemHeight);
        int validRow = 0;
        for (int row = 0; row < model()->rowCount(rootIndex()); ++row)
        {
            QModelIndex index = model()->index(row, 1, rootIndex());
            if (model()->data(index).toDouble() > 0.0) {
                if (listItem == validRow) return model()->index(row, 0, rootIndex());
                validRow++;
            }
        }
    }
    return QModelIndex();
}

QRect Diagram::itemRect(const QModelIndex &index) const
{
    if (!index.isValid()) return QRect();

    QModelIndex valueIndex;

    if (index.column() != 1) valueIndex = model()->index(index.row(), 1, rootIndex());
    else valueIndex = index;

    if (model()->data(valueIndex).toDouble() > 0.0)
    {
        int listItem = 0;
        for (int row = index.row()-1; row >= 0; --row) {
            if (model()->data(model()->index(row, 1, rootIndex())).toDouble() > 0.0) listItem++;
        }

        double itemHeight;

        switch (index.column()) {
        case 0:
            itemHeight = QFontMetrics(viewOptions().font).height();

            return QRect(totalSize,
                         int(margin + listItem*itemHeight),
                         totalSize - margin, int(itemHeight));
        case 1:
            return viewport()->rect();
        }

    }
    return QRect();
}

QRegion Diagram::itemRegion(const QModelIndex &index) const
{
    if (!index.isValid()) return QRegion();
    if (index.column() != 1) return itemRect(index);
    if (model()->data(index).toDouble() <= 0.0) return QRegion();

    double startAngle = 0.0;
    for (int row = 0; row < model()->rowCount(rootIndex()); ++row)
    {
        QModelIndex sliceIndex = model()->index(row, 1, rootIndex());
        double value = model()->data(sliceIndex).toDouble();
        if (value > 0.0)
        {
            double angle = 360*value/totalValue;
            if (sliceIndex == index)
            {
                QPainterPath slicePath;
                slicePath.moveTo(totalSize/2, totalSize/2);
                slicePath.arcTo(margin, margin, margin+pieSize, margin+pieSize,
                                startAngle, angle);
                slicePath.closeSubpath();
                return QRegion(slicePath.toFillPolygon().toPolygon());
            }
            startAngle += angle;
        }
    }
    return QRegion();
}

void Diagram::paintEvent(QPaintEvent *event)
{
    QItemSelectionModel *selections = selectionModel();
    QStyleOptionViewItem option = viewOptions();

    QBrush background = option.palette.base();
    QPen foreground(option.palette.color(QPalette::WindowText));

    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(event->rect(), background);
    painter.setPen(foreground);

    QRect pieRect = QRect(margin, margin, pieSize, pieSize);
    
    if (validItems > 0)
    {
        painter.save();
        painter.translate(pieRect.x() - horizontalScrollBar()->value(),
                          pieRect.y() - verticalScrollBar()->value());
        painter.drawEllipse(0, 0, pieSize, pieSize);
        double startAngle = 0.0;
        int row;
        for (row = 0; row < model()->rowCount(rootIndex()); ++row)
        {
            QModelIndex index = model()->index(row, 1, rootIndex());
            double value = model()->data(index).toDouble();
            if (value > 0.0)
            {
                double angle = 360*value/totalValue;
                QModelIndex colorIndex = model()->index(row, 0, rootIndex());
                QColor color = QColor(model()->data(colorIndex, Qt::DecorationRole).toString());
                if (currentIndex() == index) painter.setBrush(QBrush(color, Qt::Dense4Pattern));
                else if (selections->isSelected(index)) painter.setBrush(QBrush(color, Qt::Dense3Pattern));
                else painter.setBrush(QBrush(color));
                painter.drawPie(0, 0, pieSize, pieSize, int(startAngle*16), int(angle*16));
                startAngle += angle;
            }
        }
        painter.restore();
        int keyNumber = 0;
        for (row = 0; row < model()->rowCount(rootIndex()); ++row)
        {
            QModelIndex index = model()->index(row, 1, rootIndex());
            double value = model()->data(index).toDouble();
            if (value > 0.0) {
                QModelIndex labelIndex = model()->index(row, 0, rootIndex());
                QStyleOptionViewItem option = viewOptions();
                option.rect = visualRect(labelIndex);
                if (selections->isSelected(labelIndex)) option.state |= QStyle::State_Selected;
                if (currentIndex() == labelIndex) option.state |= QStyle::State_HasFocus;
                itemDelegate()->paint(&painter, option, labelIndex);
                keyNumber++;
            }
        }
    }
}

QRect Diagram::visualRect(const QModelIndex &index) const
{
    QRect rect = itemRect(index);
    if (rect.isValid())
        return QRect(rect.left() - horizontalScrollBar()->value(),
                     rect.top() - verticalScrollBar()->value(),
                     rect.width(), rect.height());
    else
        return rect;
}

bool Diagram::edit(const QModelIndex & /*index*/, EditTrigger /*trigger*/, QEvent * /*event*/)
{
    return true;
}

bool Diagram::isIndexHidden(const QModelIndex & /*index*/) const
{
    return true;
}

int Diagram::horizontalOffset() const
{
    return 0;
}

void Diagram::mousePressEvent(QMouseEvent * /*event*/)
{
}

void Diagram::mouseMoveEvent(QMouseEvent * /*event*/){
}

void Diagram::mouseReleaseEvent(QMouseEvent * /*event*/){
}

QModelIndex Diagram::moveCursor(QAbstractItemView::CursorAction, Qt::KeyboardModifiers){
    QModelIndex mod;
    return mod;
}

int Diagram::rows(const QModelIndex & /*index*/) const
{
    return 0;
}
void Diagram::rowsInserted(const QModelIndex & /*parent*/, int /*start*/, int /*end*/){
}
void Diagram::rowsAboutToBeRemoved(const QModelIndex & /*parent*/, int /*start*/, int /*end*/){
}
void Diagram::scrollContentsBy(int /*dx*/, int /*dy*/){
}
void Diagram::scrollTo(const QModelIndex & /*index*/, ScrollHint){
}
void Diagram::setSelection(const QRect & /*rect*/, QItemSelectionModel::SelectionFlags /*command*/){
}
void Diagram::updateGeometries(){
}
int Diagram::verticalOffset() const{
    return 0;
}
QRegion Diagram::visualRegionForSelection(const QItemSelection & /*selection*/) const{
    QRegion reg;
    return reg;
}

