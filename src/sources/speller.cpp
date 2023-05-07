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

#include <QDebug>
#include <QMessageBox>
#include <string>

#include "../includes/speller.h"

Speller::Speller(QObject *parent, char *lang) : QObject(parent)
{
    config = new_aspell_config();
    aspell_config_replace(config, "lang", lang);
    ret = new_aspell_speller(config);
    delete_aspell_config(config);
    speller = to_aspell_speller(ret);
    config = aspell_speller_config(speller);
}

int Speller::check_word(QString word)
{
    if (word.length() > 3 && check_is_word(word.toStdString().c_str()))
    {
        return aspell_speller_check(speller, word.toStdString().c_str(), -1);
    } else
        return 1;
}

void Speller::add_word(QString word)
{
    if (word.length() > 3)
    {
        aspell_speller_add_to_personal(speller, word.toStdString().c_str(), -1);
        check_for_error(speller);
    }
}

void Speller::ignore_word(QString word)
{
    aspell_speller_add_to_session(speller, word.toStdString().c_str(), -1);
    check_for_error(speller);
}

QStringList Speller::get_suggestions_lists(QString word)
{
    QStringList list;
    const AspellWordList *wordlist = aspell_speller_suggest(speller, word.toStdString().c_str(), -1);

    if (wordlist == 0)
    {
        QMessageBox::information(&widget,
                                 tr("Correción ortográfico"),
                                 tr("%1").arg(aspell_speller_error_message(speller)));
        qDebug() << "Error: " << aspell_speller_error_message(speller) << endl;
    }else{
        AspellStringEnumeration * enumeration = aspell_word_list_elements(wordlist);
        const char *suggestion;
        while ( (suggestion = aspell_string_enumeration_next(enumeration)) != 0)
        {
            #ifdef Q_OS_LINUX
                list.append(tr(suggestion));
            #elif defined(Q_OS_WIN32)
                list.append(QString::fromAscii(suggestion));
            #endif
        }
        delete_aspell_string_enumeration(enumeration);
    }
    return list;
}

void Speller::check_for_error(AspellSpeller *spl)
{
    if (aspell_speller_error(spl) != 0)
    {

        QMessageBox::information(&widget,
                                 tr("Correción ortográfico"),
                                 tr("%1").arg(aspell_speller_error_message(spl)));
        qDebug() << "Error: " << aspell_speller_error_message(spl) << endl;
    }
}

void Speller::check_for_config_error(AspellConfig *cnf)
{
    if (aspell_config_error(cnf) != 0)
    {
        QMessageBox::information(&widget,
                                 tr("Correción ortográfico"),
                                 tr("%1").arg(aspell_config_error_message(cnf)));
        qDebug() << "Error: " << aspell_config_error_message(cnf) << endl;
    }
}

bool Speller::check_is_word(QString str)
{
    QRegExp regexp("[a-zA-Z]");

    return str.contains(regexp);
}

