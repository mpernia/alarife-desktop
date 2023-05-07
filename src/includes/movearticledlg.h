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

#ifndef MOVEARTICLEDLG_H
#define MOVEARTICLEDLG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QComboBox;
class QWidget;
class QSqlQuery;
QT_END_NAMESPACE

class MoveArticleDlg : public QDialog{
Q_OBJECT
public:
    explicit MoveArticleDlg(QWidget *parent = 0, int mode = 0, int id = 0);
protected:
    void closeEvent(QCloseEvent *event);
signals:
    void updateGrid(int);
public slots:
    void selectItem(int);
    void executeQuery();
private:
    int getFatherValue(int);
    QStringList getQueryList(int);

    QComboBox *cmbList;

    int index,
        value,
        option;

    QList<int> lst;
};

#endif // MOVEARTICLEDLG_H
