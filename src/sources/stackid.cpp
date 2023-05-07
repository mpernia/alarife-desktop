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

#include "../includes/stackid.h"


void StackId::PushId(int index, int id)
{
    Reg *aux = new Reg;
    aux->father = index;
    aux->number = id;
    aux->next = last;
    last = aux;
    ++numReg;
}

void StackId::PopId()
{
    if (numReg > 1){
        Reg *aux = last;
        last = last->next;
        delete aux;
        --numReg;
    }
}


