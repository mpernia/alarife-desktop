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

#ifndef INSERTIMAGE_H
#define INSERTIMAGE_H

#include <QtGui>
#include <QDialog>

#include "alarife.h"
#include "database.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
class QTableView;
class QLabel;
class QToolButton;
QT_END_NAMESPACE

class InsertImage : public QDialog
{
Q_OBJECT
public:
    explicit InsertImage(QWidget *parent = 0);
protected:
    void closeEvent(QCloseEvent *event);
signals:
    void getSelectImage(QString);
    void compareSelectImage(int);
public slots:
    void insertMedia();
    void setModelIndex(QModelIndex index);
    void setExtImages(QString ext) {extImage = ext; }
    void setMediaPath(QString path) { mediaPath = path; }
    void InsertMediaExec(QModelIndex index);
    void FilterMediaExec();
    void EmptyFilter();
    void SqlExec(QString filter = "");
private:
    void OpenTable();

    QLabel *lblCaption,
           *lblText;

    QLineEdit *edtFilter;

    QToolButton *btnFilter,
                *btnInsert;

    int num;

    QString sql_qry,param, fileName;
    QTableView *tblGridView;
    QSqlQueryModel  *medias;
    QModelIndex modelIndex;
    QString extImage, mediaPath;
};

#endif // INSERTIMAGE_H
