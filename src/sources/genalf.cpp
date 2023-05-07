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

#include "../includes/genalf.h"
#include "../includes/folder.h"
#include "../includes/sqlfunctions.h"
#include "../includes/strfunction.h"
#include "../includes/html.h"
#include "../includes/htmlarticles.h"
#include "../includes/htmldefault.h"
#include "../includes/htmlindex.h"
#include "../includes/htmllevels.h"
#include "../includes/jscript.h"


GenAlf::GenAlf(QWidget *parent, Book *encyc) :  QWidget(parent)
{
    encyclopedia = encyc;
    PrepareExtensionFiles();
    QString path = __APP_PATH__;
    if (path.contains("/bin")) path.remove("/bin");
    setTemplatePath(path + __DIR_TEMPLATE__ + encyclopedia->getTemplate() + "/files");
    avMode = 0;
    if (encyclopedia->getVideoMode() == 1) avMode += 1;
    if(encyclopedia->getAudioMode() == 1) avMode += 2;
}

void GenAlf::setTargetPath(QString targetPath)
{
    target = targetPath;
}

void GenAlf::setTemplatePath(QString templatePath)
{
    templates = templatePath;
}

void GenAlf::setIndex(int index)
{
    id_enciclopedia = index;

    qryHtml = QSqlQuery(QString("SELECT COUNT(id_galeria) FROM galeria INNER JOIN "
                         "adjuntos ON galeria.id_adjunto = adjuntos.id_adjunto "
                         "WHERE id_enciclopedia = %1").arg(id_enciclopedia));
    qryHtml.exec();
    qryHtml.first();
    qryHtml.value(0).toInt() <= 0 ? gallery = false : gallery = true;
}

void GenAlf::generateLevels()
{
    if (QFile::exists(templates + "/datos/nivel.htm"))
    {
        QQueue <int> queue;

        qryHtml= QSqlQuery(QString("SELECT DISTINCT padre FROM nivel WHERE "
                                   "enciclopedia = %1 ORDER BY padre").
                           arg(id_enciclopedia));
        qryHtml.exec();

        while (qryHtml.next()) queue.enqueue(qryHtml.value(0).toInt());

        htmlLevels *levels = new htmlLevels(id_enciclopedia, avMode);

        while (!queue.isEmpty())
        {
            int i = queue.dequeue();

            levels->setFatherValue(i);

            qryHtml = QSqlQuery(QString("SELECT id_nivel, nombre, resumen, padre FROM nivel "
                                        "WHERE padre = %1 order by orden").arg(i));

            qryHtml.exec();
            
            while (qryHtml.next()){
                levels->setOpenPath(templates + "/datos/nivel.htm");
                levels->setSavePath(target + QString("/datos/nivel_%1.htm").
                                    arg(qryHtml.value(0).toInt()));

                levels->setPrimaryKey(qryHtml.value(0).toInt());
                levels->openHtml();
                levels->clear();

                if (sqlAutoCountRow("nivel", QString("padre = %1").arg(qryHtml.value(0).toInt())) == 0)
                    vmenu = generateAreaVMenu(levels->getHtmlText(),qryHtml.value(3).toInt());
                else
                    vmenu = generateAreaVMenu(levels->getHtmlText(),qryHtml.value(0).toInt());

                hmenu = generateAreaHMenu(levels->getHtmlText(), i);

                levels->setLeveList(generateAreaLevels(levels->getHtmlText(), qryHtml.value(0).toInt()));
                levels->setArticleList(generateAreaArticles(levels->getHtmlText(), qryHtml.value(0).toInt()));
                levels->setVMenuArea(vmenu);
                levels->setHMenuArea(hmenu);
                levels->setTitleWindows(name_enc);
                levels->setSlideShow(generateSlideShow(qryHtml.value(0).toInt(), 'n'));
                levels->setTitle(qryHtml.value(1).toString());

                QString swap = "";

                if(qryHtml.value(2).toString().contains("<body"))
                {
                    swap = selectUndefineArea(qryHtml.value(2).toString(),"body");
                }else{
                    swap = qryHtml.value(2).toString();
                }

                swap.replace("src=\"medias/", "src=\"../medias/", Qt::CaseSensitive);

                levels->setSummary(swap);

                levels->changeValueElements();
                levels->saveHtml();

                if (qryHtml.value(2).toString().contains("alt=\""))
                {
                    SelectMediaInText(qryHtml.value(2).toString());
                }
            }
        }
    }
}

void GenAlf::generateArticles()
{
    if (QFile::exists(templates + "/datos/articulo.htm"))
    {
        QQueue<int> queue;

        qryHtml= QSqlQuery(QString("SELECT DISTINCT nivel FROM articulo WHERE "
                                   "enciclopedia = %1 ORDER BY nivel").
                           arg(id_enciclopedia));
        qryHtml.exec();

        while (qryHtml.next()) queue.enqueue(qryHtml.value(0).toInt());



        htmlArticles *articles = new htmlArticles(id_enciclopedia, avMode);

        while (!queue.isEmpty()){
            int i = queue.dequeue();

            articles->setFatherValue(i);

            QString order;
            encyclopedia->getLevelOrder() == 0 ? order = "orden" : order = "nombre";


            qryHtml = QSqlQuery(QString("SELECT articulo.id_articulo, articulo.nombre, articulo.resumen, "
                                        "articulo.contenido, articulo.fuente, articulo.fecha1, articulo.fecha2, "
                                        "articulo.nivel FROM articulo WHERE articulo.nivel = %1 "
                                        "ORDER By %2").arg(i).arg(order));

            while (qryHtml.next()){

                articles->setOpenPath(templates + "/datos/articulo.htm");
                articles->setSavePath(target + QString("/datos/articulo_%1.htm").
                                      arg(qryHtml.value(0).toInt()));

                articles->setPrimaryKey(qryHtml.value(0).toInt());
                articles->openHtml();
                articles->clear();

                if (sqlAutoCountRow("nivel", QString("padre = %1").arg(qryHtml.value(7).toInt())) == 0)
                    vmenu = generateAreaVMenu(articles->getHtmlText(),
                                              sqlGetIdValue("nivel",
                                                            "padre",
                                                            QString("id_nivel = %1").arg(qryHtml.value(7).toInt())));
                else
                    vmenu = generateAreaVMenu(articles->getHtmlText(),qryHtml.value(7).toInt());

                hmenu = generateAreaHMenu(articles->getHtmlText(), qryHtml.value(7).toInt());

                articles->setTitleWindows(name_enc);
                articles->setSlideShow(generateSlideShow(qryHtml.value(0).toInt(), 'a'));
                articles->setTitle(qryHtml.value(1).toString());
                articles->setSummary(qryHtml.value(2).toString());
                articles->setRelationsAuthors(generateAreaRelationsAuthors(qryHtml.value(0).toInt()));
                articles->setRelationsArticles(generateAreaRelationsArticles(articles->getHtmlText(),
                                                                             qryHtml.value(0).toInt()));

                if(qryHtml.value(3).toString().contains("<body")){
                    QString content = selectUndefineArea(qryHtml.value(3).toString(),"body");
                    articles->setContent(content);
                }else
                    articles->setContent(qryHtml.value(3).toString());

                int ref = sqlAutoCountRow("referencias", QString("articulo = %1").arg(qryHtml.value(0).toInt()));

                if (ref > 0)
                {
                    QSqlQuery *qryRef = new QSqlQuery(SQL_Reference.arg(qryHtml.value(0).toInt()));
                    qryRef->first();
                    articles->insertReferences(qryRef->value(0).toInt(),
                                               qryRef->value(1).toInt(),
                                               qryRef->value(2).toString(),
                                               qryRef->value(3).toString(),
                                               qryRef->value(4).toString());
                    while (qryRef->next())
                    {
                        articles->insertReferences(qryRef->value(0).toInt(),
                                                   qryRef->value(1).toInt(),
                                                   qryRef->value(2).toString(),
                                                   qryRef->value(3).toString(),
                                                   qryRef->value(4).toString());
                    }
                    delete qryRef;
                }

                articles->setVMenuArea(vmenu);
                articles->setHMenuArea(hmenu);
                articles->setSourceData(generateAreaSource(qryHtml.value(4).toString()));
                articles->setDates(qryHtml.value(5).toString(), qryHtml.value(6).toString());
                articles->changeValueElements();
                articles->saveHtml();

                if (qryHtml.value(3).toString().contains("alt=\""))
                    SelectMediaInText(qryHtml.value(3).toString());
            }
        }
    }
}

void GenAlf::createFileSystem()
{
    qryArea = QSqlQuery(QString("SELECT nombre FROM enciclopedia WHERE "
                                "id_enciclopedia = %1").arg(id_enciclopedia));
    qryArea.exec();
    qryArea.first();

    QString namedir = qryArea.value(0).toString();

    namedir.replace(" ","_");

    QDir dirTarget(target);
    if (dirTarget.exists(namedir) == false){
        dirTarget.mkdir(namedir);
        target.append("/" + namedir);
    } else {
        int sub=1;
        QString tmpName = namedir;
        while (dirTarget.exists(tmpName)){
            tmpName = QString("%1(%2)").arg(namedir).arg(sub);
            sub++;
        }
        target.append("/" + tmpName);
        dirTarget.mkdir(target);
    }

    dirTarget.mkdir(target + "/datos");

    Folder folder;
    folder.copyDir(templates + "/css", target + "/css");
    folder.copyDir(templates + "/imagenes", target + "/imagenes");
    folder.copyDir(templates + "/medias", target + "/medias");
    folder.copyDir(templates + "/script", target + "/script");
    PrepareInsertMedias();
}

void GenAlf::generateIndex()
{
    if (QFile::exists(templates + "/index.htm"))
    {
        qryHtml = QSqlQuery(QString("SELECT nombre, resumen, plantilla FROM enciclopedia "
                                    "WHERE id_enciclopedia = %1").arg(id_enciclopedia));
        qryHtml.first();
        name_enc = qryHtml.value(0).toString();

        htmlIndex *index = new htmlIndex(id_enciclopedia, avMode);
        index->setFatherValue(0);
        index->setOpenPath(templates + "/index.htm");
        index->setSavePath(target + "/index.htm");
        index->openHtml();
        GetTemplateType(index->getHtmlText());
        index->setTitle(name_enc);
        index->setSlideShow(generateSlideShow(id_enciclopedia, 'i'));

        QString content;
        qryHtml.value(1).toString().contains("<body") ? content = selectUndefineArea(qryHtml.value(1).toString(),"body") : content = qryHtml.value(1).toString();
        content.replace("src=\"../medias/", "src=\"medias/", Qt::CaseSensitive);
        content.replace("src=\"../imagenes/", "src=\"imagenes/", Qt::CaseSensitive);

        index->setSummary(content);

        index->setVMenuArea(generateAreaVMenu(index->getHtmlText(), -1).simplified());

        index->setLeveList(generateAreaLevels(index->getHtmlText(), 0));
        index->setArticleList(generateAreaArticles(index->getHtmlText(), 0));
        index->changeValueElements();
        index->saveHtml();
        delete index;

        SelectMediaInText(qryHtml.value(1).toString());
    }
}

void GenAlf::generateAuthor()
{
    if (QFile::exists(templates + "/autores.htm"))
    {
        qryHtml = QSqlQuery(QString("SELECT DISTINCT autor.nombre, autor.titulo, autor.cargo, "
                                    "autor.correo, autor.foto  FROM autor INNER JOIN "
                                    "articuloautores ON autor.id_autor = articuloautores.autor "
                                    "ORDER BY autor.nombre"));
        qryHtml.exec();

        htmlDefault *author = new htmlDefault;
        author->setOpenPath(templates + "/autores.htm");
        author->setSavePath(target + "/autores.htm");
        author->openHtml();
        mainframe.clear();

        while (qryHtml.next())
        {
            swap = selectArea(author->getHtmlText(),"datos");
            swap.replace("<nombre>",qryHtml.value(0).toString());
            swap.replace("<titulo>",qryHtml.value(1).toString());
            swap.replace("<cargo>",qryHtml.value(2).toString());
            swap.replace("<correo>",qryHtml.value(3).toString());
            swap.replace("<foto>",sqlGetImageName(qryHtml.value(4).toInt()));
            mainframe.append(swap);
        }
        ClearTag("datos", mainframe);
        author->setMainFrame(mainframe);
        author->changeValueElements();
        author->saveHtml();
        delete author;
    }
}

void GenAlf::generateCredits()
{
    if (QFile::exists(templates + "/creditos.htm"))
    {
        qryHtml = QSqlQuery(QString("SELECT DISTINCT autor.nombre, autor.titulo, autor.cargo, "
                                    "autor.correo, autor.foto FROM autor INNER JOIN "
                                    "enc_autor  ON autor.id_autor = enc_autor.autor WHERE "
                                    "enc_autor.enciclopedia = %1 AND enc_autor.equipo = 1 "
                                    "ORDER BY autor.nombre").arg(id_enciclopedia));
        qryHtml.exec();

        mainframe.clear();

        htmlDefault *credits = new htmlDefault;
        credits->setOpenPath(templates + "/creditos.htm");
        credits->setSavePath(target + "/creditos.htm");
        credits->openHtml();

        swap.clear();
        while (qryHtml.next())
        {
            swap = selectArea(credits->getHtmlText(), "datos");
            swap.replace("<nombre>", qryHtml.value(0).toString());
            swap.replace("<titulo>", qryHtml.value(1).toString());
            swap.replace("<cargo>",  qryHtml.value(2).toString());
            swap.replace("<correo>", qryHtml.value(3).toString());
            swap.replace("<foto>",   sqlGetImageName(qryHtml.value(4).toInt()));
            mainframe.append(swap);
        }
        ClearTag("datos", mainframe);
        credits->setMainFrame(mainframe);
        credits->changeValueElements();
        credits->saveHtml();
        delete credits;
    }
}

void GenAlf::generateHelp()
{
    if (QFile::exists(templates + "/ayuda.htm"))
    {
        htmlDefault *help = new htmlDefault;
        help->setOpenPath(templates + "/ayuda.htm");
        help->setSavePath(target + "/ayuda.htm");
        help->openHtml();
        help->saveHtml();
        delete help;
    }
}

void GenAlf::generateTeam()
{
    if (QFile::exists(templates + "/colaboradores.htm"))
    {
        qryHtml = QSqlQuery(QString("SELECT DISTINCT autor.nombre, autor.titulo, autor.cargo, "
                                    "autor.correo, autor.foto FROM autor INNER JOIN "
                                    "enc_autor  ON autor.id_autor = enc_autor.autor WHERE "
                                    "enc_autor.enciclopedia = %1 AND enc_autor.colaborador >= "
                                    "1 ORDER BY autor.nombre").arg(id_enciclopedia));
        mainframe.clear();

        htmlDefault *team = new htmlDefault;
        team->setOpenPath(templates + "/colaboradores.htm");
        team->setSavePath(target + "/colaboradores.htm");
        team->openHtml();

        swap.clear();
        while (qryHtml.next())
        {
            swap = selectArea(team->getHtmlText(),"datos");
            swap.replace("<nombre>", qryHtml.value(0).toString());
            swap.replace("<titulo>", qryHtml.value(1).toString());
            swap.replace("<cargo>",  qryHtml.value(2).toString());
            swap.replace("<correo>", qryHtml.value(3).toString());
            swap.replace("<foto>",   sqlGetImageName(qryHtml.value(4).toInt()));
            mainframe.append(swap);
        }
        ClearTag("datos", mainframe);
        team->setMainFrame(mainframe);
        team->changeValueElements();
        team->saveHtml();
        delete team;
    }
}

void GenAlf::generateGallery()
{
    if (gallery)
    {
        if (QFile::exists(templates + "/script/gallery.js"))
        {
            jScript *script = new jScript;
            script->setIndex(encyclopedia->getIndex());
            script->setOpenPath(templates + "/script/gallery.js");
            script->setSavePath(target + "/script/gallery.js");
            script->openJScript();
            script->changeValueGallery();
            script->saveJScript();
        }

        if (QFile::exists(templates + "/galeria.htm"))
        {
            htmlIndex *index = new htmlIndex(id_enciclopedia, 0);
            index->setFatherValue(0);
            index->setOpenPath(templates + "/galeria.htm");
            index->setSavePath(target + "/galeria.htm");
            index->openHtml();
            index->setTitle(name_enc);
            index->setVMenuArea(generateAreaVMenu(index->getHtmlText(), 0).simplified());
            index->changeValueElements();
            index->saveHtml();
            delete index;
        }

    }
}

void GenAlf::generateSearch()
{
    if (QFile::exists(templates + "/script/buscador.js"))
    {
        jScript *script = new jScript;
        script->setIndex(encyclopedia->getIndex());
        script->setOpenPath(templates + "/script/buscador.js");
        script->setSavePath(target + "/script/buscador.js");
        script->openJScript();
        script->changeValueElements();
        script->saveJScript();
    }
}

void GenAlf::generateMedia()
{
    QSqlQuery *qryMedia = new QSqlQuery;
    qryMedia->exec("SELECT DISTINCT nombre||'.'||extension AS file FROM media");

    qryMedia->first();
    QString file = QString("%1/%2").arg(encyclopedia->getMediaPath()).arg(qryMedia->value(0).toString());

    if (QFile::exists(file))
    {
        QFile::copy(file, QString("%1/medias/%2").arg(target).arg(qryMedia->value(0).toString()));
    }

    while (qryMedia->next())
    {
        file = QString("%1/%2").arg(encyclopedia->getMediaPath()).arg(qryMedia->value(0).toString());

        if (QFile::exists(file))
        {
            QFile::copy(file, QString("%1/medias/%2").arg(target).arg(qryMedia->value(0).toString()));
        }
    }
}

void GenAlf::generateStadistic()
{
    QString est_aut, est_niv, est_art, est_ima, est_son, est_vid, est_xme;

    QSqlQuery *qryTmp = new QSqlQuery;

    qryTmp->exec(QString("SELECT COUNT(id_articulo) FROM articulo "
                         "WHERE enciclopedia = %1").arg(encyclopedia->getIndex()));
    qryTmp->first();
    est_art = qryTmp->value(0).toString();
    qryTmp->clear();

    qryTmp->exec(QString("SELECT COUNT(DISTINCT id_autor) FROM autor INNER JOIN "
                         "articuloautores ON articuloautores.autor = autor.id_autor"));
    qryTmp->first();
    est_aut = qryTmp->value(0).toString();
    qryTmp->clear();

    qryTmp->exec(QString("SELECT COUNT(DISTINCT nombre) FROM media WHERE %1'").arg(extImagen));
    qryTmp->first();
    est_ima = qryTmp->value(0).toString();
    qryTmp->clear();

    qryTmp->exec(QString("SELECT COUNT(id_nivel) FROM nivel "
                         "WHERE enciclopedia = %1").arg(encyclopedia->getIndex()));
    qryTmp->first();
    est_niv = qryTmp->value(0).toString();
    qryTmp->clear();

    qryTmp->exec(QString("SELECT COUNT(DISTINCT nombre) FROM media WHERE %1'").arg(extSonido));
    qryTmp->first();
    est_son = qryTmp->value(0).toString();
    qryTmp->clear();

    qryTmp->exec(QString("SELECT COUNT(DISTINCT nombre) FROM media WHERE %1'").arg(extVideo));
    qryTmp->first();
    est_vid = qryTmp->value(0).toString();
    qryTmp->clear();

    qryTmp->exec(QString("SELECT COUNT(DISTINCT nombre) FROM media WHERE %1'").arg(extXMedia));
    qryTmp->first();
    est_xme = qryTmp->value(0).toString();
    delete qryTmp;

    if (QFile::exists(templates + "/script/dialogs.js"))
    {
        QFile inFile(templates + "/script/dialogs.js");
        inFile.open(QIODevice::ReadOnly);

        QString jscriptText;
        QTextStream inStream(&inFile);
        while (!inStream.atEnd()) jscriptText.append(inStream.readLine() + "\n");

        jscriptText.replace("<est_aut>", est_aut, Qt::CaseSensitive);
        jscriptText.replace("<est_niv>", est_niv, Qt::CaseSensitive);
        jscriptText.replace("<est_art>", est_art, Qt::CaseSensitive);
        jscriptText.replace("<est_ima>", est_ima, Qt::CaseSensitive);
        jscriptText.replace("<est_son>", est_son, Qt::CaseSensitive);
        jscriptText.replace("<est_vid>", est_vid, Qt::CaseSensitive);
        jscriptText.replace("<est_xme>", est_xme, Qt::CaseSensitive);
        if (QFile::remove(target + "/script/dialogs.js"))
        {
            QFile outFile(target + "/script/dialogs.js");
            outFile.open(QIODevice::ReadWrite);
            QTextStream outStream(&outFile);
            outStream << jscriptText;
        }
    }
    sqlCustomerQuery("DROP TABLE IF EXISTS media");
}

QString GenAlf::generateAreaVMenu(QString text, int padre)
{
    int id;
    padre == -1 ? id = 0: id = padre;

    QString order;
    encyclopedia->getLevelOrder() == 0 ? order = "orden" : order = "nombre";

    qryArea = QSqlQuery(QString("SELECT id_nivel, nombre FROM nivel WHERE padre = %1 "
                                "AND enciclopedia = %2 ORDER BY %3").
                        arg(id).
                        arg(encyclopedia->getIndex()).
                        arg(order));

    qryArea.exec();
    QString emenu;

    swap.clear();
    while (qryArea.next())
    {
        emenu = selectArea(text,"emenu");
        emenu.replace("<enlace_menu>", QString("nivel_%1.htm").arg(qryArea.value(0).toInt()));
        emenu.replace("<texto_menu>", qryArea.value(1).toString());
        swap += emenu;
    }
    ClearTag("emenu", swap);

    if (gallery == true)
    {
        QString strGallery = selectArea(text,"emenu");
        QString link;

        if (id < 0)
        {
            link = "galeria.htm";
            strGallery.remove("datos/", Qt::CaseSensitive);
        }else
            link = "../galeria.htm";

        strGallery.replace("<enlace_menu>", link);
        strGallery.replace("<texto_menu>", "GALER&Iacute;A");
        ClearTag("emenu", strGallery);
        swap.append(strGallery);
    }

    return swap;
}

QString GenAlf::generateAreaHMenu(QString text, int padre)
{
    QString sql("SELECT id_nivel, nombre, padre FROM nivel WHERE "
                "id_nivel = %1 AND enciclopedia = %2 ORDER BY %3");
    QString strOld, strNew, order;

    encyclopedia->getLevelOrder() == 0 ? order = "orden" : order = "nombre";

    qryArea.exec(sql.arg(padre).arg(id_enciclopedia).arg(order));
    qryArea.first();

    if (qryArea.isValid() == true)
    {
        while (padre > 0 )
        {
            qryArea.exec(sql.arg(padre).arg(id_enciclopedia).arg(order));
            qryArea.first();
            strOld = selectArea(text, "niveles_cabecera");
            if (strOld.isEmpty()) break;
            strOld.replace("<enlace_cabecera>", QString("nivel_%1.htm").arg(qryArea.value(0).toInt()));
            strOld.replace("<texto_cabecera>", qryArea.value(1).toString());
            strNew.insert(0, QString("%1 - ").arg(strOld));
            padre = qryArea.value(2).toInt();
        }
    }
    if (strOld.isEmpty())
        strNew = "";
    else{
        strNew.insert(0, "<a href=\"../index.htm\" class=\"menu1\">INICIO</a> - ");
        strNew.truncate(strNew.length()-2);
        ClearTag("niveles_cabecera", strNew);
    }
    return strNew;
}

QString GenAlf::generateAreaLevels(QString text, int padre)
{
    int pos = 0;
    QString summary;
    QString aux = "";
    QString lista_niveles, order;

    encyclopedia->getLevelOrder() == 0 ? order = "orden" : order = "nombre";

    QString condition = QString("padre = %1 AND enciclopedia = %2 ORDER BY %3").
                                arg(padre).
                                arg(id_enciclopedia).
                                arg(order);

    qryArea = QSqlQuery(QString("SELECT id_nivel, nombre, resumen FROM nivel WHERE %1").arg(condition));

    qryArea.exec();

    if (sqlAutoCountRow("nivel", condition) == 1)
    {
        qryArea.first();
        summary = qryArea.value(2).toString();
        ClearTag("em", summary);
        summary = strToPlainText(summary);

        if (summary.length() > 120)
            summary = strsTruncate(summary, 120);

        lista_niveles = selectArea(text, "lista_niveles");
        aux = selectArea(lista_niveles, "parte_derecha");
        aux.replace("<No.2>", "No.1", Qt::CaseSensitive);
        aux.replace("<enlace_nivel2>", QString("nivel_%1.htm").arg(qryArea.value(0).toInt()), Qt::CaseSensitive);
        aux.replace("<titulo_nivel2>", qryArea.value(1).toString(), Qt::CaseSensitive);
        aux.replace("<resumen_nivel2>", summary, Qt::CaseSensitive);
        ClearTag("parte_derecha", aux);
    }else{
        while (qryArea.next() )
        {
            summary = qryArea.value(2).toString();
            ClearTag("em", summary);
            summary = strToPlainText(summary);

            if (summary.length() > 120)
                summary = strsTruncate(summary, 120);

            if (pos % 2 == 0){
                lista_niveles = selectArea(text, "lista_niveles");
                lista_niveles.replace("<No.1>", QString("No.%1").arg(pos + 1), Qt::CaseSensitive);
                lista_niveles.replace("<enlace_nivel1>", QString("nivel_%1.htm").arg(qryArea.value(0).toInt()), Qt::CaseSensitive);
                lista_niveles.replace("<titulo_nivel1>", qryArea.value(1).toString(), Qt::CaseSensitive);
                lista_niveles.replace("<resumen_nivel1>", summary, Qt::CaseSensitive);
            }else{
                lista_niveles.replace("<No.2>", QString("No.%1").arg(pos + 1), Qt::CaseSensitive);
                lista_niveles.replace("<enlace_nivel2>", QString("nivel_%1.htm").arg(qryArea.value(0).toInt()), Qt::CaseSensitive);
                lista_niveles.replace("<titulo_nivel2>", qryArea.value(1).toString(), Qt::CaseSensitive);
                lista_niveles.replace("<resumen_nivel2>", summary, Qt::CaseSensitive);
                ClearTag("parte_derecha", lista_niveles);
                aux += lista_niveles;
            }
            pos++;
        }

        if(qryArea.at() == -2 && pos % 2 != 0) {
            lista_niveles.replace(selectArea(lista_niveles, "parte_derecha"), "", Qt::CaseSensitive);
            aux += lista_niveles;
        }
    }

    ClearTag("lista_niveles", aux);
    return aux;
}

QString GenAlf::generateAreaArticles(QString text, int padre)
{
    int pos = 0;
    QString aux = "";
    QString summary;
    QString lista_articulos, order;

    encyclopedia->getArticleOrder() == 0 ? order = "orden" : order = "nombre";

    QString condition = QString("nivel = %1 AND enciclopedia = %2 ORDER BY %3").
                                arg(padre).
                                arg(id_enciclopedia).
                                arg(order);

    qryArea = QSqlQuery(QString("SELECT articulo.id_articulo, nombre, resumen FROM "
                                "articulo WHERE %1").arg(condition));
    qryArea.exec();

    if (sqlAutoCountRow("articulo", condition) == 1)
    {
        qryArea.first();
        summary = qryArea.value(2).toString();
        ClearTag("em", summary);
        summary = strToPlainText(summary);

        if (summary.length() > 120)
            summary = strsTruncate(summary, 120);

        lista_articulos = selectArea(text, "lista_articulos");
        aux = selectArea(lista_articulos, "parte_derecha");
        aux.replace("<No.2>", "No.1", Qt::CaseSensitive);
        aux.replace("<enlace_articulo2>", QString("articulo_%1.htm").arg(qryArea.value(0).toInt()), Qt::CaseSensitive);
        aux.replace("<titulo_articulo2>", qryArea.value(1).toString(), Qt::CaseSensitive);
        aux.replace("<resumen_articulo2>", summary, Qt::CaseSensitive);
        ClearTag("parte_derecha", aux);
    } else {
        while (qryArea.next())
        {
            summary = qryArea.value(2).toString();
            ClearTag("em", summary);
            summary = strToPlainText(summary);

            if (summary.length() > 120)
                summary = strsTruncate(summary, 120);

            if (pos % 2 == 0){
                lista_articulos = selectArea(text, "lista_articulos");
                lista_articulos.replace("<No.1>",QString("No.%1").arg(pos + 1),Qt::CaseSensitive);
                lista_articulos.replace("<enlace_articulo1>", QString("articulo_%1.htm").arg(qryArea.value(0).toInt()), Qt::CaseSensitive);
                lista_articulos.replace("<titulo_articulo1>", qryArea.value(1).toString(), Qt::CaseSensitive);
                lista_articulos.replace("<resumen_articulo1>", summary, Qt::CaseSensitive);
            }else{
                lista_articulos.replace("<No.2>",QString("No.%1").arg(pos + 1),Qt::CaseSensitive);
                lista_articulos.replace("<enlace_articulo2>", QString("articulo_%1.htm").arg(qryArea.value(0).toInt()), Qt::CaseSensitive);
                lista_articulos.replace("<titulo_articulo2>", qryArea.value(1).toString(), Qt::CaseSensitive);
                lista_articulos.replace("<resumen_articulo2>", summary, Qt::CaseSensitive);
                ClearTag("parte_derecha", lista_articulos);
                aux += lista_articulos;
            }
            pos++;
        }
        if(qryArea.at() == -2 && pos % 2 != 0) {
            lista_articulos.replace(selectArea(lista_articulos, "parte_derecha"), "", Qt::CaseSensitive);
            aux += lista_articulos;
        }
        ClearTag("lista_articulos", aux);
    }

    return aux;
}

QString GenAlf::generateAreaRelationsAuthors(int id_articulo)
{
    qryArea = QSqlQuery(QString("SELECT autor.nombre FROM autor INNER JOIN articuloautores "
                                "ON autor.id_autor = articuloautores.autor WHERE "
                                "articuloautores.articulo = %1").arg(id_articulo));

        swap.clear();
        while (qryArea.next()){
            swap.append(QString("%1<br>").arg(qryArea.value(0).toString()));
        }
        swap.truncate(swap.length()-4);
        return swap;
}

QString GenAlf::generateAreaRelationsArticles(QString text, int id_articulo){
    swap.clear();
    qryArea.exec(QString("SELECT articulo.id_articulo, articulo.nombre "
                         "FROM articulo INNER JOIN articuloarticulos  ON "
                         "articulo.id_articulo = articuloarticulos.IdArticuloLink "
                         "WHERE articuloarticulos.articulo = %1").arg(id_articulo));
    while (qryArea.next()){
        relation = selectArea(text, "lista_articulos_relacionados");
        relation.replace("<enlace_relacionado>", QString("articulo_%1.htm").
                         arg(qryArea.value(0).toString(), Qt::CaseSensitive));
        relation.replace("<titulo_relacionado>", qryArea.value(1).toString(), Qt::CaseSensitive);
        swap += (relation);
    }
    ClearTag("lista_articulos_relacionados", swap);
    return swap;
}

QString GenAlf::generateAreaSource(QString text){
    swap.clear();
    swap = QString ("<b>Tomado de:</b>&nbsp;%1");

    if (!text.isEmpty())
        return swap.arg(text);
    else return "";
}

QString GenAlf::generateAreaImage(int id){
    swap.clear();
    swap = QString("<img alt=\"%1\" src=\"../medias/%2\" title=\"%3\" style=\"margin-right: 10px;"
                   " float: left; width: 100px; height: 100px; \" />");

    qryArea.exec(QString("SELECT id_adjunto||'.'||extension, descripcion "
                         "FROM adjuntos WHERE id_adjunto = %1").arg(id));
    qryArea.first();

    if (qryArea.isValid() && qryArea.value(0).toString() != "0.gif")
        return swap.
                arg(qryArea.value(0).toString()).
                arg(qryArea.value(0).toString()).
                arg(qryArea.value(1).toString());
    else return "";
}

QString GenAlf::generateSlideShow(int id_pagina, char tipo)
{
    QFile htmlResFile(":/html/slide.html");
    htmlResFile.open(QIODevice::ReadOnly);

    QString htmlCode = "",
            strLinks,
            strItems,
            swap;

    int id_slide = sqlGetIdValue("slide", "id_slide",
                                 QString(" id_pagina = %1 AND tipo = '%2' ").
                                         arg(id_pagina).
                                         arg(tipo));

    if (id_slide > 0)
    {
        QTextStream htmlStream(&htmlResFile);
        while (!htmlStream.atEnd()) htmlCode.append(QString("%1\n").arg(htmlStream.readLine()));

        QSqlQuery *qryImage = new QSqlQuery;
        qryImage->exec(SQL_Candidates.arg(id_slide));

        int row = 0;

        while (qryImage->next())
        {
            swap = selectArea(htmlCode, "image-group");
            if (row == 0){
                swap.replace("<item>", "item active", Qt::CaseSensitive);
                strLinks.append(QString("<li data-target=\"#slide\" data-slide-to=\"0\" class=\"active\"></li>"));
            }else{
                swap.replace("<item>", "item", Qt::CaseSensitive);
                strLinks.append(QString("<li data-target=\"#slide\" data-slide-to=\"%1\"></li>").arg(row));
            }

            swap.replace("<file>", qryImage->value(2).toString(), Qt::CaseSensitive);
            swap.replace("<alt-text>", qryImage->value(0).toString(), Qt::CaseSensitive);
            swap.replace("<text>", qryImage->value(0).toString(), Qt::CaseSensitive);

            if (tipo == 'i')
                swap.replace("<path>", "medias", Qt::CaseSensitive);
            else
                swap.replace("<path>", "../medias", Qt::CaseSensitive);

            strItems.append(swap+"\n");
            row++;
        }

        htmlCode.replace("<indicator>", strLinks, Qt::CaseSensitive);
        htmlCode.replace(selectArea(htmlCode,"image-group"), strItems, Qt::CaseSensitive);

        if (tipo == 'i')
            htmlCode.replace("<path>", "", Qt::CaseSensitive);
        else
            htmlCode.replace("<path>", "../", Qt::CaseSensitive);

        ClearTag("image-group", htmlCode);
    }

    return selectArea(htmlCode, "slideshow");
}

void GenAlf::ClearTag(QString tag, QString &text)
{
    text.replace("<" + tag + ">", "", Qt::CaseSensitive);
    text.replace("</" +tag + ">", "", Qt::CaseSensitive);
}

void GenAlf::GetTemplateType(QString str)
{
    QWebPage web;
    web.mainFrame()->setHtml(str);

    QWebElement body;
    body = web.mainFrame()->findFirstElement("body");
    typeT = body.attribute("type").toInt();
}

void GenAlf::PrepareExtensionFiles()
{
    sqlCustomerQuery("CREATE TABLE IF NOT EXISTS media(nombre "
                     "integer(10) NOT NULL, extension varchar(5) NOT NULL);");
    sqlCustomerQuery("DELETE FROM media");

    extImagen = encyclopedia->getExtImagen();
    extSonido = encyclopedia->getExtSonido();
    extVideo = encyclopedia->getExtVideo();
    extXMedia = encyclopedia->getExtXMedia();

    extImagen.replace("*.","extension = '");
    extImagen.replace(";","' OR ");
    extSonido.replace("*.","extension = '");
    extSonido.replace(";","' OR ");
    extVideo.replace("*.","extension = '");
    extVideo.replace(";","' OR ");
    extXMedia.replace("*.","extension = '");
    extXMedia.replace(";","' OR ");
}

void GenAlf::SelectMediaInText(QString text)
{
    text = text.simplified();

    QWebPage page;
    page.mainFrame()->setHtml(text);

    QWebElementCollection imgsList = page.mainFrame()->findAllElements("img");

    QString imgSrc, imgName;

    foreach(QWebElement img, imgsList) {
        imgSrc = img.attribute("alt");
        imgName = imgSrc.section("/", -1);

        QString str0 = imgName.section(".", -1);
        QString str1 = imgName.remove(QString(".%1").arg(str0));

        sqlInsertRecord_N("media", "nombre, extension", QString("%1, <&~%2~&>").arg(str1).arg(str0));
    }
}

void GenAlf::PrepareInsertMedias()
{
    QString sql0 = "SELECT adjuntos.id_adjunto, adjuntos.extension FROM adjuntos "
                   "INNER JOIN autor ON autor.foto = adjuntos.id_adjunto "
                   "INNER JOIN enc_autor ON enc_autor.autor = autor.id_autor "
                   "WHERE adjuntos.id_adjunto != 0 AND enc_autor.equipo != 0 ";

    QString sql1 = "SELECT adjuntos.id_adjunto, adjuntos.extension FROM adjuntos "
                   "INNER JOIN autor ON autor.foto = adjuntos.id_adjunto "
                   "INNER JOIN enc_autor ON enc_autor.autor = autor.id_autor "
                   "WHERE adjuntos.id_adjunto != 0 AND enc_autor.colaborador != 0 ";


    QString sql2 = "SELECT DISTINCT autor.foto, adjuntos.extension FROM autor "
                   "INNER JOIN adjuntos ON autor.foto = adjuntos.id_adjunto "
                   "INNER JOIN articuloautores ON articuloautores.autor = autor.id_autor "
                   "WHERE adjuntos.id_adjunto !=0";

    QString sql4 = "SELECT DISTINCT adjuntos.id_adjunto, adjuntos.extension FROM adjuntos "
                   "INNER JOIN adjuntos_slide ON adjuntos_slide.id_adjunto = adjuntos.id_adjunto "
                   "WHERE adjuntos.id_adjunto != 0";

    for (int i = 0; i < 5; i++){
        switch (i){
        case 0: InsertMedias(sql0);
            break;
        case 1: InsertMedias(sql1);
            break;
        case 2: InsertMedias(sql2);
            break;
        case 4: InsertMedias(sql4);
            break;
        }
    }
}

void GenAlf::InsertMedias(QString sql)
{
    QSqlQuery *qry = new QSqlQuery;
    qry->exec(sql);

    while (qry->next()){
        sqlInsertRecord_N("media",
                        "nombre, extension",
                        QString("%1, <&~%2~&>").
                               arg(qry->value(0).toInt()).
                               arg(qry->value(1).toString()));
    }
    delete qry;
}




