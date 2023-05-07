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

#ifndef MEDIA_H
#define MEDIA_H

#include <QWidget>
#include <QDialog>

#include "database.h"
#include "alarife.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
class QTableView;
class QLabel;
class QToolButton;
class QCheckBox;
QT_END_NAMESPACE


//----------------------------------------------------------------------------------------------
class TabPageMedia : public QWidget{
    Q_OBJECT
    public:
        TabPageMedia(QWidget *parent = 0);
        void setParam(QString);
        void setExtensionFiles(QString);
        void setUser(int id = 0);
        void setMediaPath(QString path) { mediaPath = path; }
    public slots:
        void setCaptionExec(QString caption = "");
        void SqlExec(QString filter = "", int mode = 0);
        void refreshGrid();
    private slots:
        void GetIndex(QModelIndex index);
        void EditRecordExec();
        void InsertRecordExec();
        void DeleteRecordExec();
        void FilterMediaExec();
        void EmptyFilter();
    protected:
        int idUser;
        QString mediaPath;
    private:
        void OpenTable();

        QLabel *lblCaption,
               *lblText;

        QLineEdit *edtFilter;

        QCheckBox *chkDeleteAll;

        QTableView *tblGridView;

        QSqlQueryModel  *medias;

        QToolButton *btnEdit,
                    *btnDelete,
                    *btnInsert,
                    *btnFilter;

        QModelIndex media_index;

        int id_adjunto,
            opt;

        QString sql_qry,
                ext, param;
};

//----------------------------------------------------------------------------------------------
class ImagePage : public TabPageMedia{
    Q_OBJECT
    public:
        ImagePage(TabPageMedia *parent = 0);
    private:
};
//----------------------------------------------------------------------------------------------
class SoundPage : public TabPageMedia{
    Q_OBJECT
    public:
        SoundPage(TabPageMedia *parent = 0);
    private:
};
//----------------------------------------------------------------------------------------------
class VideoPage : public TabPageMedia{
    Q_OBJECT
    public:
        VideoPage(TabPageMedia *parent = 0);
    private:
};
//----------------------------------------------------------------------------------------------
class OtherMediaPage : public TabPageMedia{
    Q_OBJECT
    public:
        OtherMediaPage(TabPageMedia *parent = 0);
    private:
};
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
class MediaOption : public QWidget{
    Q_OBJECT
public:
    MediaOption(QWidget *parent = 0, Book *encic = 0);
public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
    void showData(int);
private:
    void createIcons();
    void clearFilter();

    QListWidget *lstOptions;
    QStackedWidget *PageControl;

    Book *encyclopedia;

    QString extImagen,
            extSonido,
            extVideo,
            extXMedia;

    ImagePage *pageImage;
    SoundPage *pageSound;
    VideoPage *pageVideo;
    OtherMediaPage * pageOtyher;
};


#endif

