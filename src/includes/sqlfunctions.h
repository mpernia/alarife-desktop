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

#ifndef SQLFUNCTIONS_H
#define SQLFUNCTIONS_H

#include <QString>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>


const QString SQL_ListMedia = "SELECT DISTINCT adjuntos.id_adjunto || '.' || adjuntos.extension AS image FROM adjuntos WHERE %1";

const QString SQL_Listers = "SELECT DISTINCT adjuntos.descripcion, adjuntos.id_adjunto FROM adjuntos "
                            "WHERE adjuntos.id_adjunto NOT IN "
                            "(SELECT DISTINCT adjuntos.id_adjunto FROM adjuntos INNER JOIN adjuntos_slide "
                            "ON adjuntos_slide.id_adjunto = adjuntos.id_adjunto WHERE adjuntos_slide.id_slide = %1) "
                            "AND (adjuntos.id_adjunto <> 0) AND (adjuntos.tipo = 'I') %2 ORDER BY adjuntos.descripcion";

const QString SQL_Candidates = "SELECT DISTINCT adjuntos.descripcion, adjuntos.id_adjunto, adjuntos.id_adjunto || '.' "
                               "|| adjuntos.extension AS image FROM adjuntos INNER JOIN adjuntos_slide ON "
                               "adjuntos_slide.id_adjunto = adjuntos.id_adjunto WHERE adjuntos_slide.id_slide = %1 "
                               "ORDER BY adjuntos_slide.orden";

const QString SQL_TitlesList = "SELECT DISTINCT articulo.nombre FROM articulo "
                               "WHERE articulo.id_articulo NOT IN "
                               "(SELECT DISTINCT articulo.id_articulo FROM articulo INNER JOIN articuloarticulos "
                               "ON articulo.id_articulo == articuloarticulos.IdArticuloLink WHERE articuloarticulos.articulo == %1) "
                               "AND articulo.id_articulo != %2";

const QString SQL_AuthorsList = "SELECT DISTINCT autor.nombre FROM autor "
                                "WHERE autor.id_autor NOT IN "
                                "(SELECT DISTINCT autor.id_autor FROM autor INNER JOIN articuloautores "
                                "ON autor.id_autor == articuloautores.autor WHERE articuloautores.articulo == %1) ";


 const QString SQL_LevelClean = "SELECT id_nivel FROM nivel WHERE padre NOT IN "
                                "(SELECT id_nivel FROM nivel) AND padre != 0";

 const QString SQL_ArticleClean = "SELECT articulo.id_articulo FROM articulo "
                                  "LEFT OUTER JOIN nivel ON articulo.nivel = nivel.id_nivel "
                                  "WHERE articulo.nivel NOT IN (SELECT id_nivel FROM nivel)";

 const QString SQL_ArticleAuthorClean = "SELECT articuloautores.autor FROM articuloautores "
                                        "WHERE articuloautores.articulo NOT IN "
                                        "(SELECT id_articulo FROM) articulo";

const QString SQL_Reference = "SELECT id_referencia, enlace, nombre, resumen, texto "
                              "FROM referencias WHERE articulo = %1";

 const QString SQL_GalleryItem = "SELECT DISTINCT adjuntos.id_adjunto||'.'||adjuntos.extension AS image, "
                                 "galeria.id_galeria, adjuntos.descripcion, adjuntos.id_adjunto "
                                 "FROM adjuntos LEFT OUTER JOIN galeria ON adjuntos.id_adjunto = "
                                 "galeria.id_adjunto WHERE (adjuntos.tipo= 'I') AND "
                                 "(adjuntos.id_adjunto > 0) %1 ORDER BY adjuntos.descripcion;";

 const QString SQL_GalleryAdd = "SELECT DISTINCT adjuntos.id_adjunto  FROM adjuntos "
                                "LEFT OUTER JOIN galeria ON galeria.id_adjunto = "
                                "adjuntos.id_adjunto WHERE (adjuntos.tipo= 'I') AND "
                                "(adjuntos.id_adjunto > 0) AND (galeria.id_adjunto "
                                "IS NULL)%1 ORDER BY adjuntos.id_adjunto;";

 const QString SQL_GalleryDel = "DELETE FROM galeria WHERE id_galeria IN (SELECT "
                                "galeria.id_galeria FROM galeria INNER JOIN adjuntos ON "
                                "galeria.id_adjunto = adjuntos.id_adjunto WHERE "
                                "(adjuntos.tipo= 'I') AND (adjuntos.id_adjunto > 0) %1)";

const QString SQL_SlideShow = "SELECT slide.id_pagina, slide.tipo, adjuntos.id_adjunto, "
                              "adjuntos_slide.orden FROM slide INNER JOIN adjuntos_slide "
                              "ON slide.id_slide = adjuntos_slide.id_slide INNER JOIN "
                              "adjuntos ON adjuntos.id_adjunto = adjuntos_slide.id_adjunto "
                              "WHERE slide.id_slide = %1 ORDER BY slide.id_slide";

const QString SQL_Insert = "INSERT INTO %1 (%2) VALUES (%3)";
const QString SQL_Update = "UPDATE %1 SET %2  %3";
const QString SQL_Delete = "DELETE FROM %1 %2";
//----------------------------------------------------------------------------------------------
QString sqlGetImageName(int);
QString sqlGetTextValue(QString table = "", QString field = "", QString conditions = "");
int sqlAutoIncremental(QString field = "", QString table = "", QString conditions = "");
int sqlAutoOrder(int father, int book);
int sqlAutoCountRow(QString table = "", QString conditions = "");
int sqlGetIdValue(QString table = "", QString field = "", QString conditions = "");
bool sqlExistText(QString text = "", QString field = "", QString table = "");
bool sqlInsertRecord_N(QString table = "", QString fields = "", QString values = "");
bool sqlDeleteRecord_N(QString table = "", QString values = "");
bool sqlUpdateRecord_N(QString table = "", QString fields = "", QString values = "");
void sqlCustomerQuery(QString sql = "");
void sqlBuildLevelQuery(QString, QString &);
void sqlBuildArticleQuery(QString, QString &);
void sqlClearOrphanLevels();
void sqlClearOrphanArticles();
void sqlClearArticlesAuthorRelations();
void sqlValuesAsList(QString sql, QStringList *list);



#endif // SQLFUNCTIONS_H
