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

#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QProcess>

#include "../includes/export.h"
#include "../includes/folder.h"
#include "../includes/sqlfunctions.h"
#include "../includes/strfunction.h"

#include <iostream>

using namespace std;

//----------------------------------------------------------------------------------------------
dbExport::dbExport(QWidget *parent, Book *encyc) : QWidget(parent)
{
    encyclopedia = encyc;
    id_enciclopedia = encyclopedia->getIndex();
}
//----------------------------------------------------------------------------------------------
void dbExport::createDir(QString pathDir)
{
    path = __DIR_CACHE__;
    path2 = pathDir;

    QDir *dir = new QDir;
    if (dir->exists(path))
    {
        Folder *folder = new Folder;
        folder->remFiles(path);
        delete folder;
    }
    else
        dir->mkdir(path);
    delete dir;

    QDir dirExport;
    dirExport.setPath(path);

    name = encyclopedia->getTitle();
    name.replace(" ","_");
    name.replace("(","_");
    name.replace("[","_");
    name.replace(")","_");
    name.replace("]","_");
}
//----------------------------------------------------------------------------------------------
void dbExport::exportEncyclopedia(){
    QSqlQuery *qryEncyc = new QSqlQuery;
    qryEncyc->exec(QString("SELECT nombre, resumen FROM enciclopedia "
                "WHERE id_enciclopedia = %1").arg(id_enciclopedia));
    qryEncyc->first();
    if (qryEncyc->isValid())
    {
        WriteFile("den.alfe", qryEncyc->value(0).toString());

        QString content = "";

        if(qryEncyc->value(1).toString().contains("<body"))
            content = selectUndefineArea(qryEncyc->value(1).toString(),"body");
        else
            content = qryEncyc->value(1).toString();

        WriteFile("der.alfe", content);
    }
    delete qryEncyc;
    qDebug()<< "compress exportEncyclopedia();";
}
//----------------------------------------------------------------------------------------------
void dbExport::exportLevels()
{
    QSqlQuery *qryLevels = new QSqlQuery;
    qryLevels->exec(QString("SELECT id_nivel, nombre, resumen, padre, orden FROM nivel "
                        "WHERE enciclopedia = %1 ORDER BY padre ASC").arg(id_enciclopedia));
    QString str_0;

    QString content = "";

    while (qryLevels->next())
    {
        WriteFile(qryLevels->value(0).toString() + ".nn.alfe", qryLevels->value(1).toString());

        if(qryLevels->value(2).toString().contains("<body"))
            content = selectUndefineArea(qryLevels->value(2).toString(),"body");
        else
            content = qryLevels->value(2).toString();

        WriteFile(qryLevels->value(0).toString() + ".nr.alfe", content);

        WriteFile(qryLevels->value(0).toString() + ".np.alfe", qryLevels->value(3).toString());
        WriteFile(qryLevels->value(0).toString() + ".no.alfe", qryLevels->value(4).toString());
        str_0.append(qryLevels->value(0).toString() + "\n");
    }
    WriteFile("orden.nl.alfe",str_0);
    delete qryLevels;
    qDebug()<< "compress exportLevels();";
}
//----------------------------------------------------------------------------------------------
void dbExport::exportArticles()
{
    QSqlQuery *qryArticles = new QSqlQuery;
    qryArticles->exec(QString("SELECT id_articulo, nombre, resumen, contenido, claves, nivel, "
                              "fuente, fecha1, fecha2, orden FROM articulo WHERE enciclopedia = %1").arg(id_enciclopedia));
    int i = 0;

    QString content = "";

    while (qryArticles->next())
    {
        WriteFile(qryArticles->value(0).toString() + ".an.alfe", qryArticles->value(1).toString());
        WriteFile(qryArticles->value(0).toString() + ".ar.alfe", qryArticles->value(2).toString());

        if(qryArticles->value(3).toString().contains("<body"))
            content = selectUndefineArea(qryArticles->value(3).toString(),"body");
        else
            content = qryArticles->value(3).toString();

        WriteFile(qryArticles->value(0).toString() + ".ac.alfe", content);

        WriteFile(qryArticles->value(0).toString() + ".acl.alfe", qryArticles->value(4).toString());
        WriteFile(qryArticles->value(0).toString() + ".ani.alfe", qryArticles->value(5).toString());
        WriteFile(qryArticles->value(0).toString() + ".src.alfe", qryArticles->value(6).toString());
        WriteFile(qryArticles->value(0).toString() + ".dt1.alfe", qryArticles->value(7).toString());
        WriteFile(qryArticles->value(0).toString() + ".dt2.alfe", qryArticles->value(8).toString());
        WriteFile(qryArticles->value(0).toString() + ".ao.alfe", qryArticles->value(9).toString());

        if (++i == 5000)
        {
            compressEncyclopedia();
            cleanTemporalFiles();
            i = 0;
        }
    }
    delete qryArticles;
}
//----------------------------------------------------------------------------------------------
void dbExport::exportAuthor()
{
    QSqlQuery *qryAuthor = new QSqlQuery;
    qryAuthor->exec(QString("SELECT articulo.id_articulo, articulo.enciclopedia, "
                        "autor.id_autor, autor.nombre, autor.titulo, autor.cargo, "
                        "autor.correo, autor.foto FROM articuloautores INNER JOIN "
                        "articulo ON (articuloautores.articulo=articulo.id_articulo) "
                        "INNER JOIN autor ON (articuloautores.autor=autor.id_autor) "
                        "WHERE enciclopedia = %1").arg(id_enciclopedia));
    QString str_1;
    while (qryAuthor->next())
    {
        QString str_2(qryAuthor->value(2).toString());
        if (!lstAuthors.contains(str_2))
        {
            lstAuthors.append(str_2);
            QString str_3(qryAuthor->value(3).toString() + "\n" + qryAuthor->value(4).toString() + "\n" +
                          qryAuthor->value(5).toString() + "\n" + qryAuthor->value(6).toString() + "\n" +
                          qryAuthor->value(7).toString());
            WriteFile(qryAuthor->value(2).toString() + ".da.alfe", str_3);
        }
        str_1.append(qryAuthor->value(0).toString() + " " + qryAuthor->value(2).toString() + "\n");
    }
    WriteFile("articuloautores.alfe",str_1);
    delete qryAuthor;
}
//----------------------------------------------------------------------------------------------
void dbExport::exportRelationArticles()
{
    QSqlQuery *qryRelation = new QSqlQuery;
    qryRelation->exec(QString("SELECT articuloarticulos.articulo, articulo.enciclopedia, "
                              "articuloarticulos.idarticulolink FROM articuloarticulos INNER "
                              "JOIN articulo on (articuloarticulos.articulo = articulo.id_articulo) "
                              "WHERE enciclopedia = %1").arg(id_enciclopedia));
    QString str_5;
    while (qryRelation->next())
    {
        str_5.append(qryRelation->value(0).toString() + " "+ qryRelation->value(2).toString() + "\n");
    }
    WriteFile("articuloarticulos.alfe",str_5);
    delete qryRelation;
}
//----------------------------------------------------------------------------------------------
void dbExport::exportTeamColaborator()
{
    QSqlQuery *qryTeam = new QSqlQuery;
    qryTeam->exec(QString("SELECT autor.id_autor, autor.nombre, autor.titulo, autor.cargo, "
                          "autor.correo, autor.foto, enc_autor.colaborador, enc_autor.equipo "
                          "FROM enc_autor INNER JOIN autor ON (enc_autor.autor = autor.id_autor) "
                          "WHERE (colaborador or equipo) and enciclopedia = %1").arg(id_enciclopedia));
    QString str_7;
    while (qryTeam->next())
    {
        QString str_8(qryTeam->value(0).toString());
        if (!lstAuthors.contains(str_8))
        {
            lstAuthors.append(str_8);
            QString str_6(qryTeam->value(1).toString() + "\n" + qryTeam->value(2).toString() + "\n" +
                          qryTeam->value(3).toString() + "\n" + qryTeam->value(4).toString() + "\n" +
                          qryTeam->value(5).toString());
            WriteFile(qryTeam->value(0).toString() + ".da.alfe", str_6);
        }
        str_7.append(qryTeam->value(0).toString() + " " +
                     qryTeam->value(6).toString() + " " +
                     qryTeam->value(7).toString() + "\n");
    }
    WriteFile("enc_autor.alfe", str_7);
}
//----------------------------------------------------------------------------------------------
void dbExport::exportMedias()
{
    QSqlQuery *qryMedias = new QSqlQuery;
    qryMedias->exec(QString("SELECT id_adjunto, descripcion, tipo, extension "
                            "FROM adjuntos WHERE id_adjunto > 0"));

    while (qryMedias->next())
    {
        QString of = QString("%1/%2.%3").arg(encyclopedia->getMediaPath()).
                            arg(qryMedias->value(0).toInt()).
                            arg(qryMedias->value(3).toString());

        QString nf = QString("%1/%2.%3").arg(path).
                            arg(qryMedias->value(0).toInt()).
                            arg(qryMedias->value(3).toString());

        QString df = QString("%1.md.alfe").arg(qryMedias->value(0).toInt());
        QString tf = QString("%1.mt.alfe").arg(qryMedias->value(0).toInt());
        QString ef = QString("%1.me.alfe").arg(qryMedias->value(0).toInt());

        QFile::copy(of, nf );
        WriteFile(df, qryMedias->value(1).toString());
        WriteFile(tf, qryMedias->value(2).toString());
        WriteFile(ef, qryMedias->value(3).toString());
    }
    delete qryMedias;
}
//----------------------------------------------------------------------------------------------
void dbExport::exportSlideShow()
{
    QStringList list;
    sqlValuesAsList("SELECT DISTINCT id_slide FROM slide ORDER BY id_slide ASC", &list);

    QSqlQuery *qrySlide = new QSqlQuery;
    for (int i = 0; i < list.count(); i++)
    {
        qrySlide->exec(SQL_SlideShow.arg(list.at(i)));
        QString attachment = "";
        if (qrySlide->first())
        {
            WriteFile(QString("%1.sp.alfe").arg(list.at(i)), qrySlide->value(0).toString());
            WriteFile(QString("%1.st.alfe").arg(list.at(i)), qrySlide->value(1).toString());
            attachment = QString("%1:%2").
                                 arg(qrySlide->value(2).toString()).
                                 arg(qrySlide->value(3).toString()) ;
        }

        while (qrySlide->next())
            attachment.append(QString("\n%1:%2").
                              arg(qrySlide->value(2).toString()).
                              arg(qrySlide->value(3).toString()));

        WriteFile(QString("%1.sa.alfe").arg(list.at(i)), attachment);
    }
    delete qrySlide;
}
//----------------------------------------------------------------------------------------------
void dbExport::exportGallery()
{
    QString gallery;
    QSqlQuery *qryGallery = new QSqlQuery;

    qryGallery->exec(QString("SELECT DISTINCT id_adjunto FROM galeria ORDER BY id_galeria ASC"));
    if (qryGallery->first()) gallery.append(qryGallery->value(0).toString());
    while (qryGallery->next()) gallery.append("\n" + qryGallery->value(0).toString());
    WriteFile("galeria.alfe", gallery);
    delete qryGallery;
}
//----------------------------------------------------------------------------------------------
void dbExport::exportReference()
{
    QSqlQuery *qryReference = new QSqlQuery;
    qryReference->exec(QString("SELECT id_referencia, articulo, enlace, "
                               "nombre, resumen, texto FROM referencias"));
    int row = 1;
    if (qryReference->first())
    {
        WriteFile(QString("%1.rfi.alfe").arg(row), qryReference->value(0).toString());
        WriteFile(QString("%1.rfa.alfe").arg(row), qryReference->value(1).toString());
        WriteFile(QString("%1.rfe.alfe").arg(row), qryReference->value(2).toString());
        WriteFile(QString("%1.rfn.alfe").arg(row), qryReference->value(3).toString());
        WriteFile(QString("%1.rfr.alfe").arg(row), qryReference->value(4).toString());
        WriteFile(QString("%1.rft.alfe").arg(row), qryReference->value(5).toString());
    }

    while (qryReference->next())
    {
        row++;
        WriteFile(QString("%1.rfi.alfe").arg(row),qryReference->value(0).toString());
        WriteFile(QString("%1.rfa.alfe").arg(row),qryReference->value(1).toString());
        WriteFile(QString("%1.rfe.alfe").arg(row), qryReference->value(2).toString());
        WriteFile(QString("%1.rfn.alfe").arg(row), qryReference->value(3).toString());
        WriteFile(QString("%1.rfr.alfe").arg(row), qryReference->value(4).toString());
        WriteFile(QString("%1.rft.alfe").arg(row), qryReference->value(5).toString());
    }
    delete qryReference;
}
//----------------------------------------------------------------------------------------------
void dbExport::compressEncyclopedia() /*FIXME*/
{
    QString exec = __SYS_DIR__;
    if (!exec.contains("bin/"))exec = __SYS_DIR__ + "bin/";

    QString program = exec + "7z" + __EXT__;
    QString file = QString("%1/%2.alfe").arg(path2).arg(name);

    int i = 1;
    while (QFile::exists(file))
    {
        file = QString("%1/%2(%3).alfe").arg(path2).arg(name).arg(i);
        i++;
    }

    QStringList arg;
    arg << "a" << "-tzip" << "-mx=9"
        << file
        << QString("%1/*").arg(path);

    QByteArray output;

    QProcess *process = new QProcess;
    process->setReadChannel(QProcess::StandardOutput);
    process->start(program, arg);

    while (!process->waitForFinished())
    {
        output += process->readAll();
        QString str = output.constData();
        qDebug() << str;
    }

    delete process;
}
//----------------------------------------------------------------------------------------------
void dbExport::cleanTemporalFiles(){
    Folder folder;
    folder.remFiles(path);
    folder.remDir(path);
}
//----------------------------------------------------------------------------------------------
void dbExport::UpdateStatus(int x, QString msg){
    *position = x;
    *message = msg;
}
//----------------------------------------------------------------------------------------------
void dbExport::WriteFile(QString name, QString text){
    QFile file(path + "/" + name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&file);
    out << text << "\n";
}
//----------------------------------------------------------------------------------------------
