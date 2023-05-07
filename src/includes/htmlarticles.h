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

#ifndef HTMLARTICLES_H
#define HTMLARTICLES_H

#include "htmlindex.h"

class htmlArticles : public htmlIndex
{
public:
    htmlArticles(int index, int mode) : htmlIndex(index, mode){}
    virtual ~htmlArticles(){}
    void setRelationsAuthors(QString);
    void setContent(QString);
    void setRelationsArticles(QString);
    void setSourceData(QString);
    void setDates(QString, QString);
    void clear();
    void changeValueElements();
    void insertReferences(int, int, QString, QString, QString);
private:    
    QString strAuthors,
            strContent,
            strArticles,
            strSource,
            strDateCreated,
            strDateModified,
            strReference;
};

#endif // HTMLARTICLES_H
