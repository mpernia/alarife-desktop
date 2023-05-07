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
#include "../includes/movechapterdlg.h"
#include "../includes/sqlfunctions.h"

MoveChapterDlg::MoveChapterDlg(QWidget *parent, int mode, int id) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    setWindowTitle(tr("Mover temas..."));
    setMinimumSize(350, 140);
    setMaximumSize(350, 140);

    old = id;

    option = mode;

    edtFilter = new QLineEdit;
    btnFilter = new QToolButton;
    QLabel *lblFilter = new QLabel(tr("Buscar:"));

    btnFilter->setIcon(QIcon(":/images/action_find.png"));
    btnFilter->setToolTip(tr("Buscar"));

    QHBoxLayout *lytFilter = new QHBoxLayout;
    lytFilter->addWidget(lblFilter);
    lytFilter->addWidget(edtFilter);
    lytFilter->addWidget(btnFilter);

    QLabel *lbl = new QLabel(tr("Listado de temas disponibles:"));

    cmbList = new QComboBox;
    cmbList->addItems(QStringList() << "" << "[INICIO]");
    cmbList->addItems(getQueryList(old));

    QPushButton *btnAcept = new QPushButton(tr("Aceptar"));
    QPushButton *btnCancel = new QPushButton(tr("Cancelar"));

    btnAcept->setMaximumWidth(__BTN_SIZE__);
    btnCancel->setMaximumWidth(__BTN_SIZE__);

    QHBoxLayout *lytButton = new QHBoxLayout;
    lytButton->setAlignment(Qt::AlignRight);
    lytButton->addWidget(btnCancel);
    lytButton->addWidget(btnAcept);

    QVBoxLayout *lytMain = new QVBoxLayout;
    lytMain->addLayout(lytFilter);
    lytMain->addWidget(lbl);
    lytMain->addWidget(cmbList);
    lytMain->addLayout(lytButton);
    setLayout(lytMain);

    connect(cmbList, SIGNAL(currentIndexChanged(int)), this, SLOT(selectItem(int)));
    connect(btnAcept,SIGNAL(clicked()), this, SLOT(executeQuery()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(btnFilter, SIGNAL(clicked()), this, SLOT(filterChapter()));
    connect(edtFilter,SIGNAL(textChanged(QString)),this, SLOT(emptyFilter()));
}

void MoveChapterDlg::closeEvent(QCloseEvent *event)
{
    emit moveFatherLevel(idValue);
    event->accept();
}

void MoveChapterDlg::selectItem(int item){
    if (item == 1)
        index = 0;
    else
        index = lst.at(item -2);
}

void MoveChapterDlg::executeQuery()
{
    if (option == 0)
    {
        if (sqlUpdateRecord_N("nivel",
                            QString("padre = %1").arg(index),
                            QString("WHERE id_nivel = %1").arg(old)))
        {
            idValue = index;
            QMessageBox::information(this, tr("Mover temas"),
                                     tr("El tema se han movido satisfactoriamente como "
                                        "subtema de \"%1\".").arg(cmbList->currentText()));
            close();
        }else{
            idValue = old;
            QMessageBox::critical(this, tr("Mover temas"),
                                  tr("Error. El tema no se ha podido mover como "
                                     "subtema de \"%1\".").arg(cmbList->currentText()));
        }
    }else if (option == 1){
        if (sqlUpdateRecord_N("nivel",
                            QString("padre = %1").arg(index),
                            QString("WHERE padre = %1").arg(old)))
        {
            idValue = index;
            QMessageBox::information(this, tr("Mover temas"),
                                     tr("Los temas se han movido satisfactoriamente como "
                                        "subtemas de \"%1\".").arg(cmbList->currentText()));
            close();
        }else{
            idValue = old;
            QMessageBox::critical(this, tr("Mover temas"),
                                  tr("Error. Los temas no se ha podido mover como "
                                     "subtemas de \"%1\".").arg(cmbList->currentText()));
        }
    }
}

QStringList MoveChapterDlg::getQueryList(int num)
{
    QString filter = "";
    if (!edtFilter->text().isEmpty())
    {
        filter = QString("AND nivel.nombre LIKE '%%1%'").arg(edtFilter->text());
    }

    QStringList items;
    QSqlQuery *qryTemp = new QSqlQuery;

    qryTemp->exec(QString("SELECT nivel.id_nivel, nivel.nombre FROM nivel WHERE "
                          "nivel.id_nivel != %1 AND nivel.padre != %2 %3 "
                          "ORDER BY nivel.nombre").
                  arg(num).arg(num).arg(filter));

    while (qryTemp->next())
    {
        lst << qryTemp->value(0).toInt();
        items << tr("%1").arg(qryTemp->value(1).toString());
    }

    delete qryTemp;
    return items;
}

void MoveChapterDlg::emptyFilter()
{
    if (edtFilter->text().isEmpty())
    {
        resetList();
    }
    edtFilter->setStyleSheet(tr("background-color: rgb(255, 255, 255);"));

}

void MoveChapterDlg::filterChapter()
{
    resetList();

    if (cmbList->count() == 0)
    {
        edtFilter->setStyleSheet(tr("background-color: rgb(255, 140, 140);"));
    }
}

void MoveChapterDlg::resetList()
{
    if (lst.count() > 0)
    {
        cmbList->clear();
        lst.clear();
    }
    cmbList->addItems(QStringList() << "" << "[INICIO]");
    cmbList->addItems(getQueryList(old));
}
