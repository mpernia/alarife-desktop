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

#include "../includes/authors.h"
#include "../includes/database.h"
#include "../includes/sqlfunctions.h"
#include "../includes/alarife.h"
#include "../includes/gridimage.h"


AuthorOption::AuthorOption(QWidget *parent, Book **encyc) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    setMinimumSize(685, 500);
    setMaximumSize(685, 500);

    encyclopedia = (*encyc);

    lstOptions = new QListWidget;
    lstOptions->setViewMode(QListView::IconMode);
    lstOptions->setIconSize(QSize(96, 84));
    lstOptions->setMovement(QListView::Static);
    lstOptions->setMaximumWidth(128);
    lstOptions->setMinimumHeight(478);
    lstOptions->setSpacing(12);

    pageActor = new ActorPage;
    pageAuthor = new AuthorPage;
    pageColaboration = new ColaborationPage;
    pageTeam = new TeamPage;

    pageActor->setMediaPath(encyclopedia->getMediaPath());
    pageAuthor->setMediaPath(encyclopedia->getMediaPath());
    pageColaboration->setMediaPath(encyclopedia->getMediaPath());
    pageTeam->setMediaPath(encyclopedia->getMediaPath());

    pageActor->setExtension(encyclopedia->getExtImagen());
    pageAuthor->setExtension(encyclopedia->getExtImagen());
    pageColaboration->setExtension(encyclopedia->getExtImagen());
    pageTeam->setExtension(encyclopedia->getExtImagen());

    pageAuthor->setParam(" INNER JOIN articuloautores ON articuloautores.autor = autor.id_autor");
    pageColaboration->setParam(QString(" INNER JOIN enc_autor ON enc_autor.enciclopedia = %1 "
                                       "AND enc_autor.autor = autor.id_autor WHERE "
                                       "enc_autor.colaborador = 1").arg(encyclopedia->getIndex()));
    pageTeam->setParam(QString(" INNER JOIN enc_autor ON enc_autor.enciclopedia = %1 AND enc_autor.autor"
                               " = autor.id_autor WHERE enc_autor.equipo = 1").arg(encyclopedia->getIndex()));

    pageActor->SqlExec();
    pageAuthor->SqlExec();
    pageColaboration->SqlExec();
    pageTeam->SqlExec();

    PageControl = new QStackedWidget;
    PageControl->addWidget(pageActor);
    PageControl->addWidget(pageAuthor);
    PageControl->addWidget(pageColaboration);
    PageControl->addWidget(pageTeam);

    createIcons();
    lstOptions->setCurrentRow(0);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(lstOptions);
    horizontalLayout->addWidget(PageControl, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);

    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    setLayout(mainLayout);

    setWindowTitle(tr("Actores de la enciclopedia"));
}

void AuthorOption::createIcons()
{

    QListWidgetItem *btnActor = new QListWidgetItem(lstOptions);
    btnActor->setIcon(QIcon(":/images/actor.png"));
    btnActor->setText(tr("Actores"));
    btnActor->setTextAlignment(Qt::AlignHCenter);
    btnActor->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *btnAuthor = new QListWidgetItem(lstOptions);
    btnAuthor->setIcon(QIcon(":/images/autor.png"));
    btnAuthor->setText(tr("Autores"));
    btnAuthor->setTextAlignment(Qt::AlignHCenter);
    btnAuthor->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *btnMedia = new QListWidgetItem(lstOptions);
    btnMedia->setIcon(QIcon(":/images/colaborador.png"));
    btnMedia->setText(tr("Colaboradores"));
    btnMedia->setTextAlignment(Qt::AlignHCenter);
    btnMedia->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *btnTemplate = new QListWidgetItem(lstOptions);
    btnTemplate->setIcon(QIcon(":/images/equipo.png"));
    btnTemplate->setText(tr("Equipo"));
    btnTemplate->setTextAlignment(Qt::AlignHCenter);
    btnTemplate->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(lstOptions, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
}

void AuthorOption::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current) current = previous;
    PageControl->setCurrentIndex(lstOptions->row(current));
}

TabPageAuthor::TabPageAuthor(QWidget *parent) : QWidget(parent)
{
    lblCaption = new QLabel;
    edtFilter = new QLineEdit;

    btnFilter = new QToolButton;
    btnEdit = new QToolButton;
    btnInsert = new QToolButton;
    btnDelete = new QToolButton;
    btnSelect = new QToolButton;

    btnFilter->setIcon(QIcon(":/images/action_find.png"));
    btnSelect->setIcon(QIcon(":/images/action_select.png"));
    btnEdit->setIcon(QIcon(":/images/action_edit.png"));
    btnDelete->setIcon(QIcon(":/images/action_delete.png"));
    btnInsert->setIcon(QIcon(":/images/actores.png"));

    btnFilter->setToolTip(tr("Buscar"));
    btnEdit->setToolTip(tr("Editar"));
    btnDelete->setToolTip(tr("Eliminar "));
    btnSelect->setToolTip(tr("Seleccionar"));
    btnInsert->setToolTip(tr("Insertar"));

    tblGridView = new QTableView;
    tblGridView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblGridView->setSortingEnabled(false);
    tblGridView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblGridView->setSelectionMode(QAbstractItemView::SingleSelection);
    tblGridView->setShowGrid(false);
    tblGridView->setAlternatingRowColors(true);
    tblGridView->setWordWrap(true);
    tblGridView->verticalHeader()->hide();
    tblGridView->horizontalHeader()->hide();

    QGridLayout *lytButtonPanel = new QGridLayout;
    lytButtonPanel->setAlignment(Qt::AlignRight);
    lytButtonPanel->addWidget(btnInsert, 0, 1);
    lytButtonPanel->addWidget(btnEdit, 0, 2);
    lytButtonPanel->addWidget(btnDelete, 0, 3);
    lytButtonPanel->addWidget(btnSelect, 0, 4);

    QHBoxLayout *lytFilter = new QHBoxLayout;
    lblText = new QLabel(tr("Buscar:"));
    lytFilter->addWidget(lblText);
    lytFilter->addWidget(edtFilter);
    lytFilter->addWidget(btnFilter);

    QHBoxLayout *lytTop = new QHBoxLayout;
    lytTop->addLayout(lytFilter);
    lytTop->addLayout(lytButtonPanel);

    QVBoxLayout *lytPageControl = new QVBoxLayout;
    lytPageControl->addWidget(lblCaption);
    lytPageControl->addLayout(lytTop);
    lytPageControl->addWidget(tblGridView);
    setTypeUsr();

    setLayout(lytPageControl);

    connect(btnFilter, SIGNAL(clicked()), this, SLOT(FilterMediaExec()));
    connect(edtFilter,SIGNAL(textChanged(QString)),this, SLOT(EmptyFilter()));
    connect(edtFilter, SIGNAL(returnPressed()), this, SLOT(FilterMediaExec()));

    connect(btnEdit, SIGNAL(clicked()), this, SLOT(EditRecordExec()));
    connect(btnDelete, SIGNAL(clicked()), this, SLOT(DeleteRecordExec()));
    connect(btnSelect, SIGNAL(clicked()), this, SLOT(SelectRecordExec()));
    connect(btnInsert, SIGNAL(clicked()), this, SLOT(InsertRecordExec()));

    connect(tblGridView, SIGNAL(clicked(QModelIndex)), this, SLOT(getIdUserValue(QModelIndex)));
    connect(tblGridView, SIGNAL(pressed(QModelIndex)), this, SLOT(getIdUserValue(QModelIndex)));
    connect(tblGridView, SIGNAL(activated(QModelIndex)), this, SLOT(getIdUserValue(QModelIndex)));
    connect(tblGridView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(EditRecordExec()));
}

void TabPageAuthor::setMediaPath(QString path)
{
    mediaPath = path;
}

void TabPageAuthor::setParam(QString p)
{
    param = p;
}

void TabPageAuthor::setUser(int id)
{
    userId = id;
    if (userId != 1)
    {
        btnDelete->setVisible(false);
    }
}

void TabPageAuthor::FilterMediaExec()
{
    if(edtFilter->text() != "")
    {
        SqlExec(QString("autor.nombre LIKE '%%1%' OR autor.titulo "
                        "LIKE '%%2%' OR autor.cargo LIKE '%%3%'").
                arg(edtFilter->text()).
                arg(edtFilter->text()).
                arg(edtFilter->text()));
    }
}

void TabPageAuthor::EmptyFilter()
{
    if (edtFilter->text() == "")
    {
        edtFilter->setStyleSheet(tr("background-color: rgb(255, 255, 255);"));
        SqlExec("");
    }else
        edtFilter->setStyleSheet(tr("background-color: rgb(255, 255, 255);"));
}

void TabPageAuthor::getIdUserValue(QModelIndex index)
{
    authors->query().exec();
    authors_index = index;

    QSqlRecord record = authors->record(authors_index.row());
    id_autor = record.value("id_autor").toInt();
}

void TabPageAuthor::setCaptionExec(QString caption)
{
    lblCaption->setText(tr("%1  de la enciclopedia").arg(caption));    
}

void TabPageAuthor::setTypeUsr(int param1, int param2)
{
    x = param1;
    y = param2;
}

void TabPageAuthor::SqlExec(QString filter)
{
    QString sql_qry("SELECT DISTINCT 'Nombre: ' || autor.nombre || '\n' "
                    "|| 'Titulo: ' || autor.titulo || '\n' || 'Cargo: ' || "
                    " autor.cargo || '\n' || 'E-Mail: ' || autor.correo AS "
                    "descripcion, autor.foto ||'.' || adjuntos.extension AS "
                    "fileMedia, autor.id_autor FROM autor INNER JOIN adjuntos "
                    "ON autor.foto = adjuntos.id_adjunto %1 ORDER BY autor.nombre ");

    authors = new QSqlQueryModel;

    if (filter.isEmpty())
        authors->setQuery(sql_qry.arg(param));
    else{
        QString where(" AND ");
        if (!param.contains("WHERE", Qt::CaseSensitive)) where = " WHERE ";
        authors->setQuery(sql_qry.arg(param + where +filter));
        if(authors->query().first()==false)
            edtFilter->setStyleSheet(tr("background-color: rgb(255, 140, 140);"));
    }

    updateRows();
    OpenTable();

}

void TabPageAuthor::EditRecordExec()
{
    authors->query().exec();
    QSqlRecord *record = new QSqlRecord(authors->record(authors_index.row()));
    if (record->value("id_autor").toInt() != 0)
    {
        editDlg = new EditAuthorDlg(this, id_autor, 1);
        editDlg->setCaption("Editar "+lblCaption->text().toLower());
        editDlg->setMediaPath(mediaPath);
        editDlg->setImageExtension(ext);
        editDlg->refreshvalue(&tblGridView, authors->query().lastQuery());
        editDlg->setTypeUsr(x,y);
        editDlg->show();
    }
}

void TabPageAuthor::InsertRecordExec()
{
    insertDlg = new EditAuthorDlg(this, 0, 0);
    insertDlg->setCaption("Insertar "+lblCaption->text().toLower());
    insertDlg->setImageExtension(ext);
    insertDlg->setMediaPath(mediaPath);
    insertDlg->refreshvalue(&tblGridView, authors->query().lastQuery());
    insertDlg->setTypeUsr(x,y);
    insertDlg->show();
}

void TabPageAuthor::SelectRecordExec()
{
    if (sqlAutoCountRow("autor", "") > 0)
    {
        seldlg = new SelectAuthorDlg(this, 1);
        seldlg->refreshvalue(&tblGridView, authors->query().lastQuery());
        seldlg->setTypeUsr(x,y);
        seldlg->show();
    }else
        QMessageBox::information(this, tr("Seleccionar"),
                                 tr("No existen actores registrados en la enciclopedia.\n"
                                    "Inserte primero los actores, para luego "
                                    "poder seleccionarlos."));
}

void TabPageAuthor::DeleteRecordExec()
{
    QSqlRecord *record = new QSqlRecord(authors->record(authors_index.row()));
    if (record->value("id_autor").toInt() != 0)
    {
        QString tblName,
                fields;
        switch (mode_del)
        {
            case -1: tblName = "autor";
                    fields = QString("id_autor = %1").arg(id_autor);
            break;
            case 0: tblName = "enc_autor";
                    fields = QString("autor = %1 AND colaborador = 1").arg(id_autor);
            break;
            case 1: tblName = "enc_autor";
                    fields = QString("autor = %1 AND equipo = 1").arg(id_autor);
            break;
        }

        QSqlQuery *qryTemp = new QSqlQuery;
        qryTemp->prepare(QString("DELETE FROM %1 WHERE %2;").arg(tblName).arg(fields));

        if (!qryTemp->exec()){
            QMessageBox::information(this, tr("Eliminación Fallida"),
                                     tr("El autor no se ha eliminado de la "
                                        "enciclopedia."));
        }

        delete qryTemp;
        authors->setQuery(authors->query().lastQuery());
        updateRows();
        OpenTable();
    }
}

void TabPageAuthor::OpenTable()
{
    tblGridView->setColumnWidth(0,400);
    tblGridView->setColumnWidth(1,85);
    tblGridView->hideColumn(2);
    tblGridView->selectRow(0);
}

void TabPageAuthor::updateRows()
{
    tblGridView->setModel(authors);
    for (int i=0; i < authors->rowCount(); i++){
        tblGridView->setRowHeight(i,85);
    }

    tblGridView->setItemDelegate(new GridImg(tblGridView, 0, mediaPath));
    tblGridView->show();
}

void TabPageAuthor::CustomBottomsPanel(int x)
{
    mode_del = x;
    switch (x){
        case 0 : btnInsert->setVisible(false);
                 setTypeUsr(1,0);
                 break;
        case 1 : btnInsert->setVisible(false);
                 setTypeUsr(0,1);
                 break;
        case 2 : btnDelete->setVisible(false);
                 btnInsert->setVisible(false);
                 btnSelect->setVisible(false);
                 setTypeUsr(0,0);
                 break;
        default : btnSelect->setVisible(false);
                  setTypeUsr(0,0);
                  break;
    }
}


ActorPage::ActorPage(TabPageAuthor *parent) : TabPageAuthor(parent)
{
    setCaptionExec(tr("Actores"));
    CustomBottomsPanel();
}

AuthorPage::AuthorPage(TabPageAuthor *parent) : TabPageAuthor(parent)
{
    setCaptionExec(tr("Autores"));
    CustomBottomsPanel(2);
}

ColaborationPage::ColaborationPage(TabPageAuthor *parent) : TabPageAuthor(parent)
{
    setCaptionExec(tr("Colaboradores"));
    CustomBottomsPanel(0);
}

TeamPage::TeamPage(TabPageAuthor *parent) : TabPageAuthor(parent)
{
    setCaptionExec(tr("Equipo"));
    CustomBottomsPanel(1);
}

