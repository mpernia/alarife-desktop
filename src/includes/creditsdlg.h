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

#ifndef CREDITSDLG_H
#define CREDITSDLG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QTabWidget;
class QTextEdit;
QT_END_NAMESPACE

//----------------------------------------------------------------------------------------------
class CreditsDlg : public QDialog{
Q_OBJECT
public:
    explicit CreditsDlg(QWidget *parent = 0);
signals:

public slots:

private:
        QLabel *lblCaption;
        QPushButton *btnClose;
        QTabWidget *tabPage;
};

//----------------------------------------------------------------------------------------------
class CreditsTabPage : public QWidget{
    Q_OBJECT
    public:
        CreditsTabPage(QWidget *parent = 0);
        void setTabText(QString text="");
    private:
        QTextEdit *mmText;

};
//----------------------------------------------------------------------------------------------
class ProgrammerTab : public CreditsTabPage{
    Q_OBJECT
public:
    ProgrammerTab(CreditsTabPage *parent = 0);
};
//----------------------------------------------------------------------------------------------
class TraslaitorTab : public CreditsTabPage{
    Q_OBJECT
public:
    TraslaitorTab(CreditsTabPage *parent = 0);
};
//----------------------------------------------------------------------------------------------
class DocumentationTab : public CreditsTabPage{
    Q_OBJECT
public:
    DocumentationTab(CreditsTabPage *parent = 0);
};

#endif // CREDITSDLG_H
