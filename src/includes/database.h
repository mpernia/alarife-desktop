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

#ifndef DATABASE_H 
#define DATABASE_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>
#include <QTextEdit>

#include "alarife.h"
//----------------------------------------------------------------------------------------------
class DbConnect{
public:
    DbConnect();
    ~DbConnect();
    void ConnectionString(QString);
    void Active(bool);
    void CreateDb();
    bool Connected();
    bool ChechkCompatibility();
    QString getPathFile();
    QString getFileName();
    QString getDirPath();
    int getUserNum();
    QStringList getTablesName();
private:
     QSqlDatabase dbName;
     bool connected;
     QString DbPathFile;
     int cCount;
};
//----------------------------------------------------------------------------------------------


#endif

