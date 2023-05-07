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

#include "../includes/insertmediadlg.h"

InsertMediaDlg::InsertMediaDlg(QWidget *parent, WebView *wv, Book *encyc) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    setWindowTitle("Medias de la enciclopedia");
    setMinimumSize(680, 550);
    setMaximumSize(680, 550);

    tab = new InsertMediaPanel(this, wv, encyc);
    tab_2 = new MediaOption(this, encyc);

    tabWidget = new QTabWidget;
    tabWidget->addTab(tab, tr("Insertar Medias"));
    tabWidget->addTab(tab_2, tr("Registrar Medias"));

    QVBoxLayout *lytMain = new QVBoxLayout;
    lytMain->addWidget(tabWidget);
    setLayout(lytMain);

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(updateModel()));
}


void InsertMediaDlg::updateModel()
{}

