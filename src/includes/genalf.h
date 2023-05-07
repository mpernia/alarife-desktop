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

#ifndef GENALF_H
#define GENALF_H

#include <QWidget>
#include <QString>
#include <QSqlQuery>
#include <QWebFrame>
#include <QWebPage>
#include <QWebElementCollection>

#include "database.h"

class GenAlf : public QWidget
{
Q_OBJECT
public:
    explicit GenAlf(QWidget *parent = 0, Book *encyc = 0);

signals:

public slots:
    void setTargetPath(QString);

    void setIndex(int index);
    void createFileSystem();
    void generateIndex();
    void generateAuthor();
    void generateCredits();
    void generateHelp();
    void generateTeam();
    void generateGallery();
    void generateLevels();
    void generateArticles();
    void generateSearch();
    void generateMedia();
    void generateStadistic();
private:
    void ClearTag(QString, QString&);
    void GetTemplateType(QString);
    void PrepareExtensionFiles();
    void SelectMediaInText(QString);
    void PrepareInsertMedias();
    void InsertMedias(QString);

    void setTemplatePath(QString);

    QString generateAreaVMenu(QString text, int padre);
    QString generateAreaHMenu(QString text, int padre);
    QString generateAreaLevels(QString text, int padre);
    QString generateAreaArticles(QString text, int padre);

    QString generateAreaRelationsAuthors(int id_articulo);
    QString generateAreaRelationsArticles(QString text, int id_articulo);
    QString generateAreaSource(QString);
    QString generateAreaImage(int);
    QString generateSlideShow(int id_pagina, char tipo);


    QString templates,
            target,
            dbase,
            mainframe,
            swap,
            vmenu,
            hmenu,
            relation,
            name_enc;

    QString extImagen, extSonido, extVideo, extXMedia;

    Book *encyclopedia;

    int id_enciclopedia,
        typeT,
        avMode;

    bool gallery;

    QSqlQuery qryHtml;
    QSqlQuery qryArea;
};

#endif // GENALF_H
