/****************************************************************************
**
** Copyleft: 2014. Maikel Enrique Pernia Matos.
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

#ifndef SLIDESHOW_H
#define SLIDESHOW_H

#include <QDialog>
#include <QtGui>
#include <QWidget>
#include <QWebView>
#include <QSqlQueryModel>
#include <QModelIndex>

#include "database.h"
#include "alarife.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QToolButton;
class QWidget;
class QSqlQuery;
class QSqlQueryModel;
class QTableView;
class QFile;
QT_END_NAMESPACE

extern DbConnect AlarifeDb;
extern Book encyclopedia;

class SlideShow : public QDialog{
    Q_OBJECT

public:
    SlideShow(QWidget *parent = 0, int idKey = 0, char type = 0);
    ~SlideShow();

    //void setImagePath(QString);
private slots:
    void showPreview();
    void moveLeftToRight();
    void moveAllToRight();
    void moveRightToLeft();
    void moveAllToLeft();
    void getCurrentLeftRow(QModelIndex);
    void getCurrentRightRow(QModelIndex);

private:
    void CreateWidtgets();
    void LoadCurrentItems();
    void LoadSlideShowItems();
    void LoadSlideShowFile();



    QWebView *wbvSlideShow;

    QToolButton *btnAdd,
                *btnAddAll,
                *btnDel,
                *btnDelAll;

    QPushButton *btnClose,
                *btnPreview,
                *btnSave;

    QTableView *tblLeftItems,
               *tblRightItems;

    QSqlQueryModel *mdlRightItems,
                   *mdlLeftItems;

    int rowId, rowLeft, rowRight;
    QString strItems, strLinks, htmlCode, imgPath;
    char rowType;
};

#endif // SLIDESHOW_H
