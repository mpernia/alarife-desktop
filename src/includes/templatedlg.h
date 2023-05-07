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

#ifndef TEMPLATEDLG_H
#define TEMPLATEDLG_H

#include <QDialog>

#include "templateview.h"

//----------------------------------------------------------------------------------------------
class TemplateDlg : public QDialog{
Q_OBJECT
public:
    explicit TemplateDlg(QWidget *parent = 0);
    void setTemplateName(QString&);
protected:
    void closeEvent(QCloseEvent *event);
signals:
    void getSelecction(QString);
public slots:
    void showSelecctionValue(QString);
private:
    TemplateView *view;
    QString name;
};

#endif // TEMPLATEDLG_H
