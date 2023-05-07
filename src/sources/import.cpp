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

//#include <QCoreApplication>
#include <QDir>
#include <QTextStream>
#include <QCryptographicHash>
#include <QProcess>
#include <QDebug>
#include <QTextDocument>
#include <fstream>
#include <iostream>
#include <QDir>

#include "../includes/alarife.h"
#include "../includes/import.h"
#include "../includes/folder.h"
#include "../includes/strfunction.h"
#include "../includes/sqlfunctions.h"

using namespace std;


dbImport::dbImport(QWidget *parent, QString pathBackup, int user, int id) : QWidget(parent)
{
    alfeFile = pathBackup;
    user == -1 ? id_usuario = 1 : id_usuario = id ;
    id_enciclopedia = id;
    merge = false;
}

void dbImport::setMediaDir(QString name)
{
    mediadir = name;
}

void dbImport::uncompress()
{
    pathImp = __DIR_CACHE__;

    QDir *dir = new QDir;
    if (dir->exists(pathImp))
    {
        Folder *folder = new Folder;
        folder->remFiles(pathImp);
        delete folder;
    }
    else
        dir->mkdir(pathImp);
    delete dir;

    QString path = __SYS_DIR__;
    if (!path.contains("bin/"))path = __SYS_DIR__ + "bin/";

    program = path + "7z" + __EXT__;

    QStringList arg;
    arg << "e" << "-tzip"
        << "-o" + pathImp
        << alfeFile;

    QProcess *process = new QProcess;
    process->setReadChannel(QProcess::StandardOutput);
    process->start(program, arg);

    QByteArray output;
    while (!process->waitForFinished())
    {
        output += process->readAll();
        QString str = output.constData();
        qDebug() << str;
    }

    delete process;
}

void dbImport::cleanTemporalFiles()
{
    Folder folder;
    folder.remFiles(__DIR_CACHE__);
    folder.remDir(__DIR_CACHE__);
}

void dbImport::createFileSystem()
{
    QDir *dir = new QDir;
    if (!dir->exists(__DIR_FILES__ + "/Alarife"))
        dir->mkdir(__DIR_FILES__ + "/Alarife");

    if (!dir->exists(__DIR_FILES__ + "/Alarife/" + mediadir))
        dir->mkdir(__DIR_FILES__ + "/Alarife/" + mediadir);
    delete dir;
}

void dbImport::newEncyclopedia()
{
    enombre = getStripHTMLTags(QString("%1/den.alfe").arg(pathImp));
    QString resumen(getCodeToHtml(QString("%1/der.alfe").arg(pathImp)));

    if (resumen.contains("<body")) resumen = selectUndefineArea(resumen,"body");

    contentReplaceMedias(resumen);
    id_enciclopedia = sqlAutoIncremental("id_enciclopedia", "enciclopedia");

    sqlInsertRecord_N("enciclopedia",
                    "id_enciclopedia, nombre, resumen, fecha_actualizacion",
                    QString("%1,<&~%2~&>,<&~%3~&>, <&~%4~&>").
                            arg(id_enciclopedia).
                            arg(enombre).
                            arg(resumen).
                            arg(getActualDataTime()));

    sqlInsertRecord_N("enc_usuario",
                    "enciclopedia, usuario, tipousuario",
                    QString("%1, 1, 1").arg(id_enciclopedia));

    insertExtensions();

    createUser();

    id_nivel = 0;
}

void dbImport::addEncyclopedia(int father)
{
    enombre = getStripHTMLTags(QString("%1/den.alfe").arg(pathImp));
    QString resumen(getCodeToHtml(QString("%1/der.alfe").arg(pathImp)));

    if (resumen.contains("<body")) resumen = selectUndefineArea(resumen,"body");

    int orden = sqlAutoIncremental("orden",
                                   "nivel",
                                   QString("padre = %1 AND enciclopedia = %2").
                                           arg(father).
                                           arg(id_enciclopedia));

    id_nivel = sqlAutoIncremental("id_nivel","nivel");

    sqlInsertRecord_N("nivel",
                    "id_nivel, nombre, resumen, padre, usuario, enciclopedia, orden",
                    QString("%1,<&~%2~&>, <&~%3~&>, %4, %5, %6, %7").
                            arg(id_nivel).
                            arg(enombre).
                            arg(resumen).
                            arg(father).
                            arg(id_usuario).
                            arg(id_enciclopedia).
                            arg(orden));
}

void dbImport::mergeLevels(int father)
{
    enombre = getStripHTMLTags(QString("%1/den.alfe").arg(pathImp));
    id_nivel = father;
    merge = true;
}

void dbImport::createUser()
{
    QCryptographicHash *md5 = new QCryptographicHash(QCryptographicHash::Md5);
    QByteArray *byte = new QByteArray;
    md5->addData(byte->append("alarife"));

    sqlInsertRecord_N("usuario",
                    "id_usuario, nombre, usuario,  clave, tipousuario",
                    QString("%1, <&~Editor Jefe~&>, <&~alarife~&>, <&~%2~&>, 1").
                            arg(sqlAutoIncremental("id_usuario", "usuario")).
                            arg(md5->result().data()));
}

void dbImport::importMedias()
{
    scanDir(pathImp, "*.md.alfe", &linkMedias);
    linkMedias->Sorting();

    for(int i = 0; i < linkMedias->Count(); i++) insertMedias(i);
}

void dbImport::importLevels()
{
    readFatherList("orden.nl.alfe");
    qDebug() << "\nniveles: " << linkLevels->Count() << endl;
    for (int i = 0; i < linkLevels->Count(); i++) insertLevels(i);
}

void dbImport::importArticles()
{
    scanDir(pathImp, "*.an.alfe", &linkArticles );
    linkArticles->Sorting();

    id_key = sqlAutoIncremental("id_articulo","articulo");

    sqlCustomerQuery("BEGIN TRANSACTION;");

    for(int i = 0; i < linkArticles->Count(); i++) insertArticles(i);

    sqlCustomerQuery("COMMIT;");

    cleanCss();
}

void dbImport::importRelationArticles()
{
    QFile *file = new QFile(pathImp + "/articuloarticulos.alfe");

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream articuloarticulos(file);
    QList<QString> items;

    while (!articuloarticulos.atEnd())
    {
        items.clear();
        items = articuloarticulos.readLine().split(" ");

        if (items.count() == 2)
        {
            int x = linkArticles->getPosId(items.at(0).toInt());
            int y = linkArticles->getPosId(items.at(1).toInt());

            sqlInsertRecord_N("articuloarticulos",
                            "articulo, IdArticuloLink",
                            QString("%1, %2").
                                    arg(linkArticles->array[x][1]).
                                    arg(linkArticles->array[y][1]));
        }

    }
    file->close();
    delete file;
}

void dbImport::importAuthor()
{
    scanDir(pathImp, "*.da.alfe", &linkAutors);
    linkAutors->Sorting();
    for(int i = 0; i < linkAutors->Count(); i++)
        insertAutors(QString("%1/%2.da.alfe").
                     arg(pathImp).
                     arg(linkAutors->array[i][0]),
                     i);
}

void dbImport::importTeamColaborator()
{
    QFile *file = new QFile(pathImp + "/enc_autor.alfe");
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream enc_autor(file);
    QList<QString> items;

    while (!enc_autor.atEnd())
    {
        items.clear();
        items = enc_autor.readLine().split(" ");

        if (items.count() == 3)
        {
            int i = linkAutors->getPosId(items.at(0).toInt());

            sqlInsertRecord_N("enc_autor",
                            "enciclopedia, autor, colaborador, equipo",
                            QString("%1, %2, %3, %4").
                                    arg(id_enciclopedia).
                                    arg(linkAutors->array[i][1]).
                                    arg(items.at(1).toInt()).
                                    arg(items.at(2).toInt()));
        }
    }
    file->close();
    delete file;
}

void dbImport::importRelationAuthors()
{
    QFile *file = new QFile(pathImp + "/articuloautores.alfe");
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream *articuloautores = new QTextStream(file);
    QList<QString> items;

    while (!articuloautores->atEnd())
    {
        items.clear();
        items = articuloautores->readLine().split(" ");

        if (items.count() == 2)
        {
            int id_article = linkArticles->getPosId(items.at(0).toInt());
            int id_author = linkAutors->getPosId(items.at(1).toInt());

            sqlInsertRecord_N("articuloautores",
                            "articulo, autor",
                            QString("%1, %2").
                                    arg(linkArticles->array[id_article][1]).
                                    arg(linkAutors->array[id_author][1]));
        }
    }
    delete articuloautores;
    file->close();
    delete file;
}

void dbImport::importSlideShow()
{
    scanDir(pathImp, "*.sa.alfe", &linkSlide);
    linkSlide->Sorting();
    int page = 0;
    int k = 0;

    for(int i = 0; i < linkSlide->Count(); i++)
    {
        linkSlide->array[i][1] = sqlAutoIncremental("id_slide", "slide");

        QString type = readFileToString(QString("%1.st.alfe").arg(linkSlide->array[i][0]));
        int j = readFileToInt(QString("%1.sp.alfe").arg(linkSlide->array[i][0]));

        if (type == "a")
        {
            k = linkArticles->getPosId(j);
            page = linkArticles->array[k][1];
        }
        else if  (type == "n")
        {
            k = linkLevels->getPosId(j);
            page = linkLevels->array[k][1];
        }
        else if (type == "i")
        {
            if (id_nivel == 0)
            {
                page = j;
            }
            else if (id_nivel > 0 && !merge)
            {
                page = id_nivel;
                type = "n";
            }
        }

        sqlInsertRecord_N("slide",
                        "id_slide, id_pagina, tipo",
                        QString("%1, %2, <&~%3~&>").
                                arg(linkSlide->array[i][1]).
                                arg(page).
                                arg(type));

        insertSlides(linkSlide->array[i][0]);
    }
}

void dbImport::importGallery()
{
    QFile *file = new QFile(pathImp + "/galeria.alfe");
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream *galeria = new QTextStream(file);

    while (!galeria->atEnd())
    {
        QString img = galeria->readLine();
        if (!img.isEmpty())
        {
            int id = sqlAutoIncremental("id_galeria", "galeria");
            int i = linkMedias->getPosId(img.toInt());
            sqlInsertRecord_N("galeria",
                            "id_galeria, id_adjunto, id_enciclopedia",
                            QString("%1, %2, %3").
                                    arg(id).
                                    arg(linkMedias->array[i][1]).
                                    arg(id_enciclopedia));
        }
    }
    delete galeria;
    delete file;
}

void dbImport::importReference()
{
    scanDir(pathImp, "*.rfa.alfe", &linkReference);
    linkReference->Sorting();


    for(int i = 0; i < linkReference->Count(); i++)
    {
        int id_referencia = readFileToInt(QString("%1.rfi.alfe").arg(linkReference->array[i][0]));
        int j = readFileToInt(QString("%1.rfa.alfe").arg(linkReference->array[i][0]));
        int k = linkArticles->getPosId(j);

        int enlace = 0;
        if (id_nivel == 0)
        {
            enlace = readFileToInt(QString("%1.rfe.alfe").arg(linkReference->array[i][0]));
        }else{
            int l = readFileToInt(QString("%1.rfe.alfe").arg(linkReference->array[i][0]));
            if (l > 0)
            {
                int x = linkArticles->getPosId(l);
                enlace = linkArticles->array[x][1];
            }
        }

        QString nombre(readFileToString(QString("%1.rfn.alfe").arg(linkReference->array[i][0])));
        QString resumen(readFileToString(QString("%1.rfr.alfe").arg(linkReference->array[i][0])));
        QString texto(readFileToString(QString("%1.rft.alfe").arg(linkReference->array[i][0])));

        sqlInsertRecord_N("referencias",
                        "id_referencia, articulo, enlace, nombre, resumen, texto",
                        QString("%1, %2, %3, <&~%4~&>, <&~%5~&>, <&~%6~&>").
                                arg(id_referencia).
                                arg(linkArticles->array[k][1]).
                                arg(enlace).
                                arg(nombre).
                                arg(resumen).
                                arg(texto));
    }
}

void dbImport::insertExtensions()
{
    sqlInsertRecord_N("enc_tipo_ext",
                    "enciclopedia, archivo, extension",
                    QString("%1, <&~ExtImage~&>, <&~*.bmp; *.jpg; *.jpeg; *.png; *.gif; *.svg; *.tiff~&>").
                            arg(id_enciclopedia));

    sqlInsertRecord_N("enc_tipo_ext",
                    "enciclopedia, archivo, extension",
                    QString("%1, <&~ExtSound~&>, <&~*.ogg; *.mp2; *.mp3; *.wav; *.wma~&>").
                            arg(id_enciclopedia));

    sqlInsertRecord_N("enc_tipo_ext",
                    "enciclopedia, archivo, extension",
                    QString("%1, <&~ExtVideo~&>, <&~*.avi; *.mpg; *.vob; *.flv; *.webm; *.mp4~&>").
                            arg(id_enciclopedia));

    sqlInsertRecord_N("enc_tipo_ext",
                    "enciclopedia, archivo, extension",
                    QString("%1, <&~ExtXMedia~&>, <&~*.pdf; *.txt; *.rtf; *.odt; *.doc; *.zip; *.rar~&>").
                            arg(id_enciclopedia));
}

void dbImport::insertMedias(int old)
{
    QString descripcion(readMediaDescription(QString("%1.md.alfe").arg(linkMedias->array[old][0])));

    QString extension(readFileToString(QString("%1.me.alfe").arg(linkMedias->array[old][0])));
    QString tipo(readFileToString(QString("%1.mt.alfe").arg(linkMedias->array[old][0])));

    linkMedias->array[old][1] = sqlAutoIncremental("id_adjunto", "adjuntos");

    sqlInsertRecord_N("adjuntos",
                    "id_adjunto, descripcion, extension, tipo",
                    QString("%1, <&~%2~&>, <&~%3~&>, <&~%4~&>").
                            arg(linkMedias->array[old][1]).
                            arg(descripcion).
                            arg(extension).
                            arg(tipo));

    if (QFile::exists(QString("%1/%2.%3").arg(pathImp).arg(linkMedias->array[old][0]).arg(extension))){
        QString from(QString("%1/%2.%3").arg(pathImp).arg(linkMedias->array[old][0]).arg(extension));
        QString to(QString("%1/Alarife/%2/%3.%4").arg(QDir::homePath()).
                   arg(mediadir).arg(linkMedias->array[old][1]).arg(extension));
        QFile::copy(from, to);
    }
}

void dbImport::contentReplaceMedias(QString &text)
{
    text = text.simplified();
    text.replace("src=\"medias/", "src=\"../medias/", Qt::CaseSensitive);

    page.mainFrame()->setHtml(text);

    QWebElementCollection imgsList = page.mainFrame()->findAllElements("img");

    QString imgName;

    foreach(QWebElement img, imgsList)
    {
        imgName = img.attribute("alt");
        if (QFileInfo(QString("%1/%2").arg(pathImp).arg(imgName)).baseName().toInt() != 0)
        {
            int i = linkMedias->getPosId(QFileInfo(QString("%1/%2").arg(pathImp).arg(imgName)).
                                         baseName().toInt());
            text.replace(imgName, QString("%1.%2").arg(linkMedias->array[i][1]).
                         arg(QFileInfo(QString("%1/%2").arg(pathImp).arg(imgName)).suffix()),
                         Qt::CaseSensitive);
        }        
    }
}

void dbImport::insertLevels(int old)
{qDebug() << "insertLevels: " << old << endl;
    if (QFile(QString("%1/%2.nn.alfe").arg(pathImp).
              arg(linkLevels->array[old][0])).exists())
    {
        QString nombre(getStripHTMLTags(QString("%1/%2.nn.alfe").arg(pathImp).
                                     arg(linkLevels->array[old][0])));

        int padre;

        if(readFileToInt(QString("%1.np.alfe").arg(linkLevels->array[old][0])) == 0)
            padre = id_nivel;
        else{
            int i = linkLevels->getPosId(readFileToInt(QString("%1.np.alfe").
                                                       arg(linkLevels->array[old][0])));
            padre = linkLevels->array[i][1];
        }

        QString resumen(getCodeToHtml(QString("%1/%2.nr.alfe").arg(pathImp).
                                      arg(linkLevels->array[old][0])));

        if (resumen.contains("<body")) resumen = selectUndefineArea(resumen,"body");

        contentReplaceMedias(resumen);

        linkLevels->array[old][1] = sqlAutoIncremental("id_nivel", "nivel");

        int orden = sqlAutoIncremental("orden",
                                       "nivel",
                                       QString("padre = %1 AND enciclopedia = %2").
                                               arg(padre).
                                               arg(id_enciclopedia));

        sqlInsertRecord_N("nivel",
                        "id_nivel, nombre, resumen, padre, usuario, enciclopedia, orden",
                        QString("%1, <&~%2~&>, <&~%3~&>, %4, %5, %6, %7").
                                arg(linkLevels->array[old][1]).
                                arg(nombre).
                                arg(resumen).
                                arg(padre).
                                arg(id_usuario).
                                arg(id_enciclopedia).
                                arg(orden));
    }

}

void dbImport::insertArticles(int old)
{
    QString nombre = QString (getStripHTMLTags(QString("%1/%2.an.alfe").arg(pathImp).
                                 arg(linkArticles->array[old][0])));

    contenido = QString (getCodeToHtml(QString("%1/%2.ac.alfe").arg(pathImp).
                                          arg(linkArticles->array[old][0])));

    contenido.replace("jscripts/tiny_mce/plugins/advimage", "..", Qt::CaseSensitive);

    if (contenido.contains("<body")) contenido = selectUndefineArea(contenido,"body");

    contentReplaceMedias(contenido);

    resumen = QString (getCodeToHtml(QString("%1/%2.ar.alfe").arg(pathImp).
                                  arg(linkArticles->array[old][0])));
    resumen = strToPlainText(resumen);

    contentReplaceMedias(resumen);

    claves = QString (getStripHTMLTags(QString("%1/%2.acl.alfe" ).arg(pathImp).
                                    arg(linkArticles->array[old][0])));

    int pos = linkLevels->getPosId(readFileToInt(QString("%1.ani.alfe").
                                                 arg(linkArticles->array[old][0])));

    int nivel;

    if(readFileToInt(QString("%1.ani.alfe").arg(linkArticles->array[old][0])) == 0){
        nivel = id_nivel;
    }else{
        nivel = linkLevels->array[pos][1];
    }

    linkArticles->array[old][1] = id_key++;

    QString date1(readFileToString(QString("%1.dt1.alfe").arg(linkArticles->array[old][0])));
    QString date2(readFileToString(QString("%1.dt2.alfe").arg(linkArticles->array[old][0])));

    if (date1.isEmpty()) date1 = getActualDataTime();
    if (date2.isEmpty()) date2 = getActualDataTime();

    int orden = sqlAutoIncremental("orden",
                                   "articulo",
                                   QString("nivel = %1 AND enciclopedia = %2").
                                           arg(nivel).
                                           arg(id_enciclopedia));
    sqlInsertRecord_N("articulo",
                    "id_articulo, nombre, resumen, contenido, claves, "
                    "nivel, usuario, enciclopedia, fecha1, fecha2, orden",
                    QString("%1, <&~%2~&>, <&~%3~&>, <&~%4~&>, <&~%5~&>, "
                            "%6, %7, %8, <&~%9~&>, <&~%10~&>, %11").
                            arg(linkArticles->array[old][1]).
                            arg(nombre).
                            arg(resumen).
                            arg(contenido).
                            arg(claves).
                            arg(nivel).
                            arg(id_usuario).
                            arg(id_enciclopedia).
                            arg(date1).
                            arg(date2).
                            arg(orden));
}

void dbImport::insertAutors(QString path, int old)
{
    QList<QString> *lst_autor = new QList<QString>;
    ifstream *in = new ifstream(path.toStdString().c_str());
    string s;

    while(getline(*in, s))
    {
        QString line(s.c_str());
        lst_autor->append(strHtmlDecode(line));
    }

    QString nombre = QString(lst_autor->at(0));
    QString titulo = QString(lst_autor->at(1));
    QString cargo  = QString(lst_autor->at(2));
    QString correo = QString(lst_autor->at(3));
    QString foto(lst_autor->at(4));

    if (findAuthor(nombre) != -1)
    {
        linkAutors->array[old][1] = findAuthor(nombre);
    }else{
        int i, foto2;
        if (foto.toInt() == 0){
            foto2 = 0;
        }else{
            i = linkMedias->getPosId(foto.toInt());
            foto2 = linkMedias->array[i][1];
        }

        linkAutors->array[old][1] = sqlAutoIncremental("id_autor", "autor");

        sqlInsertRecord_N("autor",
                        "id_autor, nombre, titulo, cargo, correo, foto",
                        QString("%1, <&~%2~&>, <&~%3~&>, <&~%4~&>, <&~%5~&>, %6").
                                arg(linkAutors->array[old][1]).
                                arg(nombre).
                                arg(titulo).
                                arg(cargo).
                                arg(correo).
                                arg(foto2));
    }

    delete lst_autor;
    delete in;
}

void dbImport::insertSlides(int slide)
{
    QString path(QString("%1/%2.sa.alfe").arg(pathImp).arg(slide));

    ifstream *in = new ifstream(path.toStdString().c_str());
    string s;

    QStringList list;
    while(getline(*in, s))
    {
        QString line(s.c_str());
        if (!line.isEmpty())
        {
            qDebug() << "old slide image: " << line << endl;
            list = line.split(":");
            int i = linkMedias->getPosId(slide);
            int j = linkMedias->getPosId(list[0].toInt());

            int k = 0;
            if (list.count() == 2) k = list[1].toInt();
            sqlInsertRecord_N("adjuntos_slide",
                            "id_slide, id_adjunto, orden",
                            QString("%1, %2, %3").
                                    arg(linkSlide->array[i][1]).
                                    arg(linkMedias->array[j][1]).
                                    arg(k));
        }
    }
    delete in;
}

QString dbImport::readMediaDescription(QString name)
{
    QString line = getStripHTMLTags(pathImp + "/" + name);
    return line;
}

QString dbImport::readFileToString(QString name)
{
    QFile *file = new QFile(pathImp + "/" + name);

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) return "";

    QTextStream *stream = new QTextStream(file);
    QString text(stream->readLine());

    delete stream;
    delete file;
    return text;
}

void dbImport::readFatherList(QString name)
{
    QList<int> father;

    QFile *file = new QFile(pathImp + "/" + name);

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream *stream = new QTextStream(file);

    while (!stream->atEnd()) father.append(stream->readLine().toInt());

    linkLevels = new DinArray(father.count());

    for (int i = 0; i < father.count(); i ++)
    {
        linkLevels->array[i][0] = father[i];
    }

    delete stream;
    file->close();
    delete file;
}

int dbImport::readFileToInt(QString name)
{
    QFile *file = new QFile(pathImp + "/" + name);

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) return -1;

    QTextStream *stream = new QTextStream(file);

    int i = stream->readLine().toInt();

    delete stream;
    delete file;
    return i;
}

void dbImport::scanDir(QString dirpath, QString ext, DinArray **a)
{
    QDir dir(dirpath);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setSorting(QDir::Name);

    QList<QString> filters;

    filters << ext;
    dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList();

    (*a) = new DinArray(list.size());

    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        (*a)->array[i][0] = fileInfo.baseName().toInt();
    }
}

int dbImport::findAuthor(QString name)
{
    int swap = sqlGetIdValue("autor",
                             "id_autor",
                             QString("nombre = <&~%1~&>").arg(name));
    if (swap > 0)
        return swap;
    else
        return -1;
}

void dbImport::cleanCss()
{
    QSqlQuery qryImport = QSqlQuery("SELECT id_articulo, contenido FROM articulo "
                          "WHERE contenido LIKE  '<!-- #toc%'");
    QString text;

    while (qryImport.next())
    {
        text = qryImport.value(1).toString().simplified();
        while (text.contains("<!--", Qt::CaseInsensitive))
        {
            text.remove(getSubString(text, "<!--", "-->", 3), Qt::CaseSensitive);
        }
        sqlUpdateRecord_N("articulo",
                        QString("contenido = <&~%1~&> ").arg(text),
                        QString("WHERE id_articulo = %1 ").arg(qryImport.value(0).toInt()));
    }
}
