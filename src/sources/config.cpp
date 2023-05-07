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
#include <QTimer>


#include "../includes/config.h"
#include "../includes/alarife.h"
#include "../includes/database.h"
#include "../includes/editusers.h"
#include "../includes/folder.h"
#include "../includes/sqlfunctions.h"
#include "../includes/rootdlg.h"

 ConfigDialog::ConfigDialog(QWidget *parent, Book **encyc)  : QDialog(parent){
     QWidget::setWindowModality(Qt::WindowModal);
     QWidget::setWindowFlags(__WIN_FLAG__);

    encyclopedia = (*encyc);
    setWindowTitle(tr("Preferencias"));

    setMinimumSize(600, 390);
    setMaximumSize(600, 390);

    lstOptions = new QListWidget;
    lstOptions->setViewMode(QListView::IconMode);
    lstOptions->setIconSize(QSize(96, 84));
    lstOptions->setMovement(QListView::Static);
    lstOptions->setMaximumWidth(128);
    lstOptions->setMinimumHeight(365);
    lstOptions->setSpacing(12);

    aparencetab = new AparenceTab;
    usertab = new UserTab;
    templatetab = new TemplateTab;

    PageControl = new QStackedWidget;
    PageControl->addWidget(aparencetab);
    PageControl->addWidget(templatetab);
    PageControl->addWidget(usertab);

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
}

void ConfigDialog::createIcons()
{
    QListWidgetItem *btnAparence = new QListWidgetItem(lstOptions);
    btnAparence->setIcon(QIcon(":/images/conf_aparence.png"));
    btnAparence->setText(tr("Apariencia"));
    btnAparence->setTextAlignment(Qt::AlignHCenter);
    btnAparence->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *btnTemplate = new QListWidgetItem(lstOptions);
    btnTemplate->setIcon(QIcon(":/images/conf_template.png"));
    btnTemplate->setText(tr("Plantillas"));
    btnTemplate->setTextAlignment(Qt::AlignHCenter);
    btnTemplate->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *btnUser = new QListWidgetItem(lstOptions);
    btnUser->setIcon(QIcon(":/images/conf_user.png"));
    btnUser->setText(tr("Usuarios"));
    btnUser->setTextAlignment(Qt::AlignHCenter);
    btnUser->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(lstOptions, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    int page = lstOptions->row(current); qDebug() << "page: " << page << endl;

    if (!current) current = previous;

    if (page < 2)
    {
        PageControl->setCurrentIndex(lstOptions->row(current));
    }else{
        QString passwd = QInputDialog::getText(this,
                                               tr("Contraseña de administración"),
                                               tr("Escriba la contraseña del Editor Jefe:"),
                                               QLineEdit::Password, "", &access);
        if (access && !passwd.isEmpty())
        {
            QCryptographicHash *md5 = new QCryptographicHash(QCryptographicHash::Md5);
            QByteArray *byte = new QByteArray;

            md5->addData(byte->append(passwd));
            QSqlQuery *qry = new QSqlQuery;
            qry->exec(QString("SELECT tipousuario, clave FROM usuario WHERE tipousuario = 1"));
            qry->first();

            if (qry->value(1).toString() != md5->result().data())
            {
                QMessageBox::critical(this, tr("Autenticación"),
                                      tr("Error. Contraseña incorrecta. Inténtelo otra vez"));
                PageControl->setCurrentIndex(lstOptions->row(previous));
                lstOptions->setCurrentItem(previous);
            }
            else
                PageControl->setCurrentIndex(page);
        }
    }
}

void ConfigDialog::closeEvent(QCloseEvent *event)
{
    if (aparencetab->getModifique() == true)
    {
        aparencetab->saveAparence();
    }
    event->accept();
}

AparenceTab::AparenceTab(QWidget *parent) : QWidget(parent)
{
    isModifique = false;
    cmbStyle = new QComboBox;
    cmbStyle->addItem("");
    cmbStyle->addItems(QStyleFactory::keys());
    cmbControls = new QComboBox;
    vSpacer = new QSpacerItem(10, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

    chkActiveCtrl = new QCheckBox(tr("Personalizar estílo"));
    chkActiveCtrl->setChecked(false);

    btnDelete = new QPushButton(tr("Eliminar"));
    btnNew = new QPushButton(tr("Nuevo"));

    btnDelete->setMaximumWidth(__BTN_SIZE__);
    btnNew->setMaximumWidth(__BTN_SIZE__);

    QLabel *lblInfo = new QLabel(tr("<b>Nota:</b> Los cambios en la configuración "
                                "de la apariencia tendrán efecto la próxima vez "
                                "que se inicie Alarife."));
    lblInfo->setWordWrap(true);

    QGridLayout *lytButtonPanel = new QGridLayout;
    lytButtonPanel->setAlignment(Qt::AlignRight);
    lytButtonPanel->addWidget(btnNew,0,1);
    lytButtonPanel->addWidget(btnDelete,0,3);

    QGridLayout *lytWidgets = new QGridLayout;
    QLabel *lblText1 = new QLabel(tr("Interfáz Gráfica de Usuario:"));
    lytWidgets->addWidget(lblText1,0,1);
    QLabel *lblText2=new QLabel(tr("Controles Personalizados:"));
    lytWidgets->addWidget(lblText2,0,2);
    lytWidgets->addWidget(cmbStyle,1,1);
    lytWidgets->addWidget(cmbControls,1,2);

    QVBoxLayout *lytAparence = new QVBoxLayout;
    QLabel *lblText3 = new QLabel(tr("Apariencia"));
    lytAparence->addWidget(lblText3);

    lytAparence->addWidget(chkActiveCtrl);
    lytAparence->addLayout(lytButtonPanel);
    lytAparence->addLayout(lytWidgets);
    lytAparence->addItem(vSpacer);
    lytAparence->addWidget(lblInfo);

    LoadStyleNames();
    ActiveControlStyle(false);

    connect(cmbStyle,SIGNAL(activated(QString)), this, SLOT(ChangeStyle(QString)));
    connect(cmbControls,SIGNAL(activated(QString)), this, SLOT(LoadStyleFiles(QString)));
    connect(chkActiveCtrl,SIGNAL(toggled(bool)),this,SLOT(ActiveControlStyle(bool)));

    connect(btnDelete, SIGNAL(clicked()), this, SLOT(deleteStyle()));
    connect(btnNew, SIGNAL(clicked()), this, SLOT(insertStyle()));

    setLayout(lytAparence);

    QSettings preference("Alarife", "Preference");
    strStyle = preference.value("style").toString();
    strStylesheet = preference.value("stylesheet").toString();

    int i = cmbStyle->findText(strStyle, Qt::MatchExactly);
    int j = cmbControls->findText(strStylesheet, Qt::MatchExactly);
    cmbStyle->setCurrentIndex(i);
    cmbControls->setCurrentIndex(j);

}

void AparenceTab::ChangeStyle(QString style)
{
    if (strStyle != style)
    {
        isModifique = true;
        strStyle = style;
    }
}

void AparenceTab::ActiveControlStyle(bool active)
{
    cmbStyle->setEnabled(active);
    cmbControls->setEnabled(active);

    btnDelete->setEnabled(active);
    btnNew->setEnabled(active);
}

void AparenceTab::LoadStyleNames()
{
    QString path = QString("%1/styles/").arg(__APP_PATH__);
    if (path.contains("/bin")) path.remove("/bin");
    QDir dir(path);

    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    cmbControls->clear();
    cmbControls->addItem("");
    for (int i = 0;  i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        cmbControls->addItem(fileInfo.fileName());
    }

}

void AparenceTab::LoadStyleFiles(QString name)
{
    QString path = QString("%1/styles/").arg(__APP_PATH__);
    if (path.contains("/bin")) path.remove("/bin");

    if (strStylesheet != name){

        isModifique = true;
        strStylesheet = name;
    }
}

void AparenceTab::saveAparence()
{
    QSettings Preference("Alarife", "Preference");
    Preference.setValue("style", strStyle);
    Preference.setValue("stylesheet", strStylesheet);
}

void AparenceTab::deleteStyle()
{
    QString path = QString("%1/styles/").arg(__APP_PATH__);
    if (path.contains("/bin")) path.remove("/bin");
    if (!curFile.isEmpty())
    {
        QFile::remove(path + curFile);
        LoadStyleNames();
    }
}

void AparenceTab::insertStyle()
{
    curFile = QFileDialog::getOpenFileName(this, tr("Insertar Hoja de Estilo"),
                                                   QDir::homePath(),
                                                   tr("Hojas de Estilos[*.qss] (*.qss)"));
    QString path = QString("%1/styles/").arg(__APP_PATH__);
    if (path.contains("/bin")) path.remove("/bin");

    if (!curFile.isEmpty()){
        QFile::copy(curFile, path + QFileInfo(curFile).fileName());
        LoadStyleNames();
    }
}

void AparenceTab::updateStyles()
{
    int sty = cmbStyle->findText(strStyle, Qt::MatchExactly);
    int sheet = cmbControls->findText(strStylesheet, Qt::MatchExactly);
    cmbStyle->setCurrentIndex(sty);
    cmbControls->setCurrentIndex(sheet);
}

void AparenceTab::setAppStyle()
{
    qApp->setStyle(strStyle);
}

void AparenceTab::setAppStyleSheet()
{
    qApp->setStyleSheet(stylesheet);
}

UserTab::UserTab(QWidget *parent) : QWidget(parent)
{
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

void UserTab::ActiveUserExec(bool mode)
{
    btnIndert->setEnabled(mode);
    btnEdit->setEnabled(mode);
    btnDelete->setEnabled(mode);
    tblUserView->setEnabled(mode);
    ShowUserValue(mode);
}

void UserTab::EditUserExec()
{
    if (idUsr.isValid()){
        QSqlRecord record = users->record(idUsr.row());
        EditUsers *user = new EditUsers(this, record.value(0).toInt());
        connect(user, SIGNAL(refreshGrid(bool)), this, SLOT(ActiveUserExec(bool)));
        user->show();
    }
}

void UserTab::InsertUserExec()
{
    EditUsers *user = new EditUsers(this, 0);
    connect(user, SIGNAL(refreshGrid(bool)), this, SLOT(ActiveUserExec(bool)));
    user->show();
}

void UserTab::DeleteUserExec()
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

void UserTab::getUserIndex(QModelIndex index)
{
    idUsr = index;
}

void UserTab::ShowUser(QModelIndex index)
{
    QSqlRecord record = users->record(index.row());
    EditUsers *user = new EditUsers(this, record.value(0).toInt());
    connect(user, SIGNAL(refreshGrid(bool)), this, SLOT(ShowUserValue(bool)));
    user->show();
}

void UserTab::ShowUserValue(bool type)
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

TemplateTab::TemplateTab(QWidget *parent) : QWidget(parent)
{
    btnAdd = new QPushButton(tr("Añadir"));
    btnDelete = new QPushButton(tr("Eliminar"));

    btnAdd->setMaximumWidth(__BTN_SIZE__);
    btnDelete->setMaximumWidth(__BTN_SIZE__);

    QGridLayout *lytButtonPanel = new QGridLayout;
    lytButtonPanel->setAlignment(Qt::AlignRight);
    lytButtonPanel->addWidget(btnAdd,0,1);
    lytButtonPanel->addWidget(btnDelete,0,2);

    QWidget *widget = new QWidget;
    widget->setMinimumSize(380,290);

    view = new TemplateView(widget, 370, 255);

    connect(btnAdd, SIGNAL(clicked()), this, SLOT(insertTemplates()));
    connect(btnDelete, SIGNAL(clicked()), this, SLOT(deleteTemplates()));
    connect(view,SIGNAL(selectItems(QString)),this, SLOT(getSelection(QString)));

    QVBoxLayout *lytList = new QVBoxLayout;
    lytList->setAlignment(Qt::AlignCenter);
    lytList->addWidget(widget);

    QVBoxLayout *lytTemplate = new QVBoxLayout;
    lblText = new QLabel(tr("Plantillas"));
    lytTemplate->addWidget(lblText);

    lytTemplate->addLayout(lytList);
    lytTemplate->addLayout(lytButtonPanel);

    setLayout(lytTemplate);

    view->show();
}

void TemplateTab::getSelection(QString str)
{
    name = str;
}

void TemplateTab::deleteTemplates()
{
    Folder dir;

    QString path = QString("%1/%2/%3").arg(__APP_PATH__).arg(__DIR_TEMPLATE__).arg(name);
    if (path.contains("/bin")) path.remove("/bin");

    if (dir.remDir(path)){
        QMessageBox::information(this, tr("Eliminación Satisfactoria"),
                                 tr("La plantilla registrada como \"%1\", se ha eliminado "
                                    "\ncorrectamente de %2.").arg(name).arg(__APP_NAME__));
        view->refreshItems();
    }else
        QMessageBox::critical(this, tr("Eliminación Fallida"),
                                 tr("La plantilla registrada como \"%1\", no se ha eliminado \nde %2.").
                                 arg(name).arg(__APP_NAME__));
}

void TemplateTab::insertTemplates()
{
    QString curFile = QFileDialog::getOpenFileName(this, tr("Importar plantilla..."),
                                                   QDir::homePath(),
                                                   tr("Plantillas para %1 (*.alfe.tpt *.zip)").
                                                   arg(__APP_NAME__));

    if (!curFile.isEmpty())
    {
        //QString program = "7z";
        QString program = "unzip";

        QStringList arg;

        QString path = __APP_PATH__;
        if (path.contains("/bin")) path.remove("/bin");
        QString dirName = QFileInfo(curFile).baseName();

        arg << curFile << "-d" << path + __DIR_TEMPLATE__;

        QProcess *unzip = new QProcess;
        unzip->setReadChannel(QProcess::StandardOutput);

        unzip->start(program,arg);

        QMessageBox::information(this, tr("Importar plantilla"),
                                 tr("La plantilla \"%1\" se ha registrado \ncorrectamente de %2.").
                                 arg(QFileInfo(curFile).baseName()).arg(__APP_NAME__));
        view->refreshItems();

    }else
        QMessageBox::critical(this, tr("Importar plantilla"),
                              tr("Operación cancelada por el usuario."));

}

