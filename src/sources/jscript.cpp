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

#include "../includes/jscript.h"
#include "../includes/sqlfunctions.h"
#include "../includes/strfunction.h"


void jScript::setOpenPath(QString openPath)
{
    pathOpen = openPath;
}

void jScript::setSavePath(QString savePath)
{
    pathSave = savePath;
}

void jScript::openJScript()
{
    if (QFile::exists(pathOpen))
    {
        QFile inFile(pathOpen);
        inFile.open(QIODevice::ReadOnly);

        jscriptText.clear();
        QTextStream inStream(&inFile);
        while (!inStream.atEnd()) jscriptText.append(inStream.readLine() + "\n");
    }
}

void jScript::saveJScript()
{
    QFile outFile(pathSave);
    outFile.open(QIODevice::ReadWrite);

    QTextStream outStream(&outFile);
    outStream.setCodec("UTF-8");
    outStream << jscriptText;
}

void jScript::changeValueElements()
{
    QString array = "",
            title_array = "",
            page_array  = "",
            titulo,
            resumen;
    int row = 0;

    QSqlQuery *qryScript = new QSqlQuery;

    qryScript->exec(QString("SELECT id_nivel, nombre, resumen from nivel where enciclopedia = %1").arg(index));

    while (qryScript->next())
    {
        titulo = setQuote(qryScript->value(1).toString().simplified());
        resumen = strToPlainText(qryScript->value(2).toString()).simplified();
        QString resumen1 = strHtmlEncode(resumen);
        resumen = strStripCurses(resumen);


        array.append(QString("t[%1][0] = ('nivel_%2.htm');\n").arg(row).arg(qryScript->value(0).toInt()));
        array.append(QString("t[%1][1] = ('');\n").arg(row));
        array.append(QString("t[%1][2] = ('%2');\n").arg(row).arg(setUtfToHtml(titulo)));
        array.append(QString("t[%1][3] = ('%2');\n").arg(row).arg(resumen1));
        array.append(QString("t[%1][4] = ('');\n").arg(row));
        array.append(QString("t[%1][5] = ('%2');\n").arg(row).arg(titulo).toLower());
        array.append(QString("t[%1][6] = ('%2');\n\n").arg(row).arg(resumen).toLower());

        if (title_array == "" && page_array == "")
        {
            title_array.append(QString("var title_array = ['%1'").arg(strHtmlDecode(titulo.toLower())));
            page_array.append(QString("var page_array = [\n['%1', 'nivel_%2.htm']").arg(strHtmlDecode(titulo.toLower())).arg(qryScript->value(0).toInt()));
        }else{
            title_array.append(QString(", '%1'").arg(strHtmlDecode(titulo.toLower())));
            page_array.append(QString(",\n['%1', 'nivel_%2.htm']").arg(strHtmlDecode(titulo.toLower())).arg(qryScript->value(0).toInt()));
        }
        row++;

    }

    qryScript->clear();

    qryScript->exec(QString("SELECT id_articulo, claves, nombre, resumen FROM articulo WHERE enciclopedia = %1").arg(index));

    while (qryScript->next())
    {
        titulo = setQuote(qryScript->value(2).toString().simplified());
        resumen = setQuote(qryScript->value(3).toString().simplified());

        array.append(QString("t[%1][0] = ('articulo_%2.htm');\n").arg(row).arg(qryScript->value(0).toInt()));
        array.append(QString("t[%1][1] = ('%2');\n").arg(row).arg(qryScript->value(1).toString().simplified()));
        array.append(QString("t[%1][2] = ('%2');\n").arg(row).arg(setUtfToHtml(titulo)));
        array.append(QString("t[%1][3] = ('%2');\n").arg(row).arg(setUtfToHtml(resumen)));
        array.append(QString("t[%1][4] = ('%2');\n").arg(row).arg(setAuthorList(qryScript->value(0).toInt())));
        array.append(QString("t[%1][5] = ('%2');\n").arg(row).arg(titulo).toLower());
        array.append(QString("t[%1][6] = ('%2');\n\n").arg(row).arg(resumen).toLower());

        if (title_array == "" && page_array == "")
        {
            title_array.append(QString("var title_array = ['%1'").arg(strHtmlDecode(titulo.toLower())));
            page_array.append(QString("var page_array = [\n['%1', 'articulo_%2.htm']").arg(strHtmlDecode(titulo.toLower())).arg(qryScript->value(0).toInt()));
        }else{
            title_array.append(QString(", '%1'").arg(strHtmlDecode(titulo.toLower())));
            page_array.append(QString(",\n['%1', 'articulo_%2.htm']").arg(strHtmlDecode(titulo.toLower())).arg(qryScript->value(0).toInt()));
        }
        row++;
    }
    title_array.append("];\n\n");
    page_array.append( "];\n\n");

    jscriptText.replace("<cant>",QString("%1").arg(row),Qt::CaseSensitive);
    jscriptText.replace(selectArea("datos"), array, Qt::CaseSensitive);

    jscriptText.replace("<title_array>", title_array, Qt::CaseSensitive);
    jscriptText.replace("<page_array>", page_array, Qt::CaseSensitive);
}

void jScript::changeValueGallery()
{
    QSqlQuery *qryScript = new QSqlQuery;

    qryScript->exec(QString("SELECT galeria.id_adjunto, adjuntos.extension, "
                            "adjuntos.descripcion FROM galeria INNER JOIN adjuntos "
                            "ON galeria.id_adjunto = adjuntos.id_adjunto WHERE "
                            "id_enciclopedia = %1 ORDER BY adjuntos.descripcion ASC").arg(index));

    qryScript->first();
    QString gallery = QString("['medias/%1.%2','%3']").
                          arg(qryScript->value(0).toString()).
                          arg(qryScript->value(1).toString()).
                          arg(qryScript->value(2).toString());

    while (qryScript->next())
    {
        gallery.append(QString(",['medias/%1.%2','%3']").
                               arg(qryScript->value(0).toString()).
                               arg(qryScript->value(1).toString()).
                               arg(qryScript->value(2).toString())
                               );
        sqlInsertRecord_N("media",
                        "nombre, extension",
                        QString("%1, <&~%2~&>").
                               arg(qryScript->value(0).toInt()).
                               arg(qryScript->value(1).toString()));
    }
    jscriptText.replace("<datos>", gallery, Qt::CaseSensitive);
}

QString jScript::selectArea(QString tag)
{
    if(jscriptText.toStdString().length() == 0) return "";
    size_t beg = jscriptText.toStdString().find("<"+tag.toStdString()+">");
    size_t end = jscriptText.toStdString().find("</"+tag.toStdString()+">");
    if(beg == std::string::npos) return "";
    return QString::fromStdString(std::string(jscriptText.toStdString(), beg,
                                              end - beg + tag.toStdString().length()+3));
}

QString jScript::setUtfToHtml(QString text)
{
    QString swap = text;
    swap.replace(QObject::tr("á"), QObject::tr("&aacute;"));
    swap.replace(QObject::tr("Á"), QObject::tr("&Aacute;"));
    swap.replace(QObject::tr("é"), QObject::tr("&eacute;"));
    swap.replace(QObject::tr("É"), QObject::tr("&Eacute;"));
    swap.replace(QObject::tr("í"), QObject::tr("&iacute;"));
    swap.replace(QObject::tr("Í"), QObject::tr("&Iacute;"));
    swap.replace(QObject::tr("ó"), QObject::tr("&oacute;"));
    swap.replace(QObject::tr("Ó"), QObject::tr("&Oacute;"));
    swap.replace(QObject::tr("ú"), QObject::tr("&uacute;"));
    swap.replace(QObject::tr("Ú"), QObject::tr("&Uacute;"));
    swap.replace(QObject::tr("ñ"), QObject::tr("&ntilde;"));
    swap.replace(QObject::tr("Ñ"), QObject::tr("&Ntilde;"));
    return swap;
}

QString jScript::setQuote(QString text)
{
    QString quote = text;
    quote.replace(QObject::tr("'"), QObject::tr("&#039;"));
    quote.replace(QObject::tr("\""), QObject::tr("&quot;"));
    quote.replace(QObject::tr("«"), QObject::tr("&laquo;"));
    quote.replace(QObject::tr("»"), QObject::tr("&raquo;"));

    return quote;
}

QString jScript::setAuthorList(int index)
{
    QSqlQuery *qryAuthos = new QSqlQuery;
    QString names;

    qryAuthos->exec(QString("SELECT autor.nombre FROM autor INNER JOIN articuloautores "
                            "ON articuloautores.autor = autor.id_autor INNER JOIN articulo "
                            "ON articulo.id_articulo = articuloautores.articulo WHERE "
                            "articulo.id_articulo = %1 ORDER BY autor.nombre;").arg(index));

    while (qryAuthos->next())
    {
        names.append(qryAuthos->value(0).toString() + " ");
    }

    names.replace(".", "", Qt::CaseSensitive);
    names.replace("+", "", Qt::CaseSensitive);

    return names.toLower();
}

