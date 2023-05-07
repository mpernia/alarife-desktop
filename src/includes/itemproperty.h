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

#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui>
#include <QDialog>
#include "itemid.h"

class ItemProperty : public QDialog{
    Q_OBJECT

public:
    ItemProperty(ItemId i, QWidget *parent = 0);
    ~ItemProperty();
protected:
    void closeEvent(QCloseEvent *event);
signals:
    void inserted(ItemId i);
    void refreshValues();
    void updateGridDetail(int);

private slots:
    void save();
    void insertArticles();
    void insertAuthors();
    void deleteAuthor();
    void deleteArticles();
    void currentIndexChanged(const QString &arg1);
    void enableArticles();
    void filterMediaExec();
    void emptyFilter();
    void updateList();

private:
    void createWidgets();
    void setUpdateInterface(bool);
    void getAllValues();
    void getAllListSelectedValues();
    void getAllListAvailableValues();
    void getListValues();
    void getListWidgetValues(QString, QListWidget *);
    void hideFilterToolBar();

    int usuario;
    void insertNewAuthors();
    void insertNewItems();

    ItemId itemId;

    QTabWidget *tabWidget;
    QWidget *tab;
    QLineEdit *edtTitle;
    QLineEdit *edtSource;
    QPlainTextEdit *mmKeywords;
    QPlainTextEdit *mmSummary;
    QLabel *label,
           *label_2,
           *label_3,
           *label_4,
           *label_5,
           *label_6,
           *label_7,
           *label_8,
           *label_9,
           *label_10;
    QLabel *lblNombrePropietario;
    QComboBox *cmbUser;
    QDateTimeEdit *creationDate;
    QDateTimeEdit *modifiqueDate;


    QLineEdit *edtFilter;
    QToolButton *btnFilter;

    QRadioButton *rdbAuthor,
                 *rdbItems;

    QWidget *tab_3,
            *tab_2;

    QGroupBox *groupBox,
              *groupBox_2;

    QListWidget *lstSelectedAuthors,
                *lstAvailableAuthors,
                *lstAvailableArticles,
                *lstSelectedArticles;

    QToolButton *btnAddAuthors,
                *btnDelAuthor,
                *btnAddItems,
                *btnDelItems;

    QPushButton *btnAcept,
                *btnCancel;

    bool modified;
};

#endif // DIALOG_H
