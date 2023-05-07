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
#include <QTextStream>
#include <QCryptographicHash>
#include <QProcess>
#include <QDateTime>
#include <QDebug>
#include <QTextDocument>
#include <fstream>
#include <iostream>


#include "includes/import.h"
#include "includes/folder.h"
#include "includes/compress.h"
#include "includes/strfunction.h"
#include "includes/sqlfunctions.h"

using namespace std;

//----------------------------------------------------------------------------------------------
dbImport::dbImport(QWidget *parent, QString pathBackup) : QWidget(parent){
    alfeFile = pathBackup;
    if(encyclopedia.getUserIndex() == -1)
        id_usuario = 1;
    else
        id_usuario = encyclopedia.getUserIndex();
}
//----------------------------------------------------------------------------------------------
void dbImport::setMediaDir(QString name){
    mediadir = name;
}
//----------------------------------------------------------------------------------------------
void dbImport::uncompress(){
    pathImp = __DIR_CACHE__;

    QDir *dir = new QDir;
    if (dir->exists(pathImp)) {
        Folder *folder = new Folder;
        folder->remDir(pathImp);
        delete folder;
    }
    delete dir;

    QString program = "unzip";

    QStringList arg;
    arg << alfeFile << "-d" << pathImp;

    QProcess *unzip = new QProcess;
    unzip->setReadChannel(QProcess::StandardOutput);

    unzip->start(program,arg);

    QByteArray output;

    while (unzip->waitForReadyRead() ){
        output += unzip->readAll();
        QString str = output.constData();
        qDebug() << str;
    }
}
//----------------------------------------------------------------------------------------------
void dbImport::cleanTemporalFiles(){
    Folder folder;
    folder.remDir(__DIR_CACHE__);
}

void dbImport::PruebaGuardarMedia(){
    QMapIterator<int, int> a(mapArticles);
    QMapIterator<int, int> b(mapAutors);
    QMapIterator<int, int> c(mapLevels);
    QMapIterator<int, int> d(mapMedias);

    Prueba("Articulos", &a);
    Prueba("Autores", &b);
    Prueba("Temas", &c);
    Prueba("Medias", &d);
}
//----------------------------------------------------------------------------------------------
void dbImport::createFileSystem(){
    mediadir = AlarifeDb.getFileName();

    QDir path(QDir::homePath());

    if (!path.exists("Alarife")){
      path.mkdir("Alarife");
      path = QDir::homePath()+"/Alarife";
    } else {
        path = QDir::homePath()+"/Alarife";
    }

    if (!path.exists(mediadir)) path.mkdir(mediadir);

    path = QDir::homePath() + "/Alarife/" + mediadir;
}
//----------------------------------------------------------------------------------------------
void dbImport::newEncyclopedia(){
    QString nombre(getStripHTMLTags(QString("%1/den.alfe").arg(pathImp)));

    QString resumen(getCodeToHtml(QString("%1/der.alfe").arg(pathImp)));

    contentReplaceMedias(resumen);

    QDateTime now = QDateTime::currentDateTime();

    qryImport = QSqlQuery(QString("INSERT INTO enciclopedia (id_enciclopedia, nombre, resumen, "
                                  "fecha_actualizacion) VALUES (%1,'%2','%3', '%4')").
                                 arg(sqlAutoIncremental("id_enciclopedia", "enciclopedia")).
                                 arg(nombre).
                                 arg(resumen).
                                 arg(now.toString("yyyy-MM-dd hh:mm:ss")));

    id_enciclopedia = sqlAutoIncremental("id_enciclopedia", "enciclopedia")-1;

    qryImport = QSqlQuery(QString("INSERT INTO enc_usuario (enciclopedia, usuario, "
                                  "tipousuario) VALUES (%1, 1, 1)").
                                 arg(id_enciclopedia));

    insertExtensions();

    createUser();

    id_nivel = 0;    
}
//----------------------------------------------------------------------------------------------
void dbImport::addEncyclopedia(int father){
    id_enciclopedia = encyclopedia.getIndex();
    QString nombre(getStripHTMLTags(QString("%1/den.alfe").arg(pathImp)));
    QString resumen(getStripHTMLTags(QString("%1/der.alfe").arg(pathImp)));

    contentReplaceMedias(resumen);

    qryImport= QSqlQuery(QString("SELECT MAX(orden) AS max FROM nivel WHERE "
                                 "padre = %1 AND enciclopedia = %2").arg(father).
                         arg(id_enciclopedia));
    qryImport.first();

    int orden = 0;

    if (qryImport.value(0).toInt() > 0) orden = qryImport.value(0).toInt();
    orden += 1;
    id_nivel = sqlAutoIncremental("id_nivel","nivel");

    qryImport= QSqlQuery(QString("INSERT INTO nivel (id_nivel, nombre, resumen, padre, "
                                 "usuario, enciclopedia, orden) VALUES (%1,'%2', '%3', "
                                 "%4, %5, %6, %7)").arg(id_nivel).
                         arg(nombre).arg(resumen).arg(father).arg(id_usuario).
                         arg(id_enciclopedia).arg(orden));
}
//----------------------------------------------------------------------------------------------
void dbImport::mergeLevels(int father){
    id_enciclopedia = encyclopedia.getIndex();
    id_nivel = father;
}
//----------------------------------------------------------------------------------------------
void dbImport::createUser(){
    QCryptographicHash *md5 = new QCryptographicHash(QCryptographicHash::Md5);
    QByteArray *byte = new QByteArray;
    md5->addData(byte->append("alarife"));

    qryImport= QSqlQuery(QString("INSERT INTO usuario (id_usuario, nombre, usuario, "
                                 " clave, tipousuario) VALUES (%1, 'Editor Jefe', 'alarife', "
                                 "'%2', 1)").
                         arg(sqlAutoIncremental("id_usuario", "usuario")).
                         arg(md5->result().data()));
}
//----------------------------------------------------------------------------------------------
void dbImport::importMedias(){
    PruebascanDir(pathImp, "*.md.alfe", &mapMedias);

    QMapIterator<int, int> iteMedia = mapMedias;
    while (iteMedia.hasNext()) {
         iteMedia.next();
         insertMedias(iteMedia.key());
    }
}
//----------------------------------------------------------------------------------------------
void dbImport::importLevels(){
    readFatherList();

    QMapIterator<int, int> iteLevel = mapLevels;
    while (iteLevel.hasNext()) {
         iteLevel.next();
         insertLevels(iteLevel.key());
    }
}
//----------------------------------------------------------------------------------------------
void dbImport::importArticles(){
    PruebascanDir(pathImp, "*.an.alfe", &mapArticles);
    
    qryImport= QSqlQuery("BEGIN TRANSACTION;");

    QMapIterator<int, int> iteArticle = mapArticles;
    while (iteArticle.hasNext()) {
         iteArticle.next();
         insertArticles(iteArticle.key());
    }

    qryImport = QSqlQuery("COMMIT;");

    cleanCss();
}
//----------------------------------------------------------------------------------------------
void dbImport::importRelationArticles(){
    QFile *file = new QFile(pathImp + "/articuloarticulos.alfe");

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream articuloarticulos(file);
    QList<QString> items;

    while (!articuloarticulos.atEnd()){
        items.clear();
        items = articuloarticulos.readLine().split(" ");

        if (items.count() == 2){

            int x = items.at(0).toInt();
            int y = items.at(1).toInt();

            qryImport= QSqlQuery(QString("INSERT INTO articuloarticulos (articulo, IdArticuloLink) "
                                         "VALUES (%1, %2)").arg(mapArticles.value(x)).
                                 arg(mapArticles.value(y)));
        }

    }
}
//----------------------------------------------------------------------------------------------
void dbImport::importAuthor(){
    PruebascanDir(pathImp, "*.da.alfe", &mapAutors);

    QMapIterator<int, int> iteAutor = mapAutors;
    while (iteAutor.hasNext()) {
         iteAutor.next();
         insertAutors(QString("%1/%2.da.alfe").arg(pathImp).arg(iteAutor.key()), iteAutor.key());
    }
}
//----------------------------------------------------------------------------------------------
void dbImport::importTeamColaborator(){
    QFile *file = new QFile(pathImp + "/enc_autor.alfe");
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream enc_autor(file);
    QList<QString> items;

    while (!enc_autor.atEnd()){
        items.clear();
        items = enc_autor.readLine().split(" ");

        if (items.count() == 2){

            qryImport= QSqlQuery(QString("INSERT INTO enc_autor (enciclopedia, autor, "
                                         "colaborador, equipo) VALUES (%1, %2, %3, %4)").
                                        arg(id_enciclopedia).
                                        arg(mapAutors.value(items.at(0).toInt())).
                                        arg(items.at(1).toInt()).
                                        arg(items.at(2).toInt()));
        }
    }
    delete file;
}
//----------------------------------------------------------------------------------------------
void dbImport::importRelationAuthors(){
    QFile *file = new QFile(pathImp + "/articuloautores.alfe");
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream *articuloautores = new QTextStream(file);
    QList<QString> items;

    while (!articuloautores->atEnd()){
        items.clear();
        items = articuloautores->readLine().split(" ");

        if (items.count() == 2){

            qryImport= QSqlQuery(QString("INSERT INTO articuloautores (articulo, autor) VALUES (%1, %2)").
                        arg(mapArticles.value(items.at(0).toInt())).
                        arg(mapAutors.value(items.at(1).toInt())));
        }
    }
    delete articuloautores;
    delete file;
}
//----------------------------------------------------------------------------------------------
void dbImport::insertExtensions(){
    qryImport = QSqlQuery(QString("INSERT INTO enc_tipo_ext (enciclopedia, archivo, extension) "
                                  "VALUES (%1, 'ExtImage', '*.bmp; *.jpg; *.png; *.gif');").
                          arg(id_enciclopedia));

    qryImport = QSqlQuery(QString("INSERT INTO enc_tipo_ext (enciclopedia, archivo, extension) "
                                  "VALUES (%1, 'ExtSound', '*.ogg; *.mp3; *.mp4; *.wav; *.wma');").
                          arg(id_enciclopedia));

    qryImport = QSqlQuery(QString("INSERT INTO enc_tipo_ext (enciclopedia, archivo, extension) "
                                  "VALUES (%1, 'ExtVideo', '*.avi; *.mpg; *.vob');").
                          arg(id_enciclopedia));

    qryImport = QSqlQuery(QString("INSERT INTO enc_tipo_ext (enciclopedia, archivo, extension) "
                                  "VALUES (%1, 'ExtXMedia', '*.pdf; *.txt; *.rtf; *.odt; *.doc');").
                          arg(id_enciclopedia));
}
//----------------------------------------------------------------------------------------------
void dbImport::insertMedias(int old){

    QString descripcion(readMediaDescription(QString("%1.md.alfe").arg(old)));
    QString extension(readFileToString(QString("%1.me.alfe").arg(old)));
    QString tipo(readFileToString(QString("%1.mt.alfe").arg(old)));

    mapMedias[old] = sqlAutoIncremental("id_adjunto", "adjuntos");

    qryImport = QSqlQuery(QString("INSERT INTO adjuntos(id_adjunto, descripcion, "
                                  "extension, tipo) VALUES(%1,'%2','%3', '%4')").
                          arg(mapMedias.value(old)).
                          arg(descripcion).arg(extension).arg(tipo));

    if (QFile::exists(QString("%1/%2.%3").arg(pathImp).arg(mapMedias.value(old)).arg(extension))){
        QString from(QString("%1/%2.%3").arg(pathImp).arg(mapMedias.value(old)).arg(extension));
        QString to(QString("%1/Alarife/%2/%3.%4").arg(QDir::homePath()).
                   arg(mediadir).arg(mapMedias.value(old)).arg(extension));
        QFile::copy(from, to);
    }
}
//----------------------------------------------------------------------------------------------
void dbImport::summaryReplaceMedias(QString &text, int *logo){
    text = text.simplified();

    page.mainFrame()->setHtml(text);

    QString imgFile;

    QWebElement img;
    img = page.mainFrame()->findFirstElement("img");
    imgFile = img.attribute("alt");

    if (QFileInfo(QString("%1/%2").arg(pathImp).arg(imgFile)).baseName().toInt() != 0){

        int i = QFileInfo(QString("%1/%2").arg(pathImp).arg(imgFile)).baseName().toInt();
        text.replace(imgFile,
                     QString("%1.%2").
                            arg(mapMedias.value(i)).
                            arg(QFileInfo(QString("%1/%2").
                                                 arg(pathImp).
                                                 arg(imgFile)
                                          ).suffix()),
                     Qt::CaseSensitive);
        *logo = mapMedias.value(i);

    }else *logo = 0;
}
//----------------------------------------------------------------------------------------------
void dbImport::contentReplaceMedias(QString &text){
    text = text.simplified();

    page.mainFrame()->setHtml(text);

    QWebElementCollection imgsList = page.mainFrame()->findAllElements("img");

    QString imgName, imgPath, str;

    foreach(QWebElement img, imgsList) {
        imgName = img.attribute("alt");
        if (QFileInfo(QString("%1/%2").arg(pathImp).arg(imgName)).baseName().toInt() != 0){

            int i =QFileInfo(QString("%1/%2").arg(pathImp).arg(imgName)).baseName().toInt();
            text.replace(imgName,
                         QString("%1.%2").
                                arg(mapMedias.value(i)).
                                arg(QFileInfo(QString("%1/%2").arg(pathImp).arg(imgName)).suffix()),
                         Qt::CaseSensitive);
        }
    }

}
//----------------------------------------------------------------------------------------------
void dbImport::insertLevels(int old){

    QString nombre(getStripHTMLTags(QString("%1/%2.nn.alfe").arg(pathImp).arg(old)));
    QString orden(readFileToString(QString("%1.no.alfe").arg(old)));

    int padre;

    if(readFileToInt(QString("%1.np.alfe").arg(old)) == 0)
        padre = id_nivel;
    else{
        padre = old;
    }


    int image = readFileToInt(QString("%1.nl.alfe").arg(old));

    QString resumen(getStripHTMLTags(QString("%1/%2.nr.alfe").arg(pathImp).arg(old)));

    if (image == -1) summaryReplaceMedias(resumen, &image);


    resumen.remove(getSubString(resumen, "<img", "\" />", 4), Qt::CaseSensitive);

    mapLevels[old] = sqlAutoIncremental("id_nivel", "nivel");

    qryImport = QSqlQuery(QString("INSERT INTO nivel (id_nivel, nombre, resumen, padre, usuario, "
                                  "enciclopedia, orden, logo) VALUES (%1, '%2', '%3', %4, %5, %6, %7, %8)").
                          arg(mapLevels.value(old)).arg(nombre).arg(resumen).arg(padre).
                          arg(id_usuario).arg(id_enciclopedia).arg(orden).arg(image));
}
//----------------------------------------------------------------------------------------------
void dbImport::insertArticles(int old){

    nombre = QString (getStripHTMLTags(QString("%1/%2.an.alfe").arg(pathImp).arg(old)));

    contenido = QString (getStripHTMLTags(QString("%1/%2.ac.alfe").arg(pathImp).arg(old)));

    contenido.replace("jscripts/tiny_mce/plugins/advimage", "..", Qt::CaseSensitive);

    contentReplaceMedias(contenido);

    resumen = QString (getStripHTMLTags(QString("%1/%2.ar.alfe").arg(pathImp).arg(old)));

    int image;

    summaryReplaceMedias(resumen, &image);

    claves = QString (getStripHTMLTags(QString("%1/%2.acl.alfe" ).arg(pathImp).
                                        arg(old)));

    int pos = mapLevels.key(readFileToInt(QString("%1.ani.alfe").arg(old)));

    int nivel;

    if(readFileToInt(QString("%1.ani.alfe").arg(old)) == 0){
        nivel = id_nivel;
    }else{
        nivel = mapLevels.value(pos);
    }

    mapArticles[old] = sqlAutoIncremental("id_articulo", "articulo");

    QString date1(readFileToString(QString("%1.dt1.alfe").arg(mapArticles.value(old))));
    QString date2(readFileToString(QString("%1.dt2.alfe").arg(mapArticles.value(old))));

    if (date1.isEmpty()) date1 = getActualDataTime();
    if (date2.isEmpty()) date2 = getActualDataTime();

    qryImport = QSqlQuery(QString("INSERT INTO articulo (id_articulo, nombre, resumen, contenido, "
                                  "claves, nivel, usuario, enciclopedia, fecha1, fecha2) values "
                                  "(%1, '%2', '%3', '%4', '%5', %6, %7, %8, '%9', '%10')").
                          arg(mapArticles.value(old)).
                          arg(nombre).arg(resumen).arg(contenido).arg(claves).arg(nivel).
                          arg(id_usuario).arg(id_enciclopedia).
                          arg(date1).
                          arg(date2));
}
//----------------------------------------------------------------------------------------------
void dbImport::insertAutors(QString path, int old){
    QList<QString> *lst_autor = new QList<QString>;
    ifstream *in = new ifstream(path.toStdString().c_str());
    string s;

    while(getline(*in, s)) {
        QString line(s.c_str());
        lst_autor->append(line);
    }

    QString nombre = QString(lst_autor->at(0)).toUtf8();
    QString titulo = QString(lst_autor->at(1)).toUtf8();
    QString cargo  = QString(lst_autor->at(2)).toUtf8();
    QString correo = QString(lst_autor->at(3)).toUtf8();
    QString foto(lst_autor->at(4));

    if (findAuthor(nombre) != -1){
        mapAutors[old] = findAuthor(nombre);
    }else{
        int foto2;
        if (foto.toInt() == 0){
            foto2 = 0;
        }

        
        mapAutors[old] = sqlAutoIncremental("id_autor", "autor");

        qryImport= QSqlQuery(QString("INSERT INTO autor (id_autor, nombre, titulo, cargo, correo, "
                                     "foto) VALUES (%1, '%2','%3','%4','%5',%6)").
                             arg(mapAutors.value(old)).
                             arg(nombre).
                             arg(titulo).
                             arg(cargo).
                             arg(correo).
                             arg(foto2));
    }

    delete lst_autor;
    delete in;    
}
//----------------------------------------------------------------------------------------------
QString dbImport::readMediaDescription(QString name){
    QString path(pathImp + "/" + name),
            line;
    ifstream *file = new ifstream(path.toStdString().c_str());
    string str;

    getline(*file,str);
    line.append(str.c_str());

    return line;
}
//----------------------------------------------------------------------------------------------
QString dbImport::readFileToString(QString name){
    QFile *file_0 = new QFile(pathImp + "/" + name);

    if (!file_0->open(QIODevice::ReadOnly | QIODevice::Text)) return "";

    QTextStream *stream_0 = new QTextStream(file_0);
    QString text(stream_0->readLine());

    delete stream_0;
    delete file_0;
    return text;
}
//----------------------------------------------------------------------------------------------
void dbImport::readFatherList(/*QString name*/){
    QList<int> father;

    QFile *file_1 = new QFile(QString("%1/orden.nl.alfe").arg(pathImp));

    if (!file_1->open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream *stream_1 = new QTextStream(file_1);

    while (!stream_1->atEnd()) father.append(stream_1->readLine().toInt());

    for (int i = 0; i < father.count(); i ++){
        mapLevels.insert(father[i], 0);
    }

    delete stream_1;
    delete file_1;
}
//----------------------------------------------------------------------------------------------
int dbImport::readFileToInt(QString name){
    QFile *file_2 = new QFile(pathImp + "/" + name);

    if (!file_2->open(QIODevice::ReadOnly | QIODevice::Text)) return -1;

    QTextStream *stream_2 = new QTextStream(file_2);

    int i = stream_2->readLine().toInt();

    delete stream_2;
    delete file_2;
    return i;
}
//----------------------------------------------------------------------------------------------
void dbImport::scanDir(QString dirpath, QString ext, DinArray **a){
    QDir dir(dirpath);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setSorting(QDir::Name);

    QList<QString> filters;

    filters << ext;
    dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList();

    (*a) = new DinArray(list.size());

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        (*a)->array[i][0] = fileInfo.baseName().toInt();
    }
}

void dbImport::PruebascanDir(QString dirpath, QString ext, QMap<int, int> *map){
    QDir dir(dirpath);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setSorting(QDir::Name);

    dir.setNameFilters(QStringList()<< ext);

    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        map->insert(fileInfo.baseName().toInt(), 0);
    }
}
//----------------------------------------------------------------------------------------------
int dbImport::findAuthor(QString name){
    qryImport = QSqlQuery(QString("SELECT id_autor FROM autor WHERE nombre = '%1'").arg(name));
    qryImport.first();

    if (qryImport.isValid())
        return qryImport.value(0).toInt();
    else return -1;
}
//----------------------------------------------------------------------------------------------
void dbImport::cleanCss(){
    qryImport = QSqlQuery("SELECT id_articulo, contenido FROM articulo "
                          "WHERE contenido LIKE  '<!-- #toc%'");
    QString text;

    while (qryImport.next()){
        text = qryImport.value(1).toString().simplified();

        while (text.contains("<!-- #toc", Qt::CaseInsensitive)){
            text.remove(getSubString(text, "<!-- #toc", "--><div>", 3), Qt::CaseSensitive);
        }
        text.remove("'", Qt::CaseSensitive);
        text.remove("<p style=\"text-align: right;\" dir=\"ltr\">", Qt::CaseSensitive);
        text.remove("<span style=\"font-size: small;\">", Qt::CaseSensitive);
        text.remove("<span lang=\"es-ES\">", Qt::CaseSensitive);

        sqlUpdateRecord("articulo",
                        QString("contenido = '%1' ").arg(text),
                        QString("WHERE id_articulo = %1 ").arg(qryImport.value(0).toInt()));
    }
}


void dbImport::Prueba(QString sec, QMapIterator<int, int> *i){
    QFile file(QString("/tmp/out-%1.txt").arg(sec));

    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream out(&file);

    out << "Medias Importadas" << " [" << sec << "]\n";

    while (i->hasNext()) {
        i->next();
        out << "old [" << i->key() << "]   new [" << i->value() << "]\n";
    }
}
