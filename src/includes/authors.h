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

#ifndef AUTHORS_H
#define AUTHORS_H

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
class QToolButton;
class QComboBox;
QT_END_NAMESPACE

#include "alarife.h"
#include "editauthordlg.h"
#include "selectauthordlg.h"

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class TabPageAuthor : public QWidget{
    Q_OBJECT
    public:
        TabPageAuthor(QWidget *parent = 0/*, int idUser = 0*/);
        void setParam(QString);        
        void setUser(int);
    public slots:
        void setCaptionExec(QString caption = "");
        void setMediaPath(QString path);// { mediaPath = path; }
        void SqlExec(QString filter = "");
        void setExtension(QString str= "") { ext = str; }
        void CustomBottomsPanel(int x=-1);
        void updateRows();
    private slots:
        void EditRecordExec();
        void InsertRecordExec();
        void DeleteRecordExec();
        void SelectRecordExec();
        void getIdUserValue(QModelIndex index);
        void FilterMediaExec();
        void EmptyFilter();
    protected:
        int userId;
        QString mediaPath;
private:
    void OpenTable();    
    void setTypeUsr(int param1 = 0, int param2 = 0);

    QLabel          *lblCaption,
                    *lblText;

    QLineEdit       *edtFilter;

    QTableView      *tblGridView;

    QSqlQueryModel  *authors;

    QToolButton     *btnEdit,
                    *btnDelete,
                    *btnSelect,
                    *btnInsert,
                    *btnFilter;

    QModelIndex authors_index;

    SelectAuthorDlg *seldlg;

    EditAuthorDlg   *editDlg,
                    *insertDlg;

    int             id_autor,
                    x,
                    y,
                    mode_del;
    QString         param, ext;
};
//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class ActorPage : public TabPageAuthor{
Q_OBJECT
public:
    ActorPage(TabPageAuthor *parent = 0/*, int idUser = 0*/);
private:
};
//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class AuthorPage : public TabPageAuthor{
Q_OBJECT
public:
    AuthorPage(TabPageAuthor *parent = 0/*, int idUser = 0*/);
private:
};
//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class ColaborationPage : public TabPageAuthor{
Q_OBJECT
public:
    ColaborationPage(TabPageAuthor *parent = 0/*, int idUser = 0*/);
private:

};
//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class TeamPage : public TabPageAuthor{
Q_OBJECT
public:
    TeamPage(TabPageAuthor *parent = 0/*, int idUser = 0*/);
private:
};
//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class AuthorOption : public QDialog{
Q_OBJECT
public:
    AuthorOption(QWidget *parent = 0, Book **encyc = 0);
    int set_Index();
public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
private:
    void createIcons();

    QListWidget    *lstOptions;
    QStackedWidget *PageControl;
    int            id_usuario;
    Book *encyclopedia;

    ActorPage *pageActor;
    AuthorPage * pageAuthor;
    ColaborationPage *pageColaboration;
    TeamPage *pageTeam;
};

#endif
