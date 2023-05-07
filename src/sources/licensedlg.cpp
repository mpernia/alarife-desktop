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

#include "../includes/licensedlg.h"
#include "../includes/alarife.h"

LicenseDlg::LicenseDlg(QWidget *parent) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    setWindowTitle(QObject::trUtf8("Licencia."));
    setMinimumSize(540, 250);

    QString filePath = QString(":/html/license_%1.htm").arg(QLocale::system().name());

    if (!QFile(filePath).exists()) filePath = ":/html/license_es_ES.htm";

    QFile file(filePath);
    file.open(QFile::ReadOnly);
    QByteArray data = file.readAll();
    QString str = QString::fromUtf8(data);

    mmLicense = new QTextEdit;
    mmLicense->setHtml(str);
    mmLicense->setAlignment(Qt::AlignJustify);
    mmLicense->setReadOnly(true);

    btnClose = new QPushButton(tr("Cerrar"));
    btnClose->setMaximumWidth(__BTN_SIZE__);
    connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));

    QGridLayout *lytBtn = new QGridLayout;
    lytBtn->addWidget(btnClose,0,2);

    QVBoxLayout *lytLicense = new QVBoxLayout;
    lytLicense->addWidget(mmLicense);
    lytLicense->addLayout(lytBtn);
    lytLicense->setAlignment(Qt::AlignCenter);

    setLayout(lytLicense);

}
