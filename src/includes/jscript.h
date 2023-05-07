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

#ifndef JSCRIPT_H
#define JSCRIPT_H

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

class jScript
{
public:
    jScript(){}
    void setOpenPath(QString openPath);
    void setSavePath(QString savePath);
    void openJScript();
    void saveJScript();
    void changeValueElements();
    void changeValueGallery();
    void setIndex(int i) { index = i; }
private:
    QString selectArea(QString);
    QString setUtfToHtml(QString);
    QString setQuote(QString);
    QString setAuthorList(int);

    QString jscriptText,
            pathOpen,
            pathSave;
    int index;
};

#endif // JSCRIPT_H
