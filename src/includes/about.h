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

#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
QT_END_NAMESPACE

//----------------------------------------------------------------------------------------------
class AboutBox : public QDialog{
    Q_OBJECT
    public:
        AboutBox(QWidget *parent = 0);
    private slots:
        void ShowLicense();
        void ShowCredits();
    private:
        QLabel *lblAbout, *lblLogo;
        QPushButton *btnLicense;
        QPushButton *btnCredits;
        QPushButton *btnClose;
};
//----------------------------------------------------------------------------------------------
#endif
