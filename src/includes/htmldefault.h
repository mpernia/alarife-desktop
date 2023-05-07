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

#ifndef HTMLDEFAULT_H
#define HTMLDEFAULT_H

#include "html.h"

class htmlDefault : public html
{
public:
    htmlDefault() : html(){}
    virtual ~htmlDefault() {}
    void setMainFrame(QString);
    void clear(){}
    void changeValueElements();
private:
    QString strMainFrame;
};

#endif // HTMLDEFAULT_H
