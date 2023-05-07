#include <QtGui>
#include <QSqlDatabase>
#include <QTimer>


#include "../includes/config.h"
#include "../includes/alarife.h"
#include "../includes/database.h"
#include "../includes/editusers.h"
#include "../includes/folder.h"
#include "../includes/sqlfunctions.h"
#include "../includes/rootdlg.h"

#include "../includes/dlgadminuser.h"

dlgAdminUser::dlgAdminUser(QWidget *parent) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);
    setWindowTitle(tr("Administrar uusarios"));

    chkActiveUser = new QCheckBox(tr("Activar la gestión de usuarios en las enciclopedias."));

    tblUserView = new QTableView;
    tblUserView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblUserView->setSortingEnabled(false);
    tblUserView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblUserView->setSelectionMode(QAbstractItemView::SingleSelection);
    tblUserView->setShowGrid(false);
    tblUserView->setAlternatingRowColors(true);
    tblUserView->setWordWrap(true);
    tblUserView->verticalHeader()->hide();
    tblUserView->horizontalHeader()->hide();

    edtLogin = new QLineEdit;
    edtPass = new QLineEdit;
    edtPass->setEchoMode(QLineEdit::Password);

    btnIndert = new QPushButton(tr("Insertar"));
    btnEdit = new QPushButton(tr("Editar"));
    btnDelete = new QPushButton(tr("Eliminar"));

    btnIndert->setMaximumWidth(__BTN_SIZE__);
    btnEdit->setMaximumWidth(__BTN_SIZE__);
    btnDelete->setMaximumWidth(__BTN_SIZE__);

    edtLogin->setEnabled(false);
    edtPass->setEnabled(false);

    btnIndert->setEnabled(false);
    btnEdit->setEnabled(false);
    btnDelete->setEnabled(false);
    tblUserView->setEnabled(false);

    edtLogin->setText("");
    edtPass->setText("");

    QGridLayout *lytButtonPanel = new QGridLayout;
    lytButtonPanel->setAlignment(Qt::AlignRight);
    lytButtonPanel->addWidget(btnIndert,0,1);
    lytButtonPanel->addWidget(btnEdit,0,2);
    lytButtonPanel->addWidget(btnDelete,0,3);

    QVBoxLayout *lytUser = new QVBoxLayout;
    QLabel *lblText = new QLabel(tr("Usuarios:"));
    lytUser->addWidget(lblText);
    lytUser->addWidget(chkActiveUser);
    lytUser->addLayout(lytButtonPanel);
    lytUser->addWidget(tblUserView);

    setLayout(lytUser);

    connect(chkActiveUser,SIGNAL(toggled(bool)),this,SLOT(ActiveUserExec(bool)));
    connect(btnIndert, SIGNAL(clicked()), this, SLOT(InsertUserExec()));
    connect(btnEdit, SIGNAL(clicked()), this, SLOT(EditUserExec()));
    connect(btnDelete, SIGNAL(clicked()), this, SLOT(DeleteUserExec()));

    connect(tblUserView, SIGNAL(clicked(QModelIndex)), this, SLOT(getUserIndex(QModelIndex)));
    connect(tblUserView, SIGNAL(pressed(QModelIndex)), this, SLOT(getUserIndex(QModelIndex)));
    connect(tblUserView, SIGNAL(activated(QModelIndex)), this, SLOT(getUserIndex(QModelIndex)));
    connect(tblUserView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(ShowUser(QModelIndex)));
}

void dlgAdminUser::ActiveUserExec(bool mode)
{
    btnIndert->setEnabled(mode);
    btnEdit->setEnabled(mode);
    btnDelete->setEnabled(mode);
    tblUserView->setEnabled(mode);
    ShowUserValue(mode);
}

void dlgAdminUser::EditUserExec()
{
    if (idUsr.isValid()){
        QSqlRecord record = users->record(idUsr.row());
        EditUsers *user = new EditUsers(this, record.value(0).toInt());
        connect(user, SIGNAL(refreshGrid(bool)), this, SLOT(ActiveUserExec(bool)));
        user->show();
    }
}

void dlgAdminUser::InsertUserExec()
{
    EditUsers *user = new EditUsers(this, 0);
    connect(user, SIGNAL(refreshGrid(bool)), this, SLOT(ActiveUserExec(bool)));
    user->show();
}

void dlgAdminUser::DeleteUserExec()
{
    if (idUsr.isValid())
    {
        QSqlRecord record = users->record(idUsr.row());
        QString reg(record.value("nombre").toString());

        if (sqlDeleteRecord_N("usuario",
                            QString("id_usuario = %1").
                                    arg(record.value("id_usuario").toInt())))
        {
            QMessageBox::information(this, tr("Eliminación Fallida"),
                                     tr("El usuario \"%1\" no se ha eliminado \nde la "
                                        "enciclopedia.").arg(reg));
        }else{
            QMessageBox::information(this,
                                     tr("Eliminación Satisfactoria"),
                                     tr("El usuario \"%1\" se ha eliminado \nde la "
                                        "enciclopedia.").arg(reg));
            ActiveUserExec(true);
        }
    }
}

void dlgAdminUser::getUserIndex(QModelIndex index)
{
    idUsr = index;
}

void dlgAdminUser::ShowUser(QModelIndex index)
{
    QSqlRecord record = users->record(index.row());
    EditUsers *user = new EditUsers(this, record.value(0).toInt());
    connect(user, SIGNAL(refreshGrid(bool)), this, SLOT(ShowUserValue(bool)));
    user->show();
}

void dlgAdminUser::ShowUserValue(bool type)
{
    users = new QSqlQueryModel;
    if (type == false)
        users->setQuery(QString("SELECT usuario.id_usuario FROM usuario "
                                "WHERE usuario.id_usuario = -1"));
    else users->setQuery(QString("SELECT usuario.id_usuario, 'Nombre: ' || usuario.nombre "
                                 "|| '\n' || 'Usuario: ' || usuario.usuario || '\n' || 'Tipo "
                                 "de Usuario: ' || tiposusuarios.descripcion AS usr, usuario.nombre "
                                 "FROM usuario INNER JOIN tiposusuarios ON usuario.tipousuario  = "
                                 "tiposusuarios.id_tipo_usuario "));

    tblUserView->setModel(users);

    for (int i=0; i < users->rowCount(); i++) tblUserView->setRowHeight(i,65);

    tblUserView->show();
    tblUserView->setColumnWidth(1,tblUserView->width()+130);
    tblUserView->hideColumn(0);
    tblUserView->hideColumn(2);
    tblUserView->selectRow(0);
}


