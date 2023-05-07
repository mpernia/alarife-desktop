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

#include <QSpacerItem>
#include "../includes/dlgwaitproc.h"
#include "../includes/alarife.h"

dlgWaitProc::dlgWaitProc(QWidget *parent) : QDialog(parent){
    QWidget::setWindowFlags(Qt::Dialog | Qt::WindowTitleHint|
                            Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);

    setMinimumSize(300, 70);
    setMaximumSize(300, 70);
    setWindowTitle(" ");

    closed = false;

    QLabel *lblIcon = new QLabel;
    lblIcon->setPixmap(QPixmap(":/images/dialog-warning.png"));
    lblIcon->setAlignment(Qt::AlignCenter);


    QLabel *lblText = new QLabel(tr("<b>Por favor espere...</b>"));

    QSpacerItem *hSpacer  = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QHBoxLayout *lytMain = new QHBoxLayout;
    lytMain->addWidget(lblIcon);
    lytMain->addWidget(lblText);
    lytMain->addItem(hSpacer);

    setLayout(lytMain);
}

void dlgWaitProc::setCanClose(bool mode){
    closed = mode;
}

void dlgWaitProc::closeEvent(QCloseEvent *event){
    if (closed == true) event->accept();
    else event->ignore();
}
