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

#ifndef LOADIMAGEDLG_H
#define LOADIMAGEDLG_H

#include <QDialog>

#include "database.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QTabWidget;
class QSqlQuery;
class QFileInfo;
QT_END_NAMESPACE

extern Book encyclopedia;

//----------------------------------------------------------------------------------------------
class LoadImageDlg : public QDialog{
Q_OBJECT
public:
    LoadImageDlg(QWidget *parent = 0, int mode = 0);
    void setFileExtension(QString);
    void setMediaPath(QString path) { mediaPath = path; }
    void setFileType(int);
    void loadValues();
signals:
    void refreshQuery();
private slots:
    void AceptExec();
    void CancelExec();
    void InsertExec();
private:
    void showValues();
    QString fileSize(quint64);

    QLabel *lblPreview,
           *lblText,
           *lblDescribe;

    QLineEdit *edtPath,
              *edtDescription;

    QPushButton *btnOk,
                *btnClose,
                *btnLoad;
    QString extension,
            fileName,
            oldFileName,
            tmpName,
            ini,
            mediaPath;

    int id,
        type,
        md;
};

#endif // IMAGEDLG_H
