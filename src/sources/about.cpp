/****************************************************************************
**
** Copyleft: 2010. Maikel Enrique Pernia Matos.
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

#include "../includes/about.h"
#include "../includes/alarife.h"
#include "../includes/licensedlg.h"
#include "../includes/creditsdlg.h"

//----------------------------------------------------------------------------------------------
AboutBox::AboutBox(QWidget *parent)  : QDialog(parent){
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    setWindowTitle(tr("Acerca de..."));
    setMinimumSize(400, 300);
    setMaximumSize(400, 300);

    btnLicense = new QPushButton(tr("Licencia"));
    btnCredits = new QPushButton(tr("Créditos"));
    btnClose = new QPushButton(tr("Cerrar"));

    btnLicense->setMaximumWidth(__BTN_SIZE__);
    btnCredits->setMaximumWidth(__BTN_SIZE__);
    btnClose->setMaximumWidth(__BTN_SIZE__);

    connect(btnLicense, SIGNAL(clicked()), this, SLOT(ShowLicense()));
    connect(btnCredits, SIGNAL(clicked()), this, SLOT(ShowCredits()));
    connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));

    QGridLayout *lytButtonPanel = new QGridLayout;
    lytButtonPanel->setAlignment(Qt::AlignCenter);
    lytButtonPanel->addWidget(btnLicense,0,1);
    lytButtonPanel->addWidget(btnCredits,0,2);
    lytButtonPanel->addWidget(btnClose,0,3);

    QVBoxLayout *lytAbout = new QVBoxLayout;
    lblLogo = new QLabel("");
    lblLogo->setPixmap(QPixmap(":/images/alarife_logo.png").
                        scaled(QSize(210,120),
                               Qt::IgnoreAspectRatio,
                               Qt::FastTransformation));
    lblLogo->setMinimumHeight(125);
    lblLogo->setAlignment(Qt::AlignCenter);
    lytAbout->addWidget(lblLogo);
    lblAbout = new QLabel(tr("<font size=4><b>Versión: " __APP_VERSION__ "</b></font>"));
    lblAbout->setAlignment(Qt::AlignCenter);
    lytAbout->addWidget(lblAbout);
    lblAbout = new QLabel(tr(__APP_NAME__ " es una herramienta para el desarrollo de enciclopedias "
                             "y sitios web temáticos<br>Desarrollado por: Manzinella Digital. "
                             "Manzanillo. Cuba."));
    lblAbout->setAlignment(Qt::AlignJustify);
    lblAbout->setWordWrap(true);
    lytAbout->addWidget(lblAbout);
    lytAbout->addLayout(lytButtonPanel);

    setLayout(lytAbout);
 }
//----------------------------------------------------------------------------------------------
void AboutBox::ShowLicense(){
    LicenseDlg *license = new LicenseDlg(this);
    license->showNormal();
}
//----------------------------------------------------------------------------------------------
void AboutBox::ShowCredits(){
    CreditsDlg *credits = new CreditsDlg(this);
    credits->show();
}
