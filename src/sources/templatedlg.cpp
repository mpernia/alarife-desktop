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

#include "../includes/alarife.h"
#include "../includes/templatedlg.h"


TemplateDlg::TemplateDlg(QWidget *parent) : QDialog(parent)
{

    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__); 

    setMinimumSize(380, 400);
    setMaximumSize(380, 400);

    setWindowTitle(tr("Plantillas"));
    view = new TemplateView(this, 360, 350);
    view->show();

    connect(view,SIGNAL(selectItems(QString)),this, SLOT(showSelecctionValue(QString)));

    QLabel *lblTemplate = new QLabel(tr("Plantillas registradas:"));
    QVBoxLayout *lytTemplate = new QVBoxLayout;
    lytTemplate->addWidget(lblTemplate);
    lytTemplate->addWidget(view);

    setLayout(lytTemplate);
}

void TemplateDlg::closeEvent(QCloseEvent *event)
{
    emit getSelecction(name);
    event->accept();    
}

void TemplateDlg::showSelecctionValue(QString filePath)
{
    QString path = __APP_PATH__;
    if (path.contains("/bin")) path.remove("/bin");

    QSettings settings(path + __DIR_TEMPLATE__ + filePath + "/information", QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

    settings.beginGroup("template");
    QString strAuthor = settings.value("author").toString(),
            strVersion = settings.value("version").toString(),
            strCopyright = settings.value("copyright").toString(),
            strComment = settings.value("comment").toString();
    settings.endGroup();

    strAuthor.replace(" | ", ", ", Qt::CaseSensitive);

    if (!strVersion.isEmpty()) strVersion.prepend(tr("\nVersión: "));

    int button = QMessageBox::question(this, tr("Selección de platilla"),
                                       tr("Los datos de la plantilla seleccionada son:\n"
                                          "Nombre: %1 %2\nAutor(es): %3 \nCopyright (C): "
                                          "%4\nDetalles: %5\n\nDesea guardar los cambios?").
                                          arg(filePath).
                                          arg(strVersion).
                                          arg(strAuthor).
                                          arg(strCopyright).
                                          arg(strComment),
                                       QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::Yes)
    {
        name = filePath;
        this->close();
    }
}


