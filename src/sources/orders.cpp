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

#include <QtGui>
#include <QtSql>

#include "../includes/orders.h"
#include "../includes/alarife.h"
#include "../includes/sqlfunctions.h"


OrdersDlg::OrdersDlg(QWidget *parent, int father, int id, int mode) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    setMinimumSize(450, 300);
    setMaximumSize(450, 300);

    userId = id;
    fatherLevel = father;
    imode = mode;

    setWindowTitle(tr("Ordenar Temas relacionados"));

    QSpacerItem *vSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *vSpacer1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    btnFirst = new QToolButton;
    btnUp = new QToolButton;
    btnDown = new QToolButton;
    btnLast = new QToolButton;

    btnFirst->setIcon(QIcon(QPixmap(":/images/go-top.png")));
    btnUp->setIcon(QIcon(QPixmap(":/images/go-up.png")));
    btnDown->setIcon(QIcon(QPixmap(":/images/go-down.png")));
    btnLast->setIcon(QIcon(QPixmap(":/images/go-bottom.png")));

    btnFirst->setToolTip(tr("Mover a la primera fila."));
    btnUp->setToolTip(tr("Subir una fila."));
    btnDown->setToolTip(tr("Bajar una fila."));
    btnLast->setToolTip(tr("Mover a la última fila."));

    listWidget = new QListWidget;

    QVBoxLayout *lytButton = new QVBoxLayout;
    lytButton->setContentsMargins(10, 0, 10, 0);
    lytButton->addItem(vSpacer);
    lytButton->addWidget(btnFirst);
    lytButton->addWidget(btnUp);
    lytButton->addWidget(btnDown);
    lytButton->addWidget(btnLast);
    lytButton->addItem(vSpacer1);

    QHBoxLayout *lytMain = new QHBoxLayout;
    lytMain->addWidget(listWidget);
    lytMain->addLayout(lytButton);

    loadFieldList();
    ActivedButton(false);

    connect(btnFirst, SIGNAL(clicked()), this, SLOT(changeFirstExec()));
    connect(btnUp, SIGNAL(clicked()), this, SLOT(changeUpExec()));
    connect(btnDown, SIGNAL(clicked()), this, SLOT(changeDownExec()));
    connect(btnLast, SIGNAL(clicked()), this, SLOT(changeLastExec()));

    connect(listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(getActualRow(int)));

    setLayout(lytMain);
}

void OrdersDlg::closeEvent(QCloseEvent *event)
{
    if (modified == true){
        QSqlQuery *qry = new QSqlQuery;

        imode == 0 ? sql = SQL_LEVEL_UPDATE : sql = SQL_ARTICLE_UPDATE;

        for (int i = 0; i < listWidget->count(); i++)
        {
            qry->exec(sql.arg(i).
                      arg(listWidget->item(i)->text()).
                      arg(fatherLevel).arg(userId));
        }
        emit updateGrid(fatherLevel);
    }
    event->accept();
}

void OrdersDlg::ActivedButton(bool actived)
{
    btnFirst->setEnabled(actived);
    btnUp->setEnabled(actived);
    btnDown->setEnabled(actived);
    btnLast->setEnabled(actived);
}

void OrdersDlg::getActualRow(int row)
{
    rowNew = row;
    ActivedButton(true);
}

void OrdersDlg::loadFieldList()
{
    QSqlQuery *qry = new QSqlQuery;

    imode == 0 ? sql = SQL_LEVEL_SELECT : sql = SQL_ARTICLE_SELECT;

    qry->exec(sql.arg(fatherLevel).arg(userId));
    while (qry->next()){
        listOrder << qry->value(0).toString();
    }
    delete qry;

    for (int i = 0; i < listOrder.count(); i++)
    {
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(listOrder.at(i));
        listWidget->insertItem(i, newItem);
    }
}

void OrdersDlg::changeFirstExec()
{
    rowOld = rowNew;
    rowNew = 0;
    listOrder.move(rowOld, rowNew);
    refreshList();
}

void OrdersDlg::changeUpExec()
{
    if (rowNew > 0)
    {
        rowOld = rowNew;
        rowNew = rowOld - 1;
        listOrder.move(rowOld, rowNew);
        refreshList();
    }
}

void OrdersDlg::changeDownExec()
{
    if (rowNew < listOrder.count())
    {
        rowOld = rowNew;
        rowNew = rowOld + 1;
        listOrder.move(rowOld, rowNew);
        refreshList();
    }
}

void OrdersDlg::changeLastExec()
{
    rowOld = rowNew;
    rowNew = listOrder.count() - 1;
    listOrder.move(rowOld, rowNew);
    refreshList();
}

void OrdersDlg::refreshList()
{
    listWidget->clear();

    for (int i = 0; i < listOrder.count(); i++)
    {
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(listOrder.at(i));
        listWidget->insertItem(i, newItem);
    }
    listWidget->setCurrentRow(rowNew);
    ActivedButton(false);
    modified = true;
}


