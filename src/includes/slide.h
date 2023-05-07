#ifndef SLIDE_H
#define SLIDE_H

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


class Slide : public QDialog{
    Q_OBJECT

public:
    explicit Slide(QWidget *parent = 0, int page = 0, char type = 0);
    void setMediaPath(QString path = "");
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void showPreview();
    void moveLeftToRight();
    void moveAllToRight();
    void moveRightToLeft();
    void moveAllToLeft();
    void getCurrentLeftRow(QModelIndex);
    void getCurrentRightRow(QModelIndex);
    void FilterImage();
    void EmptyFilter();

private:
    void CreateWidtgets();
    void LoadCurrentItems();
    void LoadSlideShowItems();
    void LoadSlideShowFile();
    void ReadHtmlFile();

    QWebView *wbvSlideShow;

    QLineEdit *edtFilter;

    QToolButton *btnAdd,
                *btnAddAll,
                *btnDel,
                *btnDelAll,
                *btnFilter;

    QPushButton *btnClose,
                *btnPreview;

    QTableView *tblLeftItems,
               *tblRightItems;

    QSqlQueryModel *mdlRightItems,
                   *mdlLeftItems;

    QLabel *lblRight,
           *lblLeft,
           *lblFilter;

    char slideType;
    int rowLeft, rowRight, slideId, slidePage, slideOrder;
    QString strItems, strLinks, htmlCode, imgPath, sqlTmp, sqlFilter, mediaPath;
};



#endif // SLIDE_H
