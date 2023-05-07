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

#include "database.h"
#include "dinarray.h"
#include "alarife.h"

class dbImport : public QWidget
{
Q_OBJECT
public:
    explicit dbImport(QWidget *parent = 0, QString pathBackup = "", int user = -1, int id = 1);
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
    void importSlideShow();
    void importGallery();
    void importReference();
    void uncompress();
    void cleanTemporalFiles();
    QString getName() { return enombre; }
private:
    void readFatherList(QString name = "");
    void scanDir(QString dirpath = "", QString ext = "", DinArray **a=0);
    int readFileToInt(QString name = "");//int readFatherValue(QString name = "");
    QString readMediaDescription(QString);
    QString readFileToString(QString name = "");

    void insertExtensions();
    void insertMedias(int old = 0);
    void insertLevels(int old = 0);
    void insertArticles(int old = 0);
    void insertAutors(QString path = "", int old = 0);
    void insertSlides(int slide = 0);
    void contentReplaceMedias(QString&);
    int findAuthor(QString);
    void createUser();
    void cleanCss();

    QString alfeFile,
            mediadir,
            pathImp,
            enombre,
            contenido,
            resumen,
            claves,
            program;

    int id_enciclopedia,
        id_usuario,
        id_nivel,
        id_key;

    DinArray *linkLevels,
             *linkArticles,
             *linkAutors,
             *linkMedias,
             *linkSlide,
             *linkGallery,
             *linkReference;

    QWebPage page;

    bool merge;
};

#endif // IMPORT_H
