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

#ifndef SELECTAUTHORDLG_H
#define SELECTAUTHORDLG_H

#include <QWidget>
#include <QDialog>
#include <QModelIndex>

#include "alarife.h"

QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
class QTableView;
class QSqlQueryModel;
class QSqlQuery;
class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;
QT_END_NAMESPACE


//----------------------------------------------------------------------------------------------
class SelectAuthorDlg : public QDialog{
    Q_OBJECT
    public:
        SelectAuthorDlg(QWidget *parent = 0, int id = 0);
        void setTypeUsr(int param1 = 0, int param2 = 0);
        void refreshvalue(QTableView **tbl, QString);
    private slots:
        void selectAuthor(QString name = "");
        void PostExec();
    private:
        void RefreshGrid();
        int setFirstValue();

        QLabel      *lblText;
        QComboBox   *cmbList;
        QPushButton *btnOk,
                    *btnCancel;
        QSqlQuery   *qryAuthor;
        QTableView  *grid;
        int         mode,
                    id_autor,
                    colab,
                    team,
                    idEnc;
        QString sqlStr;
};
//----------------------------------------------------------------------------------------------


#endif // SELECTAUTHORDLG_H
