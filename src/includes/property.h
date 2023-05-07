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

#ifndef PROPERTY2_H
#define PROPERTY2_H

#include <QDialog>
#include "database.h"
#include "templatedlg.h"
#include "alarife.h"
#include "mineditor.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QTextEdit;
class QTabWidget;
class QSqlQuery;
class QCheckBox;
QT_END_NAMESPACE


//----------------------------------------------------------------------------------------------
class TemplateTabPage : public QWidget{
    Q_OBJECT
public:
    TemplateTabPage(QWidget *parent = 0, int id = 0);
signals:
    void modifiedTemplate();
private slots:
    void TemplateEditExec();
    void TemplateCancelExec();
    void TemplatePostExec();
    void setSelection(QString);
private:
    void getTemplateValue();
    void setTemplateWriteMode(bool);
    void setTemplateClearCache();

    QLabel *lblImage,
           *lblDescribe;
    QPushButton *btnTEdit,
                *btnTPost,
                *btnTCancel;
    QString dirName,
            oldDirName;
    QSqlQuery *qryTemplate;
    int idEncyc;
};
//----------------------------------------------------------------------------------------------
class FilesTabPage : public QWidget{
    Q_OBJECT
public:
    FilesTabPage(QWidget *parent = 0, int id = 0);
signals:
    void modifiedExtension();
private slots:
    void FilesEditExec();
    void FilesCancelExec();
    void FilesPostExec();
private:
    void getFilesValue();
    void setFilesWriteMode(bool);
    void setFilesClearCache();

    QSqlQuery *qryFileType;
    QLabel *lblText,
           *lblImage,
           *lblSound,
           *lblVideo,
           *lblOther;
    QLineEdit *edtImage,
              *edtSound,
              *edtVideo,
              *edtOther;
    QPushButton *btnFEdit,
                *btnFPost,
                *btnFCancel;
    QString oldImage,
            oldSound,
            oldVideo,
            oldOther;
    int idEncyc;
};
//----------------------------------------------------------------------------------------------
class ExtraTabPage : public QWidget{
    Q_OBJECT
public:
    ExtraTabPage(QWidget *parent = 0, int id = 0);
signals:

private slots:
    void ExtrasEditExec();
    void ExtrasCancelExec();
    void ExtrasPostExec();
private:
    void getExtrasValue();
    void setExtrasWriteMode(bool);

    QLabel *lblInfo,
           *lblVideo,
           *lblSound;
    QCheckBox *chkVideo,
              *chkSound;
    QPushButton *btnEEdit,
                *btnEPost,
                *btnECancel;
    int idEncyc;

    bool oldSound;
    bool oldVideo;
};
//----------------------------------------------------------------------------------------------
class PropertyDlg : public QDialog{
    Q_OBJECT
public:
    explicit PropertyDlg(QWidget *parent = 0, Book **encyc = 0);
    ~PropertyDlg();
    QString *title;

private slots:
    void setTitle(QString str){ encyclopedia->setTitle(str); }
    void setTemplate(QString str){ encyclopedia->setTemplate(str); }
    void setExtImagen(QString str){ encyclopedia->setExtImagen(str); }
    void setExtSonido(QString str){ encyclopedia->setExtSonido(str); }
    void setExtVideo(QString str){ encyclopedia->setExtVideo(str); }
    void setExtXMedia(QString str){ encyclopedia->setExtXMedia(str); }
    void refreshValue();
protected:
    void closeEvent(QCloseEvent *event);
    Book *encyclopedia;
    TemplateTabPage *pageTemplate;
    FilesTabPage *pageFiles;
    ExtraTabPage *pageExtras;
private:

    QTabWidget *tabPage;

};
//----------------------------------------------------------------------------------------------
#endif

