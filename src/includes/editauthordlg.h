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

#ifndef EDITAUTHORDLG_H
#define EDITAUTHORDLG_H

#include <QWidget>
#include <QDialog>
#include <QModelIndex>

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
class QToolButton;
class QComboBox;
QT_END_NAMESPACE

#include "alarife.h"

//----------------------------------------------------------------------------------------------
class EditAuthorDlg : public QDialog{
    Q_OBJECT
    public:
        EditAuthorDlg(QWidget *parent = 0, int index = 0, int opt = 0);
        void setCaption(QString caption = "");
        void setMediaPath(QString path) { mediaPath = path; }
        void setEncyclopediaId(int id) { idEnc = id; }
        void setImageExtension(QString str) { extension = str; }
        enum Mode {AddingMode, EditingMode};
        void setTypeUsr(int param1 = 0, int param2 = 0);
        void refreshvalue(QTableView **tbl, QString);
    public slots:
        void LoadPhotoExec();
        void PostValuesExec();
        void CancelValuesExec();
    private:
        void updateInterface(Mode mode);
        void RefreshGrid();
        void getValueRecord(QSqlQuery **, int id = 0);
        int getRollValue(int id = 0);

        QSqlQuery   *qry;

        QLabel      *lblCaption;

        QLineEdit   *edtName,
                    *edtMail,
                    *edtTitle,
                    *edtJob;

        QComboBox   *cmbRol;

        QToolButton *btnLoad;

        QPushButton *btnOk,
                    *btnCancel;

        QComboBox   *cmbUserType;

        QTableView  *grid;

        Mode        currentMode;
        int         option, roll, id, idEnc,
                    fotos,
                    colab,
                    team;

        QString fileName, sqlStr, mediaPath, extension;

        QPixmap photo;

        bool change_img;
};
//----------------------------------------------------------------------------------------------


#endif // EDITAUTHORDLG_H
