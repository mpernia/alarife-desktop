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

#include <QDebug>
#include <QMessageBox>

#include "../includes/sqlfunctions.h"
#include "../includes/strfunction.h"


QString sqlGetImageName(int id)
{
    QString tmp;
    QSqlQuery *qryExec = new QSqlQuery;
    qryExec->exec(QString("SELECT adjuntos.id_adjunto||'.'||adjuntos.extension AS mediaFile "
                          "FROM adjuntos WHERE adjuntos.id_adjunto = %1").arg(id));
    qryExec->first();
    tmp = qryExec->value(0).toString();
    delete qryExec;
    return tmp;
}

QString sqlGetTextValue(QString table, QString field, QString conditions)
{
    QSqlQuery *qry = new QSqlQuery;
    if(!conditions.isEmpty()) conditions.prepend("WHERE ");
    qry->exec(QString("SELECT %1 FROM %2 %3").arg(field).arg(table).arg(conditions));
    qry->first();
    QString aux = qry->value(0).toString();
    delete qry;
    return aux;
}

int sqlAutoIncremental(QString field, QString table, QString conditions)
{
    QSqlQuery *qryExec = new QSqlQuery;
    if(!conditions.isEmpty()) conditions.prepend("WHERE ");
    qryExec->exec(QString("SELECT MAX(%1) FROM %2 %3").arg(field).arg(table).arg(conditions));
    qryExec->first();
    int swap = qryExec->value(0).toInt()+1;
    delete qryExec;
    return swap;
}

int sqlAutoOrder(int father, int book)
{
    QSqlQuery *qry = new QSqlQuery;
    qry->exec(QString("SELECT MAX(orden) FROM nivel WHERE "
                      "padre = %1 AND enciclopedia = %2").
              arg(father).arg(book));
    qry->first();
    int swap = qry->value(0).toInt()+1;
    delete qry;
    return swap;
}

int sqlAutoCountRow(QString table, QString conditions)
{
    QSqlQuery *qry = new QSqlQuery;
    if(!conditions.isEmpty()) conditions.prepend("WHERE ");
    qry->exec(QString("SELECT COUNT(*) FROM %1 %2").arg(table).arg(conditions));
    qry->first(); qDebug() << "SELECT COUNT: " << qry->lastQuery() << endl;
    int aux = qry->value(0).toInt();
    delete qry;
    return aux;
}

int sqlGetIdValue(QString table, QString field, QString conditions)
{
    QSqlQuery *qry = new QSqlQuery;
    int aux = 0;
    if(!conditions.isEmpty()) conditions.prepend("WHERE ");
    qry->exec(QString("SELECT %1 FROM %2 %3").arg(field).arg(table).arg(conditions));
    if (qry->first()) aux = qry->value(0).toInt();
    delete qry;
    return aux;
}

bool sqlExistText(QString text, QString field, QString table)
{
    QSqlQuery *qry = new QSqlQuery;
    qry->exec(QString("SELECT %1 FROM %2 WHERE %3 = '%4'").arg(field).
              arg(table).arg(field).arg(text));
    bool state = qry->isValid();
    delete qry;
    return state;
}

bool sqlInsertRecord_N(QString table, QString fields, QString values)
{
    bool state;
    QSqlQuery *qry = new QSqlQuery;
    state = qry->exec(SQL_Insert.arg(table).arg(fields).arg(quote(values)));
    qDebug() << "INSERT: " << qry->lastQuery();
    delete qry;
    return state;
}

bool sqlDeleteRecord_N(QString table, QString conditions)
{
    bool state;
    QSqlQuery *qry = new QSqlQuery;
    if(!conditions.isEmpty()) conditions.prepend("WHERE ");
    state = qry->exec(SQL_Delete.arg(table).arg(quote(conditions)));
    qDebug() << "DELETE: " << qry->lastQuery() << endl;
    delete qry;
    return state;
}

bool sqlUpdateRecord_N(QString table, QString fields, QString values)
{
    bool state;
    QSqlQuery *qry = new QSqlQuery;
    state = qry->exec(SQL_Update.arg(table).arg(quote(fields)).arg(quote(values)));
    qDebug() << "UPDATE: " << qry->lastQuery() << endl;
    delete qry;
    return state;
}

void sqlCustomerQuery(QString sql)
{
    QSqlQuery *qry = new QSqlQuery;
    qry->exec(sql);qDebug() << "CUSTOMER: " << qry->lastQuery() << endl;
    delete qry;
}

void sqlBuildLevelQuery(QString str0, QString &strSql)
{
    QString str,
            strTmp = " nombre LIKE '%%1%' ";

    if (!str0.contains(" && ", Qt::CaseSensitive) && !str0.contains(" || ", Qt::CaseSensitive)){
        strSql.append(strTmp.arg(str0).arg(str0));
    }else{
        size_t tokenAnd = str0.toStdString().find(" && ");
        size_t tokenOr = str0.toStdString().find(" || ");
        if (tokenAnd < tokenOr){
            str = QString::fromStdString(std::string(str0.toStdString(), 0, tokenAnd));
            strSql.append(strTmp.arg(str).arg(str) + " AND ");
            sqlBuildLevelQuery(QString::fromStdString(std::string(str0.toStdString(), tokenAnd + 3,
                                                            str0.toStdString().length())), strSql);
        }else{
            str = QString::fromStdString(std::string(str0.toStdString(), 0, tokenOr));
            strSql.append(strTmp.arg(str).arg(str) + " OR ");
            sqlBuildLevelQuery(QString::fromStdString(std::string(str0.toStdString(), tokenOr + 3,
                                                            str0.toStdString().length())), strSql);
        }
    }
}

void sqlBuildArticleQuery(QString str0, QString &strSql)
{
    QString str,
            strTmp = "(articulo.nombre LIKE '%%1%' OR articulo.resumen "
                     "LIKE '%%2%' OR articulo.claves LIKE '%%3%')";

    if (!str0.contains(" && ", Qt::CaseSensitive) && !str0.contains(" || ", Qt::CaseSensitive)){
        strSql.append(strTmp.arg(str0).arg(str0).arg(str0));
    }else{
        size_t tokenAnd = str0.toStdString().find(" && ");
        size_t tokenOr = str0.toStdString().find(" || ");
        if (tokenAnd < tokenOr){
            str = QString::fromStdString(std::string(str0.toStdString(), 0, tokenAnd));
            strSql.append(strTmp.arg(str).arg(str).arg(str) + " AND ");
            sqlBuildArticleQuery(QString::fromStdString(std::string(str0.toStdString(), tokenAnd + 3,
                                                            str0.toStdString().length())), strSql);
        }else{
            str = QString::fromStdString(std::string(str0.toStdString(), 0, tokenOr));
            strSql.append(strTmp.arg(str).arg(str).arg(str) + " OR ");
            sqlBuildArticleQuery(QString::fromStdString(std::string(str0.toStdString(), tokenOr + 3,
                                                            str0.toStdString().length())), strSql);
        }
    }
}


void sqlClearOrphanLevels()
{
    qDebug() << "Eliminando niveles huerfanos" ;
    QSqlQuery *qryLevels = new QSqlQuery;
    qryLevels->exec(SQL_LevelClean);

    while (qryLevels->next())
    {
        int idlevel = qryLevels->value(0).toInt();
        sqlDeleteRecord_N("articulo", QString("nivel = %1").arg(idlevel));
        sqlDeleteRecord_N("nivel", QString("id_nivel = %1").arg(idlevel));
    }
    delete qryLevels;
}

void sqlClearOrphanArticles(){
    qDebug() << "Eliminando articulos huerfanos" ;
    QSqlQuery *qryLevels = new QSqlQuery;
    qryLevels->exec(SQL_ArticleClean);

    while (qryLevels->next())
    {
        int idarticle = qryLevels->value(0).toInt();
        sqlDeleteRecord_N("articulo", QString("nivel = %1").arg(idarticle));
    }
    delete qryLevels;
}

void sqlClearArticlesAuthorRelations(){
    qDebug() << "Eliminando relaccion articulos/autor huerfanos" ;
    QSqlQuery *qryRelation = new QSqlQuery;
    qryRelation->exec(SQL_ArticleAuthorClean);

    while (qryRelation->next())
    {
        int idarticle = qryRelation->value(0).toInt();
        sqlDeleteRecord_N("articuloautores", QString("articulo = %1").arg(idarticle));
    }
    delete qryRelation;
}

void sqlChangeHtmlCodeToUtf(QString table, QString field)
{
    QSqlQuery *qry = new QSqlQuery;
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&aacute;', 'á');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&Aacute;', 'Á');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&eacute;', 'é');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&Eacute;', 'É');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&iacute;', 'í');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&Iacute;', 'Í');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&oacute;', 'ó');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&Oacute;', 'Ó');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&uacute;', 'ú');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&Uacute;', 'Ú');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&ntilde;', 'ñ');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&Ntilde;', 'Ñ');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&agrave;', 'à');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&egrave;', 'è');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&igrave;', 'ì');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&ograve;', 'ò');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&ugrave;', 'ù');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&Agrave;', 'À');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&Egrave;', 'È');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&Igrave;', 'Ì');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&Ograve;', 'Ò');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&Ugrave;', 'Ù');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&acirc;', 'â');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&ecirc;', 'ê');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&icirc;', 'î');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&ocirc;', 'ô');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&ucirc;', 'û');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&Acirc;', 'Â');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&Ecirc;', 'Ê');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&Icirc;', 'Î');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&Ocirc;', 'Ô');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&Ucirc;', 'Û');").arg(table).arg(field));
    delete qry;
}

void sqlStripHTMLTags(QString table, QString field)
{
    QSqlQuery *qry = new QSqlQuery;
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<html>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '</html>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<head>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '</head>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<title>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '</title>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<body>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '</body>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '</span>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<b>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '</b>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<i>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '</i>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<u>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '</u>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<s>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '</s>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<strong>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '</strong>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<p>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '</p>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<br>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '</ br>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<span style=\"font-style: normal;\">', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<p style=\"text-align: justify;\">', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, 'style=\"margin-top: 3px; margin-bottom: 3px; margin-left: 5px; margin-right: 5px; float: left;\"', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<p style=\"margin-bottom:0cm;text-align:justify\">', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<p style=\"margin-bottom: 0cm; text-align: justify;\">', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<em>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '</em>', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<span style=\"font-size: x-small;\">', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<span style=\"font-family: verdana, geneva;\">', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<span style=\"font-family: verdana,geneva;\">', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<span style=\"font-size: xx-small;\">', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<span style=\"text-align: justify;\">', '');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '<span>', '');").arg(table).arg(field));
    delete qry;
}

void sqlCodeToHtml(QString table, QString field)
{
    QSqlQuery *qry = new QSqlQuery;
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&lt;', '<');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&gt;', '>');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&iquest;', '¿');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&iexcl;', '¡');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&amp;', '&');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&nbsp;', ' ');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&laquo;', '«');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&raquo;', '»');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&#039;', '\''');").arg(table).arg(field));
    qry->exec(QString("UPDATE %1 SET %2 = REPLACE(%2, '&quot;', '\"');").arg(table).arg(field));
    delete qry;
}

void sqlValuesAsList(QString sql, QStringList *list)
{
    list->clear();

    QSqlQuery *qryList = new QSqlQuery;
    qryList->exec(sql);
    if (qryList->first())
        list->append(qryList->value(0).toString());

    while (qryList->next())
        list->append(qryList->value(0).toString());

    delete qryList;
}


