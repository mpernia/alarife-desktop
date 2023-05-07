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

#include <QWebFrame>
#include <QWebPage>
#include <QWebElementCollection>

#include "../includes/htmlindex.h"



void htmlIndex::setPrimaryKey(int primary)
{
    id_primary = primary;
}

void htmlIndex::setFatherValue(int father)
{
    id_padre = father;
}

void htmlIndex::setTitle(QString title)
{
    strTitle = title;
}

void htmlIndex::setSummary(QString summary)
{
    strSummary = summary;
}

void htmlIndex::setLeveList(QString level)
{
    strLeveList = level;
}

void htmlIndex::setArticleList(QString article)
{
    strArticleList = article;
}

void htmlIndex::setVMenuArea(QString text)
{
    strVmenu = text;
}

void htmlIndex::setHMenuArea(QString text)
{
    strHmenu = text;
}

void htmlIndex::setSlideShow(QString text)
{
    strSlide = text;
}

void htmlIndex::replaceArea(QString findStr, QString newStr)
{
    htmlText.replace(findStr, newStr, Qt::CaseSensitive);
}

void htmlIndex::clear()
{
    strVmenu.clear();
    strHmenu.clear();
    strTitle.clear();
    strSummary.clear();
    strLeveList.clear();
    strArticleList.clear();
    strSlide.clear();
}

void htmlIndex::changeValueElements()
{
    changeMediaLink(strSummary);
    htmlText.replace("<titulo>",strTitle, Qt::CaseSensitive);
    htmlText.replace("<nombre>", strTitle, Qt::CaseSensitive);
    htmlText.replace("<resumen>", strSummary, Qt::CaseSensitive);
    replaceArea(selectArea("emenu"), strVmenu);
    replaceArea(selectArea("niveles_cabecera"),strHmenu);

    if (strLeveList.isEmpty())
        replaceArea(selectArea("listan_vacia"),"");
    else
        replaceArea(selectArea("lista_niveles"),strLeveList);

    if (strArticleList.isEmpty())
        replaceArea(selectArea("listaa_vacia"),"");
    else
        replaceArea(selectArea("lista_articulos"),strArticleList);

    htmlText.replace("<slideshow>", strSlide, Qt::CaseSensitive);
    htmlText.replace("<estadistica>", "<span id=\"estadistica\"><script language=\"javascript\">stadistic();</script></span>");
}

void htmlIndex::changeMediaLink(QString &text)
{
    QString html = text.simplified();

    QWebPage page;
    page.mainFrame()->setHtml(html);

    QWebElementCollection aList = page.mainFrame()->findAllElements("a");

    foreach(QWebElement alink, aList)
    {
        QString type = alink.attribute("type");
        QString swap = "";
        if (type == "video" && (mode == 2 || mode == 3))
        {
            swap = TAG_VIDEO.arg(alink.attribute("href")).
                             arg(alink.attribute("alt")).
                             arg(alink.attribute("title"));

        }

        if (type == "audio" && (mode == 1 || mode == 3))
        {
            swap = TAG_AUDIO.arg(alink.attribute("href")).
                             arg(alink.attribute("alt")).
                             arg(alink.attribute("title"));
        }

        if (!swap.isEmpty())
        {
            alink.replace(swap);
        }
    }

    text = page.mainFrame()->toHtml();
}

