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

#ifndef NEWITEM_H
#define NEWITEM_H

#include <QDialog>
#include "includes/itemid.h"


namespace Ui {
    class NewItem;
}

class NewItem : public QDialog
{
    Q_OBJECT

public:
    explicit NewItem(ItemId *i, QWidget *parent = 0);
    ~NewItem();

private:
    void setUpdateInterface(bool);
    void getAllValues();
    Ui::NewItem *ui;
    ItemId *itemId;
    int usuario;
    void insertNewAuthors();
    void insertNewItems();

private slots:
    void on_aceptButton_clicked();
    void on_insertItems_clicked();
    void on_insertAuthors_clicked();
    void on_delAuthor_clicked();
    void on_delItems_clicked();
    void on_cmbUser_currentIndexChanged(const QString &arg1);

signals:
    void insertado(ItemId i);
};

#endif // NEWITEM_H
