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

#include "../includes/htmllevels.h"

void htmlLevels::setPictureCode(QString image)
{
    if (!image.isEmpty()) strImage = image;
    else strImage.clear();
}

void htmlLevels::changeValueElements()
{
    changeMediaLink(strSummary);
    htmlText.replace("<titulo>", QString("%1 [%2]").arg(winTitle).arg(strTitle), Qt::CaseSensitive);
    htmlText.replace("<titulo2>", winTitle, Qt::CaseSensitive);
    htmlText.replace("<imagen>", strImage, Qt::CaseSensitive);
    htmlText.replace("<nombre>", strTitle, Qt::CaseSensitive);
    htmlText.replace("<resumen>", strSummary, Qt::CaseSensitive);
    replaceArea(selectArea("emenu"), strVmenu);
    replaceArea(selectArea("niveles_cabecera"),strHmenu);

    if (strLeveList.isEmpty())
    {
        replaceArea(selectArea("listan_vacia"),"");
    } else {
        replaceArea(selectArea("lista_niveles"),strLeveList);
    }

    if (strArticleList.isEmpty())
    {
        replaceArea(selectArea("listaa_vacia"),"");
    } else {
        replaceArea(selectArea("lista_articulos"),strArticleList);
    }
    htmlText.replace("<slideshow>", strSlide, Qt::CaseSensitive);
}



