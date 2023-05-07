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

#include <iostream>

using namespace std;

#include "../includes/dinarray.h"

DinArray::DinArray(int row)
{
    nrow = row;
    ncol = 2;
    array = new int*[nrow];
    for (int i=0; i<nrow; i++) array[i] = new int[ncol];
    for(int i = 0; i < nrow; i++) array[i][1]= -1;
}

DinArray::~DinArray()
{
    for(int i=0; i<nrow; i++) delete [] array[i];
    delete [] array;
}

int DinArray::Count()
{
    return nrow;
}

bool DinArray::isEmpty()
{
    return nrow;
}

void DinArray::Sorting()
{
    int swap;
    for (int i = 1; i <= nrow - 1; i++)
        for (int j = 0; j <= nrow - 2; j++)
            if (array[j][0] > array[j+1][0])
            {
                swap = array[j][0];
                array[j][0] = array[j+1][0];
                array[j+1][0] = swap;
            }
}

int DinArray::getPosId(int value)
{
    for (int i = 0; i < nrow; i++){
        if (array[i][0] == value){
            return i;
        }
    }
    return 0;
}

