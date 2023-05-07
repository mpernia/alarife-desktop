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

#ifndef IMPORT_H
#define IMPORT_H

#include <QString>
#include <QSqlQuery>
#include <QWebFrame>
#include <QWebPage>
#include <QWebElementCollection>

#include <QMap>
#include <QMapIterator>

#include "database.h"
#include "dinarray.h"
#include "alarife.h"

extern DbConnect AlarifeDb;
extern QSqlQuery qryImport;
extern Book encyclopedia;

class dbImport : public QWidget
{
Q_OBJECT
public:
    explicit dbImport(QWidget *parent = 0, QString pathBackup = "");
signals:

public slots:        
    void setMediaDir(QString);
    void createFileSystem();
    void newEncyclopedia();
    void addEncyclopedia(int father = 0);
    void mergeLevels(int father = 0);
    void importMedias();
    void importLevels();
    void importArticles();
    void importRelationArticles();
    void importAuthor();
    void importTeamColaborator();
    void importRelationAuthors();
    void uncompress();
    void cleanTemporalFiles();
    void PruebaGuardarMedia();
private:
    void readFatherList();
    void scanDir(QString dirpath = "", QString ext = "", DinArray **a=0);
    void PruebascanDir(QString dirpath = "", QString ext = "", QMap<int, int> *map=0);
    int readFileToInt(QString name = "");
    QString readMediaDescription(QString);
    QString readFileToString(QString name = "");

    void insertExtensions();
    void insertMedias(int old = 0);
    void insertLevels(int old = 0);
    void insertArticles(int old = 0);
    void insertAutors(QString path = "", int old = 0);
    void insertTeamColaborator();
    void summaryReplaceMedias(QString&, int *);
    void contentReplaceMedias(QString&);
    int findAuthor(QString);
    void createUser();
    void cleanCss();

    void Prueba(QString, QMapIterator<int, int>*);

    QString alfeFile,
            mediadir,
            pathImp,
            nombre,
            contenido,
            resumen,
            claves;

    int id_enciclopedia,
        id_usuario,
        id_nivel,
        id_key;

    QMap<int, int>mapLevels,
                  mapArticles,
                  mapAutors,
                  mapMedias;

    QWebPage page;
};

#endif // IMPORT_H
