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

#ifndef EXPORT_H
#define EXPORT_H

#include <QList>

#include "alarife.h"
#include "database.h"

class dbExport: public QWidget
{
Q_OBJECT
public:
    explicit dbExport(QWidget *parent = 0, Book *encyc = 0);
    int *position;
    QString *message;
signals:

public slots:
    void exportEncyclopedia();
    void exportLevels();
    void exportArticles();
    void exportAuthor();
    void exportRelationArticles();
    void exportTeamColaborator();
    void exportMedias();
    void exportSlideShow();
    void exportGallery();
    void exportReference();
    void createDir(QString pathDir = "");
    void compressEncyclopedia();
    void cleanTemporalFiles();

private:
    void WriteFile(QString name = "", QString text = "");
    void UpdateStatus(int x = 0, QString msg = "");

    DbConnect *conn;
    QList<QString> lstAuthors,
                   lstMedia;
    QString path,
            path2,
            name;
    int id_enciclopedia;
    Book *encyclopedia;

};

#endif // EXPORT_H
