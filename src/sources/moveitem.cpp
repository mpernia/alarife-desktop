#include "../includes/moveitem.h"
#include "../includes/database.h"
#include "../includes/sqlfunctions.h"

MoveElement::MoveElement(QWidget *parent) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__); 

    create_widget();

    backLeft.push(0);
    backRight.push(0);

    load_records(&tblLeft, 0);
    load_records(&tblRight, 0);

    connect(tblLeft, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(show_left_items()));
    connect(tblRight, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(show_right_items()));
    connect(btnToLeft, SIGNAL(clicked()), this, SLOT(move_right_to_left()));
    connect(btnToRight, SIGNAL(clicked()), this, SLOT(move_left_to_right()));
}

MoveElement::~MoveElement()
{
    
}

void MoveElement::move_left_to_right()
{
    if (backLeft.top() != backRight.top())
    {
        if (!tblLeft->selectedItems().isEmpty())
        {
            QList<QTableWidgetItem *>lstLeftSelected = tblLeft->selectedItems();

            QTableWidgetItem *item;
            foreach (item, lstLeftSelected)
            {
                if (item->data(Qt::DisplayRole).toString() != "[...]")
                {
                    int row = item->row();
                    move_itemWidget(tblLeft, row, backRight.top());
                }
            }
            update_list();
        }
        else
            QMessageBox::critical(this, tr("%1").arg(windowTitle()),
                                  tr("Debe seleccionar al menos un artículo o tema "
                                     "en la lista ubicada a la izquierda antes de pulsar "
                                     "el botóns  «mover para la derecha»."));
    }else
    {
        QMessageBox::critical(this, tr("%1").arg(windowTitle()),
                              tr("Error. Imposible mover artículos o temas; ambas "
                                 "listas.están en el mismo orden."));
    }
}

void MoveElement::move_right_to_left()
{
    if (backLeft.top() != backRight.top())
    {
        if (!tblRight->selectedItems().isEmpty())
        {
            QList<QTableWidgetItem *>lstRightSelected = tblRight->selectedItems();

            QTableWidgetItem *item;
            foreach (item, lstRightSelected)
            {
                if (item->data(Qt::DisplayRole).toString() != "[...]")
                {
                    int row = item->row();
                    move_itemWidget(tblRight, row, backLeft.top());
                }
            }
            update_list();
        }
        else
            QMessageBox::critical(this, tr("%1").arg(windowTitle()),
                                  tr("Debe seleccionar al menos un artículo o tema "
                                     "en la lista ubicada a la derecha antes de pulsar "
                                     "el botóns  «mover para la izquierda»."));
    }else
    {
        QMessageBox::critical(this, tr("%1").arg(windowTitle()),
                              tr("Error. Imposible mover artículos o temas; ambas "
                                 "listas.están en el mismo orden."));
    }
}

void MoveElement::show_left_items()
{
    int i = tblLeft->currentRow();
    int j = 0;

    if (tblLeft->item(i,0)->data(Qt::DisplayRole).toString() == "[...]")
    {
        backLeft.pop();
        j = backLeft.top();
    }
    else
    {
        j = tblLeft->item(i,1)->data(Qt::DisplayRole).toInt();
        backLeft.push(j);
    }

    if (chkSynchronize->isChecked())
    {
        load_records(&tblLeft, j);
        load_records(&tblRight, j);
        backRight.push(j);
    }
    else
        load_records(&tblLeft, j);
}

void MoveElement::show_right_items()
{
    int i = tblRight->currentRow();
    int j = 0;

    if (tblRight->item(i,0)->data(Qt::DisplayRole).toString() == "[...]")
    {
        backRight.pop();
        j = backRight.top();
    }
    else
    {
        j = tblRight->item(i,1)->data(Qt::DisplayRole).toInt();
        backRight.push(j);
    }

    if (chkSynchronize->isChecked())
    {
        load_records(&tblRight, j);
        load_records(&tblLeft, j);
        backLeft.push(j);
    }
    else
        load_records(&tblRight, j);
}

void MoveElement::create_widget()
{
    chkSynchronize = new QCheckBox(tr("Sincronizar listas"));

    tblLeft = new QTableWidget;
    tblLeft->setColumnCount(4);
    tblLeft->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblLeft->setSortingEnabled(false);
    tblLeft->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblLeft->setSelectionMode(QAbstractItemView::SingleSelection);
    tblLeft->setShowGrid(false);
    tblLeft->setAlternatingRowColors(true);
    tblLeft->setWordWrap(true);
    tblLeft->verticalHeader()->hide();
    tblLeft->horizontalHeader()->hide();
    tblLeft->setColumnWidth(0,290);
    tblLeft->setColumnHidden(1, true);
    tblLeft->setColumnHidden(2, true);
    tblLeft->setColumnHidden(3, true);
    tblLeft->resizeRowsToContents();
    tblLeft->setSelectionMode(QAbstractItemView::MultiSelection);

    tblRight = new QTableWidget;
    tblRight->setColumnCount(4);
    tblRight->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblRight->setSortingEnabled(false);
    tblRight->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblRight->setSelectionMode(QAbstractItemView::SingleSelection);
    tblRight->setShowGrid(false);
    tblRight->setAlternatingRowColors(true);
    tblRight->setWordWrap(true);
    tblRight->verticalHeader()->hide();
    tblRight->horizontalHeader()->hide();
    tblRight->setColumnWidth(0,290);
    tblRight->setColumnHidden(1, true);
    tblRight->setColumnHidden(2, true);
    tblRight->setColumnHidden(3, true);
    tblRight->resizeRowsToContents();
    tblRight->setSelectionMode(QAbstractItemView::MultiSelection);

    btnToLeft = new QToolButton;
    btnToRight = new QToolButton;

    btnToLeft->setToolTip(tr("Mover para la izquierda"));
    btnToRight->setToolTip(tr("Mover para la derecha"));

    btnToLeft->setText(tr("<<-"));
    btnToRight->setText(tr("->>"));

    QSpacerItem *vSpacer = new QSpacerItem(10, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *vSpacer1 = new QSpacerItem(10, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    QVBoxLayout *lytButtons = new QVBoxLayout();
    lytButtons->addItem(vSpacer);
    lytButtons->addWidget(btnToRight);
    lytButtons->addWidget(btnToLeft);
    lytButtons->addItem(vSpacer1);


    QHBoxLayout *lytCentral = new QHBoxLayout;
    lytCentral->addWidget(tblLeft);
    lytCentral->addLayout(lytButtons);
    lytCentral->addWidget(tblRight);

    QVBoxLayout *lytMain = new QVBoxLayout;
    lytMain->addWidget(chkSynchronize);
    lytMain->addLayout(lytCentral);

    setLayout(lytMain);
    setWindowTitle(tr("Mover temas o artículos"));
    setMinimumWidth(700);
    setMaximumWidth(700);
}

void MoveElement::load_records(QTableWidget **tblElement, int father)
{
    qryData = new QSqlQuery;
    qryData->exec(sqlLevels.arg(father)); qDebug() << qryData->lastQuery() << endl;

    int i = 0;

    if (father > 0)
    {
        (*tblElement)->setRowCount(i+1);
        QTableWidgetItem *itemTitle = new QTableWidgetItem(tr("[...]"));
        QTableWidgetItem *itemId = new QTableWidgetItem("-1");
        QTableWidgetItem *itemFather = new QTableWidgetItem("-1");
        QTableWidgetItem *itemType = new QTableWidgetItem("-1");

        itemTitle->setToolTip(tr("Regresar"));
        (*tblElement)->setItem(i, 0, itemTitle);
        (*tblElement)->setItem(i, 1, itemId);
        (*tblElement)->setItem(i, 2, itemFather);
        (*tblElement)->setItem(i, 3, itemType);
        i++;
    }

    while (qryData->next())
    {
        QTableWidgetItem *itemTitle = new QTableWidgetItem(QIcon(":/images/nivel_new.png"), tr("%1").arg(qryData->value(0).toString()));
        QTableWidgetItem *itemId = new QTableWidgetItem(tr("%1").arg(qryData->value(1).toInt()));
        QTableWidgetItem *itemFather = new QTableWidgetItem(tr("%1").arg(qryData->value(2).toInt()));
        QTableWidgetItem *itemType = new QTableWidgetItem("0");

        itemTitle->setToolTip(tr("Nivel: %1").arg(qryData->value(0).toString()));
        (*tblElement)->setRowCount(i+1);
        (*tblElement)->setItem(i, 0, itemTitle);
        (*tblElement)->setItem(i, 1, itemId);
        (*tblElement)->setItem(i, 2, itemFather);
        (*tblElement)->setItem(i, 3, itemType);
        (*tblElement)->resizeRowsToContents();
        i++;
    }

    qryData->exec(sqlArticles.arg(father));

    while (qryData->next())
    {
        QTableWidgetItem *itemTitle = new QTableWidgetItem(QIcon(":/images/articulo_new.png"), tr("%1").arg(qryData->value(0).toString()));
        QTableWidgetItem *itemId = new QTableWidgetItem(tr("%1").arg(qryData->value(1).toInt()));
        QTableWidgetItem *itemFather = new QTableWidgetItem(tr("%1").arg(qryData->value(2).toInt()));
        QTableWidgetItem *itemType = new QTableWidgetItem("1");

        itemTitle->setToolTip(tr("Artículo: %1").arg(qryData->value(0).toString()));
        (*tblElement)->setRowCount(i+1);
        (*tblElement)->setItem(i, 0, itemTitle);
        (*tblElement)->setItem(i, 1, itemId);
        (*tblElement)->setItem(i, 2, itemFather);
        (*tblElement)->setItem(i, 3, itemType);
        (*tblElement)->resizeRowsToContents();
        i++;
    }

    delete qryData;
}

void MoveElement::update_list()
{
    int i = backLeft.top();
    int j = backRight.top();

    load_records(&tblLeft, i);
    load_records(&tblRight, j);
}

void MoveElement::move_itemWidget(const QTableWidget *tbl, int i, int j)
{
    QString table;
    QString field;
    QString values;

    if (tbl->item(i, 3)->data(Qt::DisplayRole).toInt() == 0)
    {
        table = "nivel";
        field = "padre=";
        values = "WHERE id_nivel=";
    }
    else
    {
        table = "articulo";
        field = "nivel=";
        values = "WHERE id_articulo=";
    }

    sqlUpdateRecord_N(table,
                    field.append(QString("%1").arg(j)),
                    values.append(tbl->item(i, 1)->data(Qt::DisplayRole).toString()));

}
