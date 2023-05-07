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

#include <QRegExp>
#include <QDateTime>
#include <QWebFrame>
#include <QWebPage>
#include <QWebElementCollection>

#include "../includes/htmlarticles.h"
#include "../includes/strfunction.h"

void htmlArticles::setContent(QString content)
{
    strReference = selectArea("referencia");
    stripTag(strReference, "referencia");
    replaceArea(selectArea("referencia"),"");

    strContent = content;
    strContent.replace("jscripts/tiny_mce/plugins/advimage", "..", Qt::CaseSensitive);
    strContent.replace("src=\"medias/", "src=\"../medias/", Qt::CaseSensitive);
    qDebug() << "mode: " << mode << endl;
    changeMediaLink(strContent);
}

void htmlArticles::setRelationsAuthors(QString text)
{
    strAuthors = text;
}

void htmlArticles::setRelationsArticles(QString text)
{
    strArticles = text;
}

void htmlArticles::setSourceData(QString text)
{
    strSource = text;
}

void htmlArticles::setDates(QString created, QString modified)
{
    if (!created.isEmpty() || created != "0000-00-00")
    {
        strDateCreated = QString("Publicado: %1.").arg(getPrintDateTime(created, "dddd dd 'de' MMMM 'del' yyyy"));
    }

    if ((!modified.isEmpty() || modified != "0000-00-00") && created != modified)
    {
        strDateModified = QString("&Uacute;ltima modificaci&oacute;n: %1.").
                          arg(getPrintDateTime(modified, "dddd dd 'de' MMMM 'del' yyyy"));
    }
}

void htmlArticles::clear()
{
    strAuthors.clear();
    strContent.clear();
    strArticles.clear();
    strSource.clear();
    strDateCreated.clear();
    strDateModified.clear();
}

void htmlArticles::changeValueElements()
{
    htmlText.replace("<titulo>", QString("%1 [%2]").arg(winTitle).arg(strTitle), Qt::CaseSensitive);
    htmlText.replace("<titulo2>", winTitle, Qt::CaseSensitive);
    htmlText.replace("<titulo_articulo>", strTitle, Qt::CaseSensitive);
    htmlText.replace("<resumen_articulo>", QString("<b>%1</b>").arg(strSummary), Qt::CaseSensitive);

    if (strAuthors.isEmpty())
    {
        replaceArea(selectArea("autor_vacio"), "<td><br></td>");
    }else{
        htmlText.replace("<autores>", strAuthors, Qt::CaseSensitive);
    }

    htmlText.replace("<texto_articulo>",strContent, Qt::CaseSensitive);
    htmlText.replace("<fuente>", strSource, Qt::CaseSensitive);
    htmlText.replace("<publicado>", strDateCreated, Qt::CaseSensitive);
    htmlText.replace("<modificado>", strDateModified, Qt::CaseSensitive);
    htmlText.replace(selectArea("emenu"),strVmenu, Qt::CaseSensitive);
    replaceArea(selectArea("niveles_cabecera"),strHmenu);

    if (strArticles.isEmpty()) {
        replaceArea(selectArea("relacionados_vacio"), "");
    } else {
        replaceArea(selectArea("lista_articulos_relacionados"),strArticles);
    }
    htmlText.replace("<slideshow>", strSlide, Qt::CaseSensitive);
}

void htmlArticles::insertReferences(int id, int link, QString title, QString summary, QString text)
{
    QString target = strReference;

    if (!title.isEmpty() && !summary.isEmpty())
    {
        target.replace("<ref_titulo>", title, Qt::CaseSensitive);
        target.replace("<ref_resumen>", summary, Qt::CaseSensitive);
    }else{
        target.remove("title=\"<ref_titulo>\"", Qt::CaseSensitive);
        if (summary.isEmpty()) target.replace("<ref_resumen>", title, Qt::CaseSensitive);
        if (title.isEmpty()) target.replace("<ref_resumen>", summary, Qt::CaseSensitive);
    }

    target.replace("<ref_texto>", text, Qt::CaseSensitive);

    if (link > 0)
        target.replace("<ref_link>", QString("articulo_%1.htm").arg(link), Qt::CaseSensitive);
    else
        target.replace("onclick=\"javascript:window.location='<ref_link>'\"", "", Qt::CaseSensitive);

    QString reg = selectReferenceArea(strContent, id);
    if (!reg.isEmpty()) strContent.replace(reg, target);
}
