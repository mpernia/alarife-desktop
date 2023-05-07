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

#ifndef STRFUNCTION_H
#define STRFUNCTION_H

#include <QString>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>

QString getActualDataTime();
QString getPrintDateTime(QString, QString);
QString getStripFileName(const QString &fullFileName);
QString getSubString(QString, QString, QString, int);
QString getFileSize(quint64);
QString getCodeToHtml(QString path = "");
QString getStripHTMLTags(QString path = "");
QString selectArea(QString text, QString tag);
QString selectUndefineArea(QString text, QString tag);
QString strsTruncate(QString, int);
QString strHtmlEncode(QString);
QString strHtmlDecode(QString str);
QString strToPlainText(QString);
QString strStripCurses(QString);
QString selectReferenceArea(QString, int);
QString quote(QString);

#endif // STRFUNCTION_H
