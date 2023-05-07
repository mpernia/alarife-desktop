#ifndef GALLERYDLG_H
#define GALLERYDLG_H

#include <QtGui/QDialog>

#include "gallery.h"

QT_BEGIN_NAMESPACE
class QString;
class QWidget;
class QLabel;
class QToolButton;
class QLineEdit;
QT_END_NAMESPACE

class GalleryDlg : public QDialog
{
    Q_OBJECT

public:
    GalleryDlg(QWidget *parent = 0, QString = "");
    ~GalleryDlg();
private slots:
    void filterItems();
    void emptyFilter();

private:
    QLineEdit *edtFilter;
    QToolButton *btnFilter;
    QToolButton *btnSelectAll;
    QToolButton *btnClear;
    QLabel *lblFilter;
    Gallery *gallery;
    QString imagePath;
};

#endif // GALLERYDLG_H
