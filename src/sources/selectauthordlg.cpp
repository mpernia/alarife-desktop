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
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtSql>

#include "../includes/database.h"
#include "../includes/alarife.h"
#include "../includes/gridimage.h"
#include "../includes/selectauthordlg.h"
#include "../includes/sqlfunctions.h"

SelectAuthorDlg::SelectAuthorDlg(QWidget *parent, int id) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    QSpacerItem *vSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    setMinimumSize(350, 150);
    setMaximumSize(350, 150);
    idEnc = id;

    btnOk = new QPushButton(tr("Aceptar"));
    btnCancel = new QPushButton(tr("Cerrar"));

    btnOk->setMaximumWidth(__BTN_SIZE__);
    btnCancel->setMaximumWidth(__BTN_SIZE__);

    lblText = new QLabel;
    cmbList = new QComboBox;

    qryAuthor = new QSqlQuery;
    qryAuthor->exec(QString("SELECT DISTINCT autor.nombre FROM autor"));

    cmbList->addItem("");
    while (qryAuthor->next()){
        cmbList->addItems(QStringList() << qryAuthor->value(0).toString());
    }
    delete qryAuthor;


    QGridLayout *lytButtonPanel = new QGridLayout;
    lytButtonPanel->setAlignment(Qt::AlignRight);
    lytButtonPanel->addWidget(btnOk,0,1);
    lytButtonPanel->addWidget(btnCancel,0,2);

    QVBoxLayout *lytDescription = new QVBoxLayout;
    lblText = new QLabel(tr("Seleccione de la lista el nombre del actor que desea "
                         "agregar a la categoría."));
    lblText->setWordWrap(true);
    lytDescription->addWidget(lblText);
    lblText = new QLabel("Actores:");
    lytDescription->addWidget(lblText);
    lytDescription->addWidget(cmbList);
    lytDescription->addItem(vSpacer);
    lytDescription->addLayout(lytButtonPanel);

    connect(cmbList, SIGNAL(activated(QString)), this, SLOT(selectAuthor(QString)));
    connect(btnOk, SIGNAL(clicked()), this, SLOT(PostExec()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));

    setLayout(lytDescription);
}

void SelectAuthorDlg::setTypeUsr(int param1, int param2)
{
    colab = param1;
    team = param2;

    QString title;
    if (colab == 1) title.append("colaboradores");
    if (team == 1)title.append("equipo");
    setWindowTitle(tr("Seleccionar %1").arg(title));
}

void SelectAuthorDlg::PostExec()
{
    sqlInsertRecord_N("enc_autor",
                    "enciclopedia, autor, colaborador, equipo",
                    QString("%1, %2, %3, %4").
                            arg(idEnc).
                            arg(id_autor).
                            arg(colab).
                            arg(team));

    RefreshGrid();
}

int SelectAuthorDlg::setFirstValue()
{
    qryAuthor = new QSqlQuery;
    qryAuthor->exec(QString("SELECT id_autor FROM autor ORDER BY nombre"));
    qryAuthor->first();
    int id = qryAuthor->value(0).toInt();
    delete qryAuthor;
    return id;
}

void SelectAuthorDlg::selectAuthor(QString name)
{
    qryAuthor = new QSqlQuery;
    qryAuthor->exec(QString("SELECT id_autor FROM autor WHERE nombre = '%1'").arg(name));
    qryAuthor->first();
    id_autor = qryAuthor->value(0).toInt();
    delete qryAuthor;
}

void SelectAuthorDlg::refreshvalue(QTableView **tbl, QString sql)
{
   grid = (*tbl);
   sqlStr = sql;
}

void SelectAuthorDlg::RefreshGrid()
{
    QSqlQueryModel *master = new QSqlQueryModel;
    master->setQuery(sqlStr);
    grid->setModel(master);

    for (int i=0; i < master->rowCount(); i++)
    {
        grid->setRowHeight(i,85);
    }

    grid->show();
    grid->selectRow(0);
    grid->setColumnWidth(0,400);
    grid->setColumnWidth(1,85);
    grid->hideColumn(2);
}


