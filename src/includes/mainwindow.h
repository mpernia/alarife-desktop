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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QProgressDialog>
#include <QMainWindow>
#include <QModelIndex>
#include <QQueue>

#include "alarife.h"
#include "login.h"

#include "rootdlg.h"
#include "import.h"
#include "editor.h"
#include "itemid.h"
#include "stackid.h"
#include "dlgwaitproc.h"
#include "tableview.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QLabel;
class QLineEdit;
class QTextEdit;
class QPushButton;
class QToolButton;
class QRadioButton;
class QWidget;
class QDockWidget;
class QSqlQuery;
class QSqlQueryModel;
class QTableView;
class QFile;
class QProgressBar;
class QWebView;
class QTimer;
QT_END_NAMESPACE

//----------------------------------------------------------------------------------------------
class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow() { delete AlarifeDb; delete encyclopedia; }
    void setLoatFile(QString);
protected:
    void closeEvent(QCloseEvent *event);    
    void createContextMenu();
private slots:
    void UpdateBookInfo();
    void NewExec();
    void OpenExec();
    void fileRecentOpen();
    void clearRecents();
    void StadisticExec();
    void HomePageExec();
    void PropertyExec();
    void AuthorsExec();
    void MediaExec();

    void ToolBarSearchExec();
    void ToolBarSearchShow();
    void ToolBarSearchHide();
    void ClearSearch();
    void EmptySearch();
    void resetSearch(bool);

    void ChapterAddExec();
    void ChapterEditExec();
    void ChapterDeleteExec();
    void ChapterDeleteAll();
    void ChapterOrderExec();
    void ChapterOrder(bool);
    void ChapterDeleteMsg(bool);
    void ChapterMoveExec();
    void ChapterMoveExecAll();

    void ArticleAddExec();
    void ArticleEditExec();
    void ArticleDeleteExec();
    void ArticleDeleteAll();
    void ActArticlePropertyExec();
    void ArticleOrderExec();
    void ArticleMoveExec();
    void ArticleMoveAllExec();
    void ArticleOrder(bool);
    void showEditor(ItemId);
    void MoveElementExec();

    void BookExportExec();
    void NewBookImportExec();
    void NewBookImportPrepare();
    void AddBookImportExec();
    void MergeBookImportExec();
    void Exec_BookExport();
    void Exec_NewBookImport();
    void Exec_AddBookImport();
    void Exec_MergeBookImport();

    void GeneratedHtmlExec();
    void Exec_Generation();
    void BuilderAlarifeExec();
    void RestructuringData();

    void ContentsExec();
    void CheckedBoss();
    void ConfigurationExec(bool);
    void AboutExec();

    void ShowDetails(QModelIndex index);
    void ShowArticle(QModelIndex index);
    void updateTitle();
    void changeUser();
    void MasterOnDoubleClick(QModelIndex);
    void FirstFatherLevel();
    void PriorFatherLevel();
    void editClose();
    void changeLevelImage();
    void openConection(bool);
    void refreshGrids(int);
    void changeChapterFather(int);
    void setFocusMasterView();
    void setFocusDetailView();
    void refreshTraceBar(int);


    void adminUser();
    void adminTemplate();
    void adminStyles();

private:
    void CreateActions();
    void CreateMenus();
    void CreateToolBars();
    void CreateStatusBar();
    void ReadSettings();
    void WriteSettings();
    void CreatePanels();
    void ClearBookInfo();
    void BookImportCommon();
    void OpenArticleList(int father = 0, QString find ="");
    void OpenRelationTable(int father = 0, QString find ="");
    void AdjustHeader();
    void EnabledAccion(bool actived = false);
    void EnabledLevelsEdit(bool actived = false);
    void EnabledArticlesEdit(bool actived = false);
    void ShowLoginDlg();
    void ProgressPanelActive(bool active = false);
    void UpdateProgressPanel(int value = 0, QString text = "");
    void UpdatePanelCaption(int row = 0);

    QString selectImage(int);
    void loadImage(QString);
    void openTraceBar();
    void closeTraceBar();
    void PushTraceBar(int);

    void updateRecentFileActions();
    void writeRecents();

    bool isOwner(QSqlRecord);
    enum { MaxRecentFiles = 6 };

    void GUI_DEBUG(QString msg){QMessageBox::critical(0, "Debugger", msg, QMessageBox::Ok);}

    void wait(int interval)
    {

        QTimer timer;
        timer.setInterval(interval);
        timer.setSingleShot(true);
        timer.start();
    }

    QString curFile,
            StrSql,
            DirPath,
            windowtitle;

    int minValue,
        maxValue,
        id_master,
        current_master,
        current_detail,
        id_detail,
        impMode,
        visibleTraceBar;

    QMenu *mnBook,
          *menuRecent,
          *mnEdit,
          *mnChapter,
          *mnArticle,
          *mnHelp,
          *mnImport,
          *mnTool;

    QToolBar *tlbBook,
             *tlbEdit,
             *tlbTool,
             *tlbHelp,
             *tlbFind,
             *tlbTrace;

    QAction *actNew,
            *actOpen,
            *actRecent[MaxRecentFiles],
            *actClearRecent,
            *actStadistic,
            *actHomePage,
            *actProperty,
            *actAuthor,
            *actMedia,
            *actExit,
            *actSearch,
            *actCloseFind,
            *actChapter,
            *actChapterAdd,
            *actChapterEdit,
            *actChapterDelete,
            *actChapterDelAll,
            *actChapterOrder,
            *actChapterByName,
            *actArticleAdd,
            *actArticleEdit,
            *actArticleDelete,
            *actArticleDelAll,
            *actArticleProperty,
            *actArticleOrder,
            *actArticleByName,
            *actMoveElement,

            *actBookExport,
            *actAddBookImport,
            *actMergeBookImport,
            *actNewBookImport,
            *actGenerated,
            *actBenchmark,
            *actBuilder,

            *actContents,
            *actConfiguration,
            *actAbout,

            *actAdminUser,
            *actAdminTemplate,
            *actAdminStyles;

    QDockWidget *dckMaster,
                *dckDetail,
                *dckTreeView;

    QWidget *cntDetail,
            *cntTreeView,
            *cntMaster,
            *progresspanel;

    QWebView *previewMaster;

    QTextEdit *mmMasterText;

    QLabel *lblInform,
           *prgLbl,
           *lblTrace;

    QLineEdit   *edtSearch;

    QToolButton *btnFirst,
                *btnPrior,
                *btnClear;

    QPushButton *btnSearch,
                *btnImage;


    QRadioButton *rbtMaster,
                 *rbtDetail,
                 *rbtAuthor;

    TableView *DetailView,
              *MasterView;

    QSqlQueryModel *master,
                   *detail;

    QSqlQuery *qryTemp;

    QModelIndex MasterId,
                DetailId;

    QProgressBar *prgBar;

    QTimer *timer;

    QPixmap pixmap;

    QStack<QString>trace;

    bool isFull;

    QString strTrace;

    StackId *stack;
    dbImport *impdb;

    Editor *edit;

    dlgWaitProc *dlgwait;

    QList <Reg> lstLevel;
    QList <QString> lstName;

    DbConnect *AlarifeDb;
    Book *encyclopedia;
};
//----------------------------------------------------------------------------------------------
#endif
