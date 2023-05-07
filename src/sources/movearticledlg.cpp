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
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>

#include "../includes/alarife.h"
#include "../includes/database.h"
#include "../includes/movearticledlg.h"
#include "../includes/sqlfunctions.h"

MoveArticleDlg::MoveArticleDlg(QWidget *parent, int mode, int id) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    setWindowTitle(tr("Mover artícilos..."));
    setMinimumSize(350, 100);
    setMaximumSize(350, 100);

    value = id;

    option = mode;

    QLabel *lbl = new QLabel(tr("Listado de temas disponibles:"));

    cmbList = new QComboBox;
    cmbList->addItems(QStringList() << "" << "[INICIO]");
    cmbList->addItems(getQueryList(getFatherValue(value)));

    QPushButton *btnAcept = new QPushButton(tr("Aceptar"));
    QPushButton *btnCancel = new QPushButton(tr("Cancelar"));

    btnAcept->setMaximumWidth(__BTN_SIZE__);
    btnCancel->setMaximumWidth(__BTN_SIZE__);

    QHBoxLayout *lytButton = new QHBoxLayout;
    lytButton->setAlignment(Qt::AlignRight);
    lytButton->addWidget(btnCancel);
    lytButton->addWidget(btnAcept);

    QVBoxLayout *lytMain = new QVBoxLayout;
    lytMain->addWidget(lbl);
    lytMain->addWidget(cmbList);
    lytMain->addLayout(lytButton);
    setLayout(lytMain);

    connect(cmbList, SIGNAL(currentIndexChanged(int)), this, SLOT(selectItem(int)));
    connect(btnAcept,SIGNAL(clicked()), this, SLOT(executeQuery()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
}

void MoveArticleDlg::closeEvent(QCloseEvent *event)
{
    if (option == 0)
        emit updateGrid(getFatherValue(value));
    else if (option == 1)
        emit updateGrid(index);
    event->accept();
}

void MoveArticleDlg::selectItem(int item)
{
    if (item == 1)
        index = 0;
    else
        index = lst.at(item -2);
}

void MoveArticleDlg::executeQuery()
{
    if (option == 0)
    {
        if(sqlUpdateRecord_N("articulo",
                           QString("nivel = %1").arg(index),
                           QString("WHERE id_articulo = %1").arg(value)))
        {
            QMessageBox::information(this, tr("Mover artículos"),
                                     tr("El artículo se han movido satisfactoriamente hacia "
                                        "el tema de \"%1\".").arg(cmbList->currentText()));
            close();
        }else{
            QMessageBox::critical(this, tr("Mover artículos"),
                                  tr("Error. El artículo no se ha podido mover hacia "
                                     "el tema de \"%1\".").arg(cmbList->currentText()));
        }
    }else if (option == 1) {
        if(sqlUpdateRecord_N("articulo",
                           QString("nivel = %1").arg(index),
                           QString("WHERE nivel = %2").arg(getFatherValue(value))))
        {
            QMessageBox::information(this, tr("Mover artículos"),
                                     tr("Los artículos se han movido satisfactoriamente hacia "
                                        "el tema de \"%1\".").arg(cmbList->currentText()));
            close();
        }else{
            QMessageBox::critical(this, tr("Mover artículos"),
                                  tr("Error. Los artículos no se han podido mover hacia "
                                     "el tema de \"%1\".").arg(cmbList->currentText()));
        }
    }
}

int MoveArticleDlg::getFatherValue(int i)
{
    QSqlQuery *qry = new QSqlQuery;
    qry->exec(QString("SELECT nivel FROM articulo WHERE id_articulo = %1").arg(i));
    qry->first();
    int ii = qry->value(0).toInt();
    delete qry;
    return ii;
}

QStringList MoveArticleDlg::getQueryList(int num)
{
    QStringList items;
    QSqlQuery *qryTemp = new QSqlQuery;
    qryTemp->exec(QString("SELECT nivel.id_nivel, nivel.nombre FROM nivel WHERE "
                          "nivel.id_nivel != %1 ORDER BY nivel.nombre").
                  arg(num));
    while (qryTemp->next()){
        lst << qryTemp->value(0).toInt();
        items << tr("%1").arg(qryTemp->value(1).toString());
    }
    delete qryTemp;
    return items;
}
