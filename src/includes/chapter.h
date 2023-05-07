#ifndef CHAPTER_H
#define CHAPTER_H

#include <QMainWindow>

#include "baseditor.h"
#include "database.h"
#include "alarife.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QAction;
class QToolButton;
class QToolBar;
class QSqlQuery;
QT_END_NAMESPACE

class Chapter : public QMainWindow{
Q_OBJECT
public:
    explicit Chapter(QWidget *parent = 0, int father = -1, int index = -1, int opt = 0, Book **encyc = 0);
protected:
    void closeEvent(QCloseEvent *event);
signals:
    void updateGrid(int);
public slots:
    void saveDocument();
    void newChapter();
    // void insertText(QStringList words);
    void insertText(QMap<QString, QString>);
    void showXMedias();
    void showGallery();
    void showSlide();
    void loadValues();
    void modif();
    void spell();
private:
    QString originalDoc;
    QToolBar *tlbStandard;

    QAction *actInsertMedia,
            *actInsertSlide,
            *actSave,
            *actSpell,
            *actGallery,
            *actNew;

    QLineEdit *edtTitle;

    QToolButton *btnProperty,
                *btnInsertMedia,
                *btnSave,
                *btnNew;

    BasEditor *basEditor;

    int valueBook,
        valueFather,
        valueIndex,
        valueUser,
        valueOpt;
    Book *encyclopedia;
};

#endif // CHAPTER_H
