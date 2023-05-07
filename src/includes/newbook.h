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

#ifndef NEWBOOK_H
#define NEWBOOK_H

#include <QWizard>

#include "database.h"
#include "alarife.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QTextEdit;
class QPushButton;
class QImage;
QT_END_NAMESPACE

//----------------------------------------------------------------------------------------------
class NewBook : public QWizard{
    Q_OBJECT
public:
    NewBook(QWidget *parent = 0, DbConnect ** = 0, Book **encyc = 0);
    void accept();
    void createAllDir(QString&/*, QString*/);
protected:

signals:
    void closeDlg(bool);
private:
    bool isNewBook;
    DbConnect *AlarifeDb2;
    Book *encyclopedia;
};
//----------------------------------------------------------------------------------------------
class IntroductionPage : public QWizardPage{
    Q_OBJECT
public:
    IntroductionPage(QWidget *parent = 0);
private:
    QLabel *lblInfo,
           *lblTitle,
           *lblDbName;
    QLineEdit *edtTitle,
              *edtDbName;
};
//----------------------------------------------------------------------------------------------
class AdministratorPage : public QWizardPage{
    Q_OBJECT
public:
    AdministratorPage(QWidget *parent = 0);
private:
    QLineEdit *edtName,
              *edtLogin,
              *edtPass;
    QLabel *lblText,
           *lblPass,
           *lblLogin;
};
//----------------------------------------------------------------------------------------------
class XMediaPage : public QWizardPage{
    Q_OBJECT
public:
    XMediaPage(QWidget *parent = 0);
private:
    QLabel *lblMedia,
           *lblImage,
           *lblSound,
           *lblVideo,
           *lblOther;
    QLineEdit *edtImage,
              *edtSound,
              *edtVideo,
              *edtOther;
};
//----------------------------------------------------------------------------------------------
class ConclusionPage : public QWizardPage{
    Q_OBJECT
public:
    ConclusionPage(QWidget *parent = 0);
protected:
    void initializePage();
private:
    QLabel *label;
};
//----------------------------------------------------------------------------------------------
#endif
