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

#include <fstream>
#include <iostream>
#include <QtGui>
#include <QObject>
#include <QtSql>
#include <QFileInfo>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "../includes/database.h"
using namespace std;

DbConnect::DbConnect()
{
    connected = false;
    cCount = 0;
}

DbConnect::~DbConnect()
{
    connected = false;
    DbPathFile.clear();
    dbName.close();
}

bool DbConnect::Connected()
{
    return connected;
}

QString DbConnect::getPathFile()
{
    return DbPathFile;
}

QString DbConnect::getFileName()
{
    return QFileInfo(DbPathFile).baseName();
}

QString DbConnect::getDirPath()
{
    return QFileInfo(DbPathFile).absolutePath();
}

void DbConnect::ConnectionString(QString DbPath)
{
    dbName = QSqlDatabase::addDatabase("QSQLITE");
    DbPathFile = DbPath;
    dbName.setDatabaseName(DbPath);
 }

void DbConnect::Active(bool mode)
{
    if (mode == true)
    {
        if (!dbName.open())
        {
            QMessageBox::critical(0, qApp->tr("Error de conexión"),
                                  qApp->tr("Imposible establecer la conexión con "
                                           "Base de datos\n"
                                           "SUGERENCIA\n verifique que existe el archivo\n"
                                           "verifique que existen las librerías para manipular "
                                           "la base de datos .\n\nHaga click en el boton Cancelar."
                                           "\ndirección:\n")+getPathFile(), QMessageBox::Cancel);
            connected = false;
        }else{
            QSqlQuery *qrydb = new QSqlQuery;
            qrydb->exec("PRAGMA foreign_keys = ON;");
            delete qrydb;
            connected = true;
        }
    }else {
       dbName.close();
       dbName.removeDatabase(getFileName());
       dbName = QSqlDatabase();
       DbPathFile.clear();
       connected = false;
   }
}

void DbConnect::CreateDb()
{
    QSqlQuery *qrydb = new QSqlQuery;
    qrydb->exec("CREATE TABLE tokens (id_token integer(10) primary key, nombre varchar(255) NULL);");
    qrydb->exec("CREATE TABLE version (id_version integer(10) primary key, valor varchar(20) NULL);");
    qrydb->exec("CREATE TABLE claves (nombre varchar(250) NULL);");
    qrydb->exec("CREATE TABLE tiposusuarios (id_tipo_usuario integer(10) primary key, descripcion varchar(50) NULL);");
    qrydb->exec("CREATE TABLE usuario (id_usuario integer(10) primary key, nombre varchar(250) NULL, usuario varchar(50) NULL, clave varchar(32) NULL, tipousuario  integer(10) NOT NULL, FOREIGN KEY (tipousuario) REFERENCES tiposusuarios(id_tipo_usuario) ON DELETE CASCADE ON UPDATE CASCADE);");
    qrydb->exec("CREATE TABLE enciclopedia (id_enciclopedia integer(10) primary key, nombre varchar(250) NULL, resumen text NULL, plantilla varchar(255), fecha_actualizacion datetime NOT NULL default '0000-00-00 00:00:00', ordenar_articulo integer(10) NOT NULL default 1, ordenar_nivel integer(10) NOT NULL default 0, audio_embebido integer(10) NOT NULL default 0, video_embebido integer(10) NOT NULL default 0);");
    qrydb->exec("CREATE TABLE enc_usuario (enciclopedia integer(10) NOT NULL, usuario integer(10) NOT NULL, tipousuario integer(10) NULL, FOREIGN KEY (enciclopedia) REFERENCES enciclopedia(id_enciclopedia) ON DELETE CASCADE ON UPDATE CASCADE, FOREIGN KEY (usuario) REFERENCES usuario(id_usuario) ON DELETE CASCADE ON UPDATE CASCADE);");
    qrydb->exec("CREATE TABLE enc_autor (enciclopedia integer(10) NOT NULL, autor integer(10) NOT NULL, colaborador integer(10) NULL, equipo integer(10) NULL, FOREIGN KEY (enciclopedia) REFERENCES enciclopedia(id_enciclopedia) ON DELETE CASCADE ON UPDATE CASCADE, FOREIGN KEY (autor) REFERENCES autor(id_autor) ON DELETE CASCADE ON UPDATE CASCADE);");
    qrydb->exec("CREATE TABLE enc_tipo_ext (enciclopedia  integer(10) NULL, archivo varchar(50) NULL, extension varchar(255) NULL);");
    qrydb->exec("CREATE TABLE nivel (id_nivel integer(10) primary key, nombre varchar(250) NULL, resumen text NULL, padre integer(10) NULL, usuario integer(10) NOT NULL, enciclopedia integer(10) NOT NULL, orden integer(10) NULL, FOREIGN KEY (usuario) REFERENCES usuario(id_usuario) ON DELETE CASCADE ON UPDATE CASCADE, FOREIGN KEY (enciclopedia) REFERENCES enciclopedia(id_enciclopedia) ON DELETE CASCADE ON UPDATE CASCADE);");
    qrydb->exec("CREATE TABLE adjuntos (id_adjunto integer(10) primary key, descripcion varchar(250) NULL, extension varchar(5) NULL, tipo char NULL);");
    qrydb->exec("CREATE TABLE autor (id_autor integer(10) primary key, nombre varchar(250) NULL, titulo varchar(200) NULL, cargo varchar(200) NULL, correo varchar(250) NULL, foto integer(10) NOT NULL, FOREIGN KEY (foto) REFERENCES adjuntos(id_adjunto) ON DELETE CASCADE ON UPDATE CASCADE);");
    qrydb->exec("CREATE TABLE articulo(id_articulo integer(10) primary key, nombre varchar(250) NULL, resumen text NULL, contenido text NULL, claves text NULL, fuente varchar(250) NULL, fecha1 date NOT NULL default '0000-00-00', fecha2 date NOT NULL default '0000-00-00', usuario integer(10) NULL, nivel integer(10) NULL, orden integer(10) NOT NULL default 1, enciclopedia integer(10) NULL);");
    qrydb->exec("CREATE TABLE articuloarticulos (articulo integer NOT NULL, IdArticuloLink  integer(10) NOT NULL, FOREIGN KEY (IdArticuloLink) REFERENCES articulo(id_articulo) ON DELETE CASCADE ON UPDATE CASCADE, FOREIGN KEY (articulo) REFERENCES articulo(id_articulo) ON DELETE CASCADE ON UPDATE CASCADE);");
    qrydb->exec("CREATE TABLE articuloautores (articulo integer(10) NOT NULL, autor integer(10) NOT NULL, FOREIGN KEY (autor) REFERENCES autor(id_autor) ON DELETE CASCADE ON UPDATE CASCADE, FOREIGN KEY (articulo) REFERENCES articulo(id_articulo) ON DELETE CASCADE ON UPDATE CASCADE);");
    qrydb->exec("CREATE TABLE slide (id_slide integer(10) PRIMARY KEY, id_pagina integer(10), tipo char(1));");
    qrydb->exec("CREATE TABLE adjuntos_slide (id_slide integer(10) NOT NULL, id_adjunto integer(10), orden integer(10) NOT NULL default 1, FOREIGN KEY(id_slide) REFERENCES slide(id_slide) ON DELETE CASCADE ON UPDATE CASCADE);");
    qrydb->exec("CREATE TABLE referencias (id_referencia integer(10) NOT NULL, articulo integer(10) NULL, enlace integer(10) NULL, nombre varchar(250) NULL, resumen text NULL, texto varchar(255) NOT NULL, FOREIGN KEY (articulo) REFERENCES articulo(id_articulo) ON DELETE CASCADE ON UPDATE CASCADE);");
    qrydb->exec("CREATE TABLE galeria (id_galeria integer(10) NOT NULL, id_adjunto integer(10) NOT NULL, id_enciclopedia integer(10) NOT NULL);");
    qrydb->exec("INSERT INTO adjuntos VALUES (0,'SIN IMAGEN', 'gif','I');");
    qrydb->exec("INSERT INTO tiposusuarios VALUES (1,'Editor Jefe');");
    qrydb->exec("INSERT INTO tiposusuarios VALUES (2,'Editor');");
    qrydb->exec("INSERT INTO version VALUES (1,'" __DB_VERSION__ "');");
    delete qrydb;
}

int DbConnect::getUserNum()
{
    return 0;
}

bool DbConnect::ChechkCompatibility()
{
    QStringList tables = getTablesName();
    if (tables.contains("version"))
    {
        QSqlQuery *qrydb = new QSqlQuery;
        qrydb->exec("SELECT valor FROM version");
        qrydb->first();
        QString version = qrydb->value(0).toString();
        delete qrydb;
        if (version == __DB_VERSION__)
            return true;
        else
            return false;
    }else
        return false;
}

QStringList DbConnect::getTablesName()
{
    QStringList list;
    QSqlQuery *qrydb = new QSqlQuery;
    qrydb->exec("SELECT tbl_name FROM sqlite_master");
    qrydb->first();
    list.append(qrydb->value(0).toString());
    while (qrydb->next()) list.append(qrydb->value(0).toString());
    delete qrydb;
    return list;
}
