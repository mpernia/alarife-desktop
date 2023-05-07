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

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QSplashScreen>
#include <QTextCodec>
#include <stdlib.h>
#include <iostream>
#include <QFile>

#include "../includes/mainwindow.h"

using namespace std;


int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(true);
    Q_INIT_RESOURCE(alarife);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QPixmap pixmap(":/images/splash.png");
    QSplashScreen splash(pixmap);

    splash.show();

    QString translatorFileName("qt_"),
            translatorPath;

    QString path = __APP_PATH__;
    if (path.contains("/bin")) path.remove("/bin");

    #ifndef Q_OS_UNIX
        QCoreApplication::addLibraryPath(path + "/plugins/");
        translatorFileName += "es";
        translatorPath = path + "/plugins/translations/";
    #else
        translatorFileName += QLocale::system().name();
        translatorPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
    #endif

    QTranslator *translator = new QTranslator(&app);

    if (translator->load(translatorFileName, translatorPath))
        app.installTranslator(translator);

    MainWindow mainwindow;
    mainwindow.setWindowIcon(QPixmap(":/images/Alarife.png"));

    if (argc == 2 && QFile::exists(argv[1]))
    {
        mainwindow.setLoatFile(argv[1]);
    }

    mainwindow.show();

    splash.finish(&mainwindow);
    return app.exec();
}

