#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>

#include "alarife.h"
#include "mineditor.h"
#include "database.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QAction;
class QToolButton;
class QToolBar;
class QSqlQuery;
QT_END_NAMESPACE

class HomePage : public QMainWindow{
Q_OBJECT
public:
    explicit HomePage(QWidget *parent = 0, Book **encyc = 0);
protected:
    void closeEvent(QCloseEvent *event);
signals:
    void updateEncInfo();
public slots:
    void saveDocument();
    void insertText(QMap<QString, QString>);
    void showXMedias();
    void showProperty();
    void showGallery();
    void showHelp();
    void showSlide();
    void loadValues();
    void modif();
    void spell();
private:
    QString originalDoc;
    QToolBar *tlbStandard;

    QAction *actProperty,
            *actInsertMedia,
            *actInsertSlide,
            *actSave,
            *actSpell,
            *actGallery,
            *actHelp;

    QLineEdit *edtTitle;

    QToolButton *btnProperty,
                *btnInsertMedia,
                *btnSave;

    Book *encyclopedia;

    MinEditor *miniEditor;

    int id_enc;

};

#endif // HOMEPAGE_H
