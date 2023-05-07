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



#include "../includes/html.h"
#include "../includes/strfunction.h"
#include "../includes/alarife.h"


html::html()
{
}

html::~html()
{
    pathOpen.clear();
    pathSave.clear();
}

void html::setOpenPath(QString openPath)
{
    pathOpen = openPath;
}

void html::setSavePath(QString savePath)
{
    pathSave = savePath;
}

void html::setTitleWindows(QString title)
{
    winTitle = title;
}

void html::openHtml()
{
    if (QFile::exists(pathOpen))
    {
        QFile inFile(pathOpen);
        inFile.open(QIODevice::ReadOnly);

        clearHtml();
        QTextStream inStream(&inFile);
        while (!inStream.atEnd()) htmlText.append(inStream.readLine() + "\n");
        htmlText.replace("charset=iso-8859-1", "charset=UTF-8");
        htmlText.replace("charset=\"iso-8859-1\"", "charset=\"UTF-8\"");
        htmlText.replace("<generado>", QString("Generado con %1 &copy; 2012 - 2019").arg(__APP_NAME__));
        htmlText.replace("<fecha_generado>", getPrintDateTime(getActualDataTime(), "dd 'de' MMMM 'del' yyyy"), Qt::CaseSensitive);
        htmlText.replace("Alarife Web 3.0 (C) 2010", QString("%1 &copy; 2012").arg(__APP_NAME__));
        htmlText.replace("vfernandez@mzo.grm.desoft.cu", "herlin.matos@gmail.com");
        htmlText.replace("Ing. V&iacute;ctor Fern&aacute;ndez Bertot.", "Ing. Herlin Reinier Matos Lastres.");
        htmlText.replace("yordankis2001@yahoo.com", "meperniamatos@gmail.com");
        htmlText.replace("Tec. Yordankis Fernandez Pacheco.", "Ing. Maikel Enrique Pern&iacute;a Matos.");
        htmlText.replace("Lic. Yordankis Fernandez Pacheco.", "Ing. Maikel Enrique Pern&iacute;a Matos.");
    }
}

void html::saveHtml()
{
    QFile outFile(pathSave);
    outFile.open(QIODevice::ReadWrite);

    QTextStream outStream(&outFile);
    outStream.setCodec("UTF-8");
    outStream << htmlText;
}

void html::clearHtml()
{
    htmlText.clear();
}

void html::closeHtml()
{
    clearHtml();
    pathOpen.clear();
    pathSave.clear();
}

QString html::getHtmlText()
{
    return htmlText;
}

QString html::selectArea(QString tag)
{
    if(htmlText.toStdString().length() == 0) return "";
    size_t beg = htmlText.toStdString().find("<"+tag.toStdString()+">");
    size_t end = htmlText.toStdString().find("</"+tag.toStdString()+">");
    if(beg == std::string::npos) return "";
    return QString::fromStdString(std::string(htmlText.toStdString(), beg,
                                              end - beg + tag.toStdString().length()+3));
}

void html::stripTag(QString &text, QString tag)
{
    text.replace(QString("<%1>").arg(tag), "");
    text.replace(QString("</%1>").arg(tag), "");
}



