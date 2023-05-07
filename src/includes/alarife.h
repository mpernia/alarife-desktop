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

#ifndef ALARIFE_H
#define ALARIFE_H

#include <QtGlobal>

#include <QString>
#include <QMainWindow>
#include <QFileInfo>
#include <QMessageBox>

#ifdef Q_OS_LINUX
    #define __SYS_DIR__  QDir::rootPath() + "usr/bin/"
    #define __TMP_DIR__  QDir::tempPath()
    #define __BTN_SIZE__ 100
    #define __EXT__ ""
    #define __WIN_FLAG__ Qt::Dialog | Qt::WindowCloseButtonHint
    #define __HTTP__ "file://"
#elif defined(Q_OS_WIN32)
    #define __SYS_DIR__  QDir::currentPath() + "/"
    #define __TMP_DIR__  QDir::homePath()
    #define __BTN_SIZE__ 200
    #define __EXT__ ".exe"
    #define __WIN_FLAG__ Qt::Dialog | Qt::WindowTitleHint
    #define __HTTP__ ""
#endif

#define __APP_NAME__      "Alarife-4"
#define __APP_VERSION__   "23.03.2019"
#define __APP_PATH__      QCoreApplication::applicationDirPath()
#define __DIR_FILES__     QDir::homePath()
#define __DIR_CACHE__     QDir::homePath() + "/Alarife/.cache"
#define __DIR_TEMPLATE__  "/templates/"
#define __DB_VERSION__    "2.0.2"

static const QSize resultSize(200, 200);
//----------------------------------------------------------------------------------------------

class Book{
    public:
        Book(){index = 0; user = -1;}
        void setTitle(QString str){title = str;}
        void setTemplate(QString str){templates = str;}
        void setExtImagen(QString str){ext_image = str;}
        void setExtSonido(QString str){ext_sound = str;}
        void setExtVideo(QString str){ext_video = str;}
        void setExtXMedia(QString str){ext_xmedia = str;}
        void setStyle(QString str){style = str;}
        void setStyleSheet(QString str){stylesheet = str;}
        void setMediaPath(QString str){path_image = str;}
        void setIndex(int num){index = num;}
        void setUserIndex(int num){user = num;}
        void setVideoMode(int mode){video = mode;}
        void setAudioMode(int mode){audio = mode;}
        void setArticleOrder(int order) {article = order;}
        void setLevelOrder(int order) {level = order;}
        void setActiveUsers(int usr) {active = usr;}
        QString getTitle(){return title;}
        QString getTemplate(){return templates;}
        QString getExtImagen(){return quote(ext_image);}
        QString getExtSonido(){return quote(ext_sound);}
        QString getExtVideo(){return quote(ext_video);}
        QString getExtXMedia(){return quote(ext_xmedia);}
        QString getStyle(){return style;}
        QString getStyleSheet(){return stylesheet;}
        QString getMediaPath(){return path_image;}
        int getIndex(){return index;}
        int getUserIndex(){return user;}
        int getVideoMode(){return video;}
        int getAudioMode(){return audio;}
        int getArticleOrder() {return article;}
        int getLevelOrder() {return level;}
        int activeUsers() {return active;}
    private:
        QString quote(QString str){ return str.replace("'", "\''", Qt::CaseSensitive); }
        QString title,
                templates,
                ext_image,
                ext_sound,
                ext_video,
                ext_xmedia,
                style,
                stylesheet,
                path_image;
        int user,
            index,
            video,
            audio,
            article,
            level,
            active;

};




//----------------------------------------------------------------------------------------------



#endif
