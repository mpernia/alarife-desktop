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

#include "../includes/dlgadmintemplates.h"

dlgAdminTemplates::dlgAdminTemplates(QWidget *parent) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);
    setWindowTitle(tr("Administrar plantillas"));

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

void dlgAdminTemplates::getSelection(QString str)
{
    name = str;
}

void dlgAdminTemplates::deleteTemplates()
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

void dlgAdminTemplates::insertTemplates()
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

