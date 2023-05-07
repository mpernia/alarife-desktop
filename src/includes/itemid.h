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

#ifndef ITEMID_H
#define ITEMID_H
#include <QString>


class ItemId
{
private:
    QString encPath;
    int enciclopedia;
    int user;
    int item;
    int father;
    int mode;
    bool owner;
public:
    ItemId();
    enum {INSERT, EDIT, READONLY};
    void setPath(QString p = ""){encPath = p;}
    void setEnciclopedia(int a = 0){enciclopedia = a;}
    void setUser(int a = 0){user = a;}
    void setItem(int a = 0){item = a;}
    void setFather(int a = 0){father = a;}
    void setMode(int a = 0){mode = a;}
    void setOwner(bool own = false) { owner = own; }
    QString getPath(){return encPath;}
    int getEnciclopedia(){return enciclopedia;}
    int getUser(){return user;}
    int getItem(){return item;}
    int getFather(){return father;}
    int getMode(){return mode;}
    bool isOwner() { return owner; }
};

#endif // ITEMID_H
