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

#ifndef ORDERS_H
#define ORDERS_H

#include <QListWidget>
#include <QDialog>
#include <QList>

#include "database.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton; 
class QToolButton;
class QSqlQuery;
class QSqlQueryModel;
class QTableView;
class QListWidget;
QT_END_NAMESPACE

const QString SQL_LEVEL_SELECT = "SELECT nombre, orden, id_nivel FROM nivel WHERE "
                                 "padre = %1 AND enciclopedia = %2 ORDER BY orden";

const QString SQL_ARTICLE_SELECT = "SELECT nombre, orden, id_articulo FROM articulo WHERE "
                                   "nivel = %1 AND enciclopedia = %2 ORDER BY orden";

const QString SQL_LEVEL_UPDATE = "UPDATE nivel SET orden = %1 WHERE nombre = '%2' "
                                 "AND padre = %3 AND enciclopedia = %4";

const QString SQL_ARTICLE_UPDATE = "UPDATE articulo SET orden = %1 WHERE nombre = '%2' "
                                   "AND nivel = %3 AND enciclopedia = %4";

class OrdersDlg : public QDialog{
Q_OBJECT
public:
    OrdersDlg(QWidget *parent = 0, int father = 0, int id = 0, int mode = 0);
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void getActualRow(int);
    void changeUpExec();
    void changeDownExec();
    void changeFirstExec();
    void changeLastExec();
signals:
    void updateGrid(int);
private:
    void loadFieldList();
    void ActivedButton(bool actived = false);
    void refreshList();

    QSqlQueryModel *levels;

    QListWidget *listWidget;

    QList <QString>listOrder;

    QToolButton *btnUp,
                *btnDown,
                *btnFirst,
                *btnLast;

    QModelIndex id_level;

    int rowOld,
        rowNew,
        fatherLevel,
        imode;
    bool modified;

    int userId;

    QString sql;
};

#endif
