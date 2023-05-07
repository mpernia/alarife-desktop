/****************************************************************************
**
** Copyleft: 2018. Maikel Enrique Pernia Matos.
** Contact: perniamatos@infomed.sld.cu
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

#ifndef SPELLER_H
#define SPELLER_H


#include <QWidget>
#include <QObject>

#include "aspell.h"

class Speller : public QObject
{
    Q_OBJECT
public:
    explicit Speller(QObject *parent = 0, char *lang = 0);
    void add_word(QString);
    void ignore_word(QString word);
    int check_word(QString word);
    QStringList get_suggestions_lists(QString word);

protected:
    void check_for_error(AspellSpeller *spl);
    void check_for_config_error(AspellConfig *cnf);

signals:

public slots:

private:
    bool check_is_word(QString);

    AspellCanHaveError *ret;
    AspellSpeller * speller;
    int have;
    char * word_end;
    AspellConfig * config;
    QWidget widget;
};

#endif // SPELLER_H
