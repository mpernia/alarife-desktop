#include <QtGui>
#include <QSqlDatabase>
#include <QTimer>


#include "../includes/dlgadminstyles.h"

dlgAdminStyles::dlgAdminStyles(QWidget *parent) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);
    setWindowTitle(tr("Administrar apariencia"));

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

void dlgAdminStyles::closeEvent(QCloseEvent *event)
{
    if (isModifique == true)
    {
        saveAparence();
    }

    event->accept();
}

void dlgAdminStyles::ChangeStyle(QString style)
{
    if (strStyle != style)
    {
        isModifique = true;
        strStyle = style;
    }
}

void dlgAdminStyles::ActiveControlStyle(bool active)
{
    cmbStyle->setEnabled(active);
    cmbControls->setEnabled(active);

    btnDelete->setEnabled(active);
    btnNew->setEnabled(active);
}

void dlgAdminStyles::LoadStyleNames()
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

void dlgAdminStyles::LoadStyleFiles(QString name)
{
    QString path = QString("%1/styles/").arg(__APP_PATH__);
    if (path.contains("/bin")) path.remove("/bin");

    if (strStylesheet != name){

        isModifique = true;
        strStylesheet = name;
    }
}

void dlgAdminStyles::saveAparence()
{
    QSettings Preference("Alarife", "Preference");
    Preference.setValue("style", strStyle);
    Preference.setValue("stylesheet", strStylesheet);
}

void dlgAdminStyles::deleteStyle()
{
    QString path = QString("%1/styles/").arg(__APP_PATH__);
    if (path.contains("/bin")) path.remove("/bin");
    if (!curFile.isEmpty())
    {
        QFile::remove(path + curFile);
        LoadStyleNames();
    }
}

void dlgAdminStyles::insertStyle()
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

void dlgAdminStyles::updateStyles()
{
    int sty = cmbStyle->findText(strStyle, Qt::MatchExactly);
    int sheet = cmbControls->findText(strStylesheet, Qt::MatchExactly);
    cmbStyle->setCurrentIndex(sty);
    cmbControls->setCurrentIndex(sheet);
}

void dlgAdminStyles::setAppStyle()
{
    qApp->setStyle(strStyle);
}

void dlgAdminStyles::setAppStyleSheet()
{
    qApp->setStyleSheet(stylesheet);
}


