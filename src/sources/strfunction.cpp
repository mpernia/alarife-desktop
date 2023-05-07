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

#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QTextStream>
#include <QTextEdit>
#include <QDebug>

#include "../includes/strfunction.h"


QString getActualDataTime()
{
    QDateTime now = QDateTime::currentDateTime();
    return now.toString("yyyy-MM-dd hh:mm:ss");
}

QString getPrintDateTime(QString str, QString format)
{
    QDateTime date;
    date = QVariant(str).toDateTime();
    return date.toString(format);
}

QString getStripFileName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

QString getSubString(QString str, QString str1, QString str2, int size)
{
    if(str.toStdString().length() == 0) return "";
    size_t beg = str.toStdString().find(str1.toStdString());
    size_t end = str.toStdString().find(str2.toStdString());
    if(beg == std::string::npos) return "";
    return QString::fromStdString(std::string(str.toStdString(), beg, end - beg + size));
}

QString getFileSize(quint64 q)
{
    float f0 = q, f1;
    int i;
    QString s0, s1, s2, s3;
    if (f0 < 1048576)
    {
        i = 1024;
        s0 = "Kb";
    }else{
        i = 1048576;
        s0 = "Mb";
    }
    s1 = QString("%1").arg(f0/i);

    size_t beg = 0;
    size_t end = s1.toStdString().find(".");

    s2 = QString::fromStdString(std::string(s1.toStdString(), beg, end +3));

    s3 = s2.right(1);
    s2.truncate(s2.length()-1);
    f1 = s2.toFloat();
    if (s3.toInt() > 5) f1 += 0.1;

    return QString("%1 %2").arg(f1).arg(s0);
}

QString getCodeToHtml(QString path)
{
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return "";

    QString text;

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        line.replace(QObject::tr("&lt;"), QObject::tr("<"));
        line.replace(QObject::tr("&gt;"), QObject::tr(">"));
        line.replace(QObject::tr("&iquest;"), QObject::tr("¿"));
        line.replace(QObject::tr("&iexcl;"), QObject::tr("¡"));
        line.replace(QObject::tr("&amp;"), QObject::tr("&"));
        line.replace(QObject::tr("&nbsp;"), QObject::tr(" "));
        line.replace(QObject::tr("&laquo;"), QObject::tr("«"));
        line.replace(QObject::tr("&raquo;"), QObject::tr("»"));
        line.replace(QObject::tr("&#039;"), QObject::tr("\''"));
        line.replace(QObject::tr("&quot;"), QObject::tr("\""));

        line.replace(QObject::tr("&aacute;"), QObject::tr("á"));
        line.replace(QObject::tr("&Aacute;"), QObject::tr("Á"));
        line.replace(QObject::tr("&eacute;"), QObject::tr("é"));
        line.replace(QObject::tr("&Eacute;"), QObject::tr("É"));
        line.replace(QObject::tr("&iacute;"), QObject::tr("í"));
        line.replace(QObject::tr("&Iacute;"), QObject::tr("Í"));
        line.replace(QObject::tr("&oacute;"), QObject::tr("ó"));
        line.replace(QObject::tr("&Oacute;"), QObject::tr("Ó"));
        line.replace(QObject::tr("&uacute;"), QObject::tr("ú"));
        line.replace(QObject::tr("&Uacute;"), QObject::tr("Ú"));
        line.replace(QObject::tr("&ntilde;"), QObject::tr("ñ"));
        line.replace(QObject::tr("&Ntilde;"), QObject::tr("Ñ"));
        line.replace(QObject::tr("&Ntilde;"), QObject::tr("Ñ"));
        line.replace(QObject::tr("&agrave;"), QObject::tr("à"));
        line.replace(QObject::tr("&egrave;"), QObject::tr("è"));
        line.replace(QObject::tr("&igrave;"), QObject::tr("ì"));
        line.replace(QObject::tr("&ograve;"), QObject::tr("ò"));
        line.replace(QObject::tr("&ugrave;"), QObject::tr("ù"));
        line.replace(QObject::tr("&Agrave;"), QObject::tr("À"));
        line.replace(QObject::tr("&Egrave;"), QObject::tr("È"));
        line.replace(QObject::tr("&Igrave;"), QObject::tr("Ì"));
        line.replace(QObject::tr("&Ograve;"), QObject::tr("Ò"));
        line.replace(QObject::tr("&Ugrave;"), QObject::tr("Ù"));
        line.replace(QObject::tr("&acirc;"), QObject::tr("â"));
        line.replace(QObject::tr("&ecirc;"), QObject::tr("ê"));
        line.replace(QObject::tr("&icirc;"), QObject::tr("î"));
        line.replace(QObject::tr("&ocirc;"), QObject::tr("ô"));
        line.replace(QObject::tr("&ucirc;"), QObject::tr("û"));
        line.replace(QObject::tr("&Acirc;"), QObject::tr("Â"));
        line.replace(QObject::tr("&Ecirc;"), QObject::tr("Ê"));
        line.replace(QObject::tr("&Icirc;"), QObject::tr("Î"));
        line.replace(QObject::tr("&Ocirc;"), QObject::tr("Ô"));
        line.replace(QObject::tr("&Ucirc;"), QObject::tr("Û"));
        text.append(line);
    }
    file.close();
    return text;
}


QString getStripHTMLTags(QString path)
{
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return "";

    QString text;

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();

        line.replace(QObject::tr("&amp;"), QObject::tr("&"));
        line.replace(QObject::tr("&lt;"), QObject::tr("<"));
        line.replace(QObject::tr("&gt;"), QObject::tr(">"));
        line.replace(QObject::tr("&iquest;"), QObject::tr("¿"));
        line.replace(QObject::tr("&iexcl;"), QObject::tr("¡"));
        line.replace(QObject::tr("&nbsp;"), QObject::tr(" "));
        line.replace(QObject::tr("&laquo;"), QObject::tr("«"));
        line.replace(QObject::tr("&raquo;"), QObject::tr("»"));
        line.replace(QObject::tr("&#039;"), QObject::tr("\''"));
        line.replace(QObject::tr("&quot;"), QObject::tr("\""));

        text.append(line);
    }
    file.close();
    QTextEdit htmlText;
    htmlText.setHtml(text);
    return htmlText.toPlainText();
    return text;
}

QString selectArea(QString text, QString tag)
{
    if(text.toStdString().length() == 0) return "";
    size_t beg = text.toStdString().find("<"+tag.toStdString()+">");
    size_t end = text.toStdString().find("</"+tag.toStdString()+">");
    if(beg == std::string::npos) return "";
    return QString::fromStdString(std::string(text.toStdString(),
                                              beg,
                                              end - beg + tag.toStdString().length()+3));
}

QString selectUndefineArea(QString text, QString tag)
{
    size_t beg = text.toStdString().find("<"+tag.toStdString());
    size_t end = text.toStdString().find("</"+tag.toStdString()+">");

    QString tmp = QString::fromStdString(std::string(text.toStdString(),
                                                     beg,
                                                     end - beg + tag.toStdString().length()-4));

    beg = tmp.toStdString().find(">");

    return QString::fromStdString(std::string(tmp.toStdString(),
                                              beg + 1,
                                              tmp.toStdString().length()));
}

QString strsTruncate(QString str, int i)
{
    if (str.length() > i)
    {
        if (str.at(i) != 32)
        {
            for (int j = i; j < str.length(); j++)
            {
                if (str.at(j) == 32)
                {
                    i = j;
                    break;
                }
            }
        }
        str.truncate(i);
        str.append(" [...]");
    }
    return str;
}

QString strHtmlEncode(QString str)
{
    str.replace(QObject::tr("<"), QObject::tr("&lt;"));
    str.replace(QObject::tr(">"), QObject::tr("&gt;"));
    str.replace(QObject::tr("¿"), QObject::tr("&iquest;"));
    str.replace(QObject::tr("¡"), QObject::tr("&iexcl;"));
    str.replace(QObject::tr("&"), QObject::tr("&amp;"));
    str.replace(QObject::tr("«"), QObject::tr("&laquo;"));
    str.replace(QObject::tr("»"), QObject::tr("&raquo;"));
    str.replace(QObject::tr("\''"), QObject::tr("&#039;"));
    str.replace(QObject::tr("\""), QObject::tr("&quot;"));

    str.replace(QObject::tr("á"), QObject::tr("&aacute;"));
    str.replace(QObject::tr("Á"), QObject::tr("&Aacute;"));
    str.replace(QObject::tr("é"), QObject::tr("&eacute;"));
    str.replace(QObject::tr("É"), QObject::tr("&Eacute;"));
    str.replace(QObject::tr("í"), QObject::tr("&iacute;"));
    str.replace(QObject::tr("Í"), QObject::tr("&Iacute;"));
    str.replace(QObject::tr("ó"), QObject::tr("&oacute;"));
    str.replace(QObject::tr("Ó"), QObject::tr("&Oacute;"));
    str.replace(QObject::tr("ú"), QObject::tr("&uacute;"));
    str.replace(QObject::tr("Ú"), QObject::tr("&Uacute;"));
    str.replace(QObject::tr("ñ"), QObject::tr("&ntilde;"));
    str.replace(QObject::tr("Ñ"), QObject::tr("&Ntilde;"));
    str.replace(QObject::tr("Ñ"), QObject::tr("&Ntilde;"));
    str.replace(QObject::tr("à"), QObject::tr("&agrave;"));
    str.replace(QObject::tr("è"), QObject::tr("&egrave;"));
    str.replace(QObject::tr("ì"), QObject::tr("&igrave;"));
    str.replace(QObject::tr("ò"), QObject::tr("&ograve;"));
    str.replace(QObject::tr("ù"), QObject::tr("&ugrave;"));
    str.replace(QObject::tr("À"), QObject::tr("&Agrave;"));
    str.replace(QObject::tr("È"), QObject::tr("&Egrave;"));
    str.replace(QObject::tr("Ì"), QObject::tr("&Igrave;"));
    str.replace(QObject::tr("Ò"), QObject::tr("&Ograve;"));
    str.replace(QObject::tr("Ù"), QObject::tr("&Ugrave;"));
    str.replace(QObject::tr("â"), QObject::tr("&acirc;"));
    str.replace(QObject::tr("ê"), QObject::tr("&ecirc;"));
    str.replace(QObject::tr("î"), QObject::tr("&icirc;"));
    str.replace(QObject::tr("ô"), QObject::tr("&ocirc;"));
    str.replace(QObject::tr("û"), QObject::tr("&ucirc;"));
    str.replace(QObject::tr("Â"), QObject::tr("&Acirc;"));
    str.replace(QObject::tr("Ê"), QObject::tr("&Ecirc;"));
    str.replace(QObject::tr("Î"), QObject::tr("&Icirc;"));
    str.replace(QObject::tr("Ô"), QObject::tr("&Ocirc;"));
    str.replace(QObject::tr("Û"), QObject::tr("&Ucirc;"));
    return str;
}

QString strHtmlDecode(QString str)
{
    str.replace(QObject::tr("&amp;"), QObject::tr("&"));
    str.replace(QObject::tr("&lt;"), QObject::tr("<"));
    str.replace(QObject::tr("&gt;"), QObject::tr(">"));
    str.replace(QObject::tr("&iquest;"), QObject::tr("¿"));
    str.replace(QObject::tr("&iexcl;"), QObject::tr("¡"));
    str.replace(QObject::tr("&nbsp;"), QObject::tr(" "));
    str.replace(QObject::tr("&laquo;"), QObject::tr("«"));
    str.replace(QObject::tr("&raquo;"), QObject::tr("»"));
    str.replace(QObject::tr("&#039;"), QObject::tr("\''"));
    str.replace(QObject::tr("&quot;"), QObject::tr("\""));

    str.replace(QObject::tr("&aacute;"), QObject::tr("á"));
    str.replace(QObject::tr("&Aacute;"), QObject::tr("Á"));
    str.replace(QObject::tr("&eacute;"), QObject::tr("é"));
    str.replace(QObject::tr("&Eacute;"), QObject::tr("É"));
    str.replace(QObject::tr("&iacute;"), QObject::tr("í"));
    str.replace(QObject::tr("&Iacute;"), QObject::tr("Í"));
    str.replace(QObject::tr("&oacute;"), QObject::tr("ó"));
    str.replace(QObject::tr("&Oacute;"), QObject::tr("Ó"));
    str.replace(QObject::tr("&uacute;"), QObject::tr("ú"));
    str.replace(QObject::tr("&Uacute;"), QObject::tr("Ú"));
    str.replace(QObject::tr("&ntilde;"), QObject::tr("ñ"));
    str.replace(QObject::tr("&Ntilde;"), QObject::tr("Ñ"));
    str.replace(QObject::tr("&Ntilde;"), QObject::tr("Ñ"));
    str.replace(QObject::tr("&agrave;"), QObject::tr("à"));
    str.replace(QObject::tr("&egrave;"), QObject::tr("è"));
    str.replace(QObject::tr("&igrave;"), QObject::tr("ì"));
    str.replace(QObject::tr("&ograve;"), QObject::tr("ò"));
    str.replace(QObject::tr("&ugrave;"), QObject::tr("ù"));
    str.replace(QObject::tr("&Agrave;"), QObject::tr("À"));
    str.replace(QObject::tr("&Egrave;"), QObject::tr("È"));
    str.replace(QObject::tr("&Igrave;"), QObject::tr("Ì"));
    str.replace(QObject::tr("&Ograve;"), QObject::tr("Ò"));
    str.replace(QObject::tr("&Ugrave;"), QObject::tr("Ù"));
    str.replace(QObject::tr("&acirc;"), QObject::tr("â"));
    str.replace(QObject::tr("&ecirc;"), QObject::tr("ê"));
    str.replace(QObject::tr("&icirc;"), QObject::tr("î"));
    str.replace(QObject::tr("&ocirc;"), QObject::tr("ô"));
    str.replace(QObject::tr("&ucirc;"), QObject::tr("û"));
    str.replace(QObject::tr("&Acirc;"), QObject::tr("Â"));
    str.replace(QObject::tr("&Ecirc;"), QObject::tr("Ê"));
    str.replace(QObject::tr("&Icirc;"), QObject::tr("Î"));
    str.replace(QObject::tr("&Ocirc;"), QObject::tr("Ô"));
    str.replace(QObject::tr("&Ucirc;"), QObject::tr("Û"));
    return str;
}


QString strToPlainText(QString str)
{
    if (str.contains("<img", Qt::CaseSensitive))
    {
        int i = 0;
        while ((i = str.indexOf("<img", i, Qt::CaseSensitive)) != -1)
        {
            int j = i;
            QString swap = "";
            while (str.at(j) != '>')
            {
                swap.append(str.at(j));
                j++;
            }
            swap.append(str.at(j));
            str.remove(swap, Qt::CaseSensitive);
            i++;
        }
    }

    QTextEdit htmlText;
    htmlText.setHtml(str);
    return htmlText.toPlainText();
}

QString strStripCurses(QString text)
{
    QStringList curses, sign;
    curses << "a" << "abajo" << "acá" << "acaso" << "además" << "adrede" << "ahí" << "ahora"
           << "al" << "algo" << "allá" << "allí" << "alrededor" << "alguna" << "algunas"
           << "alguno" << "algunos" << "anoche" << "ante" << "anteriormente" << "antes"
           << "aprisa" << "aproximadamente" << "aquí" << "arriba" << "así" << "asimismo" << "atrás"
           << "aun" << "aún" << "ayer" << "bajo" << "bastante" << "bien" << "cabe" << "casi"
           << "cerca" << "ciertamente" << "cierto" << "claro" << "como" << "cómo" << "con"
           << "concretamente" << "conque" << "consecuentemente" << "contra" << "contrariamente" << "cuándo" << "cuánto"
           << "de" << "debajo" << "del" << "delante" << "demasiado" << "deprisa" << "desde" << "despacio" << "detrás"
           << "dónde" << "donde" << "durante" << "efectivamente" << "el" << "contrario" << "ello"
           << "ellos" << "ella" << "ellas" << "en" << "encima" << "enfrente" << "enseguida" << "entre" << "es"
           << "este" << "esta" << "estas" << "están" << "estar" << "esto" << "estupendamente" << "exacto" << "excepto"
           << "fue" << "fácilmente" << "fielmente" << "ha" << "hacia" << "hasta" << "he" << "hoy" << "inclusive"
           << "incluso" << "igual" << "jamás" << "la" << "las" << "le" << "les" << "lejos" << "li" << "lo" << "los"
           << "luego" << "mal" << "mañana" << "más" << "mediante" << "mejor" << "menos" << "menos" << "mientras"
           << "mismamente" << "mucha" << "muchas" << "mucho" << "muchos" << "muy" << "mejor" << "nada"
           << "negativamente" << "ni" << "no" << "nunca" << "o" << "obvio" << "otros" << "otras" << "otro"
           << "otra" << "obstante" << "para" << "peor" << "pero" << "poco" << "por" << "posiblemente"
           << "precisamente" << "probablemente" << "prontamente" << "pronto" << "propiamente" << "próximamente"
           << "puede" << "que" << "quizá" << "quizás" << "regular" << "responsablemente" << "se" << "ser"
           << "salvo" << "según" << "seguramente" << "sí" << "siempre" << "sin embargo" << "sin" << "sino"
           << "siquiera" << "so" << "son" << "sobre" << "solamente" << "solamente" << "sólo" << "solo" << "su"
           << "tal" << "también" << "tampoco" << "tan" << "tan" << "tanto" << "tarde" << "temprano" << "todavía"
           << "toda" << "todas" << "todo" << "todos" << "tras" << "través" << "que" << "qué" << "u" << "un" << "una"
           << "uno" << "únicamente" << "vez" << "varias" << "verdaderamente" << "versus" << "vía" << "viceversa" << "y" << "ya";

    sign << "." << "," << ";" << ":" << "!" << "¡" << "¿" << "?" << "@" << "~"
         << "$" << "%" << "&" << "^" << "*" << "#" << "\\" << "|" << "/"
         << "+" << "~" << "'" << "\"";

    foreach (QString i, sign) text.remove(i, Qt::CaseSensitive);

    foreach (QString item, curses)
    {
        text.replace(" " + item + " ", " ", Qt::CaseSensitive);
    }

    return text;
}

QString selectReferenceArea(QString text, int id)
{
    QString str = QString("<reference id=\"%1\" title=\"Texto de referencia\">").arg(id);

    QString swap = "";
    if (text.contains(str, Qt::CaseSensitive))
    {
        qDebug() << "referencia :" << str << endl;
        size_t beg = text.toStdString().find(str.toStdString());
        size_t end = text.toStdString().find("</reference>");

        swap = QString::fromStdString(std::string(text.toStdString(),
                                                  beg,
                                                  end - beg + 12));

    }

    return swap;
}

QString quote(QString str)
{
    if (str.contains("'")) str.replace("'", "\''", Qt::CaseSensitive);
    str.replace(QRegExp("(<&~|~&>)"), "'");
    return str;
}

