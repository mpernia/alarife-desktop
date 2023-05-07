# ##########################################################################
# ##
# ## Copyleft: 2012 - 2016. MultySystem. Archivo Historico de Manzanillo
# ## Contact:  Herlin Reinier Matos Lastres <hmatos@uci.grm.cu>
# ##           Maikel Enrique Pernía Matos <corba@grannet.grm.sld.cu>
# ##
# ## This file is part of the Alarife-4 application.
# ##
# ## GNU General Public License Usage
# ##
# ## This file may be used under the terms of the GNU General Public License;
# ## either version 2 of the License, or (at your option) any later version,
# ## as published by the Free Software Foundation and appearing in the file
# ## LICENSE included in the packaging of this file.  Please review the
# ## following information to ensure the GNU General Public License version 2
# ## requirements will be met: http://www.gnu.org/copyleft/gpl.html.
# ##########################################################################


QT       += core gui sql webkit xml



win32:LIBS += -L C:\\Qt\\libs  -laspell-15
unix:LIBS  += -laspell

win32:DESTDIR = ../win32/bin
unix:DESTDIR = ../

TARGET = alarife
TEMPLATE = app

CONFIG += qt warn_on

SOURCES += sources/main.cpp \
           sources/mainwindow.cpp \
           sources/htmlmanager.cpp \
           sources/webview.cpp \
           sources/itemid.cpp \
           sources/editor.cpp \
           sources/mineditor.cpp \
           sources/speller.cpp \
           sources/spellerdlg.cpp \
           sources/about.cpp \
           sources/authors.cpp \
           sources/chapter.cpp \
           sources/config.cpp \
           sources/database.cpp \
           sources/diagram.cpp \
           sources/dinarray.cpp \
           sources/export.cpp \
           sources/gridimage.cpp \
           sources/media.cpp \
           sources/newbook.cpp \
           sources/orders.cpp \
           sources/property.cpp \
           sources/import.cpp \
           sources/loadimagedlg.cpp \
           sources/login.cpp \
           sources/stadistic.cpp \
           sources/folder.cpp \
           sources/html.cpp \
           sources/htmlarticles.cpp \
           sources/htmldefault.cpp \
           sources/htmlindex.cpp \
           sources/htmllevels.cpp \
           sources/jscript.cpp \
           sources/stackid.cpp \
           sources/editauthordlg.cpp \
           sources/selectauthordlg.cpp \
           sources/genalf.cpp \
           sources/templatedlg.cpp \
           sources/templateview.cpp \
           sources/imageitem.cpp \
           sources/editusers.cpp \
           sources/licensedlg.cpp \
           sources/creditsdlg.cpp \
           sources/insertmediadlg.cpp \
           sources/movechapterdlg.cpp \
           sources/insertimage.cpp \
           sources/movearticledlg.cpp \
           #sources/compress.cpp \
           sources/sqlfunctions.cpp \
           sources/strfunction.cpp \
           sources/homepage.cpp \
           sources/dlgwaitproc.cpp \
           sources/tableview.cpp \
           sources/itemproperty.cpp \
           sources/wdgmedia.cpp \
           sources/slide.cpp \
           sources/reference.cpp \
           sources/moveitem.cpp \
           sources/baseditor.cpp \
           sources/gallery.cpp \
           sources/gallerydlg.cpp \
           sources/dlgadminuser.cpp \
           sources/dlgadmintemplates.cpp \
           sources/dlgadminstyles.cpp

HEADERS += includes/mainwindow.h \
           includes/htmlmanager.h \
           includes/webview.h \
           includes/itemid.h \
           includes/editor.h \
           includes/mineditor.h \
           includes/speller.h \
           includes/spellerdlg.h \
           includes/about.h \
           includes/alarife.h \
           includes/authors.h \
           includes/chapter.h \
           includes/config.h \
           includes/database.h \
           includes/diagram.h \
           includes/dinarray.h \
           includes/export.h \
           includes/gridimage.h \
           includes/media.h \
           includes/newbook.h \
           includes/orders.h \
           includes/property.h \
           includes/import.h \
           includes/loadimagedlg.h \
           includes/login.h \
           includes/stadistic.h \
           includes/folder.h \
           includes/html.h \
           includes/htmlarticles.h \
           includes/htmldefault.h \
           includes/htmlindex.h \
           includes/htmllevels.h \
           includes/jscript.h \
           includes/stackid.h \
           includes/editauthordlg.h \
           includes/selectauthordlg.h \
           includes/genalf.h \
           includes/templatedlg.h \
           includes/templateview.h \
           includes/imageitem.h \
           includes/editusers.h \
           includes/licensedlg.h \
           includes/creditsdlg.h \
           includes/insertmediadlg.h \
           includes/movechapterdlg.h \
           includes/insertimage.h \
           includes/movearticledlg.h \
           includes/sqlfunctions.h \
           includes/strfunction.h \
           includes/homepage.h \
           includes/dlgwaitproc.h \
           includes/tableview.h \
           includes/itemproperty.h \
           includes/wdgmedia.h \
           includes/slide.h \
           includes/reference.h \
           includes/moveitem.h \
           includes/baseditor.h \
           includes/gallery.h \
           includes/gallerydlg.h \
           includes/dlgadminuser.h \
           includes/dlgadmintemplates.h \
           includes/dlgadminstyles.h

RESOURCES += resources/alarife.qrc\
             resources/qjseditor.qrc

win32:RC_FILE = resources/description.rc
