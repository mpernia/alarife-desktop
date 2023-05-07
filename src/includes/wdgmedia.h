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

#ifndef WDGMEDIA_H
#define WDGMEDIA_H


#include <QWidget>
#include <QDialog>

#include "alarife.h"
#include "database.h"
#include "webview.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
class QTableView;
class QLabel;
class QToolButton;
QT_END_NAMESPACE


//----------------------------------------------------------------------------------------------
class TabPageInsertMedia : public QWidget{
Q_OBJECT
public:
    TabPageInsertMedia(QWidget *parent = 0);
    void setWebView(WebView *);
    void setParam(QString);
    void setMediaPath(QString path){ mediaPath = path; }
    void refresh();

public slots:
    void setCaptionExec(QString caption = "");
    void SqlExec(QString filter = "", int mode = 0);
    void GetIndex(QModelIndex);
private slots:
    void FilterMediaExec();
    void EmptyFilter();
protected:
    QTableView *tblGridView;
    QSqlQueryModel  *medias;
    WebView *webView;
    QModelIndex media_index;
    QToolButton *btnInsert;
    QString mediaPath;
private:
    void OpenTable();

    QLabel *lblCaption,
           *lblText;

    QLineEdit *edtFilter;

    QToolButton *btnFilter;


    int id_adjunto, opt;

    QString sql_qry,param;

};
//----------------------------------------------------------------------------------------------
class InsertImagePage : public TabPageInsertMedia{
Q_OBJECT
public:
    InsertImagePage(TabPageInsertMedia *parent = 0);
public slots:
    void InsertMediaExec(QModelIndex index);
    void InsertImage();
};
//----------------------------------------------------------------------------------------------
class InsertSoundPage : public TabPageInsertMedia{
Q_OBJECT
public:
    InsertSoundPage(TabPageInsertMedia *parent = 0);
public slots:
    void InsertMediaExec(QModelIndex index);
    void InsertImage();
};
//----------------------------------------------------------------------------------------------
class InsertVideoPage : public TabPageInsertMedia{
Q_OBJECT
public:
    InsertVideoPage(TabPageInsertMedia *parent = 0);
public slots:
    void InsertMediaExec(QModelIndex index);
    void InsertImage();
};
//----------------------------------------------------------------------------------------------
class InsertOtherMediaPage : public TabPageInsertMedia{
Q_OBJECT
public:
    InsertOtherMediaPage(TabPageInsertMedia *parent = 0);
public slots:
    void InsertMediaExec(QModelIndex index);
    void InsertImage();
};

//----------------------------------------------------------------------------------------------
class InsertMediaPanel : public QWidget{
Q_OBJECT
public:
    InsertMediaPanel(QWidget *parent = 0, WebView *wv = 0, Book *encyc = 0);
public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
private:
    void createIcons();
    void clearFilter();

    QListWidget *lstOptions;
    QStackedWidget *PageControl;

    QString extImagen,
            extSonido,
            extVideo,
            extXMedia;

    InsertImagePage      *pageimage;
    InsertSoundPage      *pagesound;
    InsertVideoPage      *pagevideo;
    InsertOtherMediaPage *pageother;

    Book *encyclopedia;
};
//----------------------------------------------------------------------------------------------


#endif // WDGMEDIA_H
