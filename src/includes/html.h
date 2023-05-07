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

#ifndef HTML_H
#define HTML_H

#include <QDebug>
#include <QtSql>
#include <QDir>

QT_BEGIN_NAMESPACE
class QString;
class QStringList;
class QFile;
class QTextStream;
class QSqlQuery;
QT_END_NAMESPACE


const QString TAG_VIDEO = "<video src=\"%1\" controls preload=\"none\" "
                          "align=\"left\" height=\"200\" width=\"300\" "
                          "style=\"margin-right: 10px;\" alt=\"%2\" "
                          "title=\"%3\">&nbsp;</video>";

const QString TAG_AUDIO = "<audio src=\"%1\" controls preload=\"none\" "
                          "alt=\"%2\" title=\"%3\">&nbsp;</audio>";
class html
{
public:
    html();
    ~html();
    void openHtml();
    void saveHtml();
    void closeHtml();
    void setOpenPath(QString openPath);
    void setSavePath(QString savePath);
    void setTitleWindows(QString title);
    QString getHtmlText();
    virtual void clear() = 0;
    virtual void changeValueElements() = 0;
protected:
    void clearHtml();
    void stripTag(QString &, QString);


    QString selectArea(QString);
    QString htmlText,
            winTitle;

private:
    QString pathOpen,
            pathSave;
};

#endif // HTML_H
