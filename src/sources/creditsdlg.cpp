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

#include "../includes/creditsdlg.h"
#include "../includes/alarife.h"

CreditsDlg::CreditsDlg(QWidget *parent) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    setWindowTitle(tr("Créditos"));
    setMinimumSize(450,350);
    setMaximumSize(450,350);

    tabPage = new QTabWidget;
    tabPage->addTab(new ProgrammerTab, tr("Desarrolladores"));
    tabPage->addTab(new DocumentationTab, tr("Documentación"));

    btnClose = new QPushButton("Cerrar");
    btnClose->setMaximumWidth(__BTN_SIZE__);
    connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));

    QGridLayout *lytBtn = new QGridLayout;
    lytBtn->addWidget(btnClose,0,2);

    QVBoxLayout *lytCredits = new QVBoxLayout;
    lytCredits->addWidget(tabPage);
    lytCredits->addLayout(lytBtn);

    setLayout(lytCredits);
}

CreditsTabPage::CreditsTabPage(QWidget *parent) : QWidget(parent)
{
    mmText = new QTextEdit;
    mmText->setAlignment(Qt::AlignJustify);
    mmText->setReadOnly(true);

    QVBoxLayout *lytCred = new QVBoxLayout;
    lytCred->addWidget(mmText);
    setLayout(lytCred);
}

void CreditsTabPage::setTabText(QString text)
{
    QFile file(text);
    file.open(QFile::ReadOnly);
    QByteArray data = file.readAll();
    QString str = QString::fromUtf8(data);
    mmText->setHtml(str);
}

ProgrammerTab::ProgrammerTab(CreditsTabPage *parent) : CreditsTabPage(parent)
{
    QString filePath = QString(":/html/credits_%1.htm").arg(QLocale::system().name());
    if (!QFile(filePath).exists()) filePath = ":/html/credits_es_ES.htm";
    setTabText(filePath);
}

DocumentationTab::DocumentationTab(CreditsTabPage *parent) : CreditsTabPage(parent)
{
    QString filePath = QString(":/html/documentation_%1.htm").arg(QLocale::system().name());
    if (!QFile(filePath).exists()) filePath = ":/html/documentation_es_ES.htm";
    setTabText(filePath);
}

TraslaitorTab::TraslaitorTab(CreditsTabPage *parent) : CreditsTabPage(parent)
{
    setTabText(":/html/trasleitor.htm");
}

