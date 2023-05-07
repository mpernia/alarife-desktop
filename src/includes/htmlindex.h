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

#ifndef HTMLINDEX_H
#define HTMLINDEX_H

#include "html.h"

class htmlIndex : public html
{
public:
    htmlIndex(int index, int mode) : html(), id_enciclopedia(index), mode(mode){}
    virtual ~htmlIndex(){}

    void setPrimaryKey(int);
    void setFatherValue(int);
    void setTitle(QString);
    void setSummary(QString);
    void setLeveList(QString);
    void setArticleList(QString);
    void setVMenuArea(QString);
    void setHMenuArea(QString);
    void setSlideShow(QString);
    void clear();
    void changeValueElements();
protected:
    void changeMediaLink(QString &);
    void replaceArea(QString, QString);
    QString strVmenu,
            strHmenu,
            strTitle,
            strSummary,
            strLeveList,
            strArticleList,
            strSlide;
    int id_enciclopedia,
        id_padre,
        id_primary,
        mode;

    QStack <QString> trace;

};

#endif // HTMLINDEX_H
