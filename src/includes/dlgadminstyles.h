#ifndef DLGADMINSTYLES_H
#define DLGADMINSTYLES_H

#include <QDialog>
#include <QWidget>
#include <QtSql>

#include "alarife.h"
#include "templateview.h"

QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
class QTableView;
class QSqlDatabase;
class QLabel;
class QLineEdit;
class QComboBox;
class QCheckBox;
class QSpacerItem;
class QTextEdit;
class QModelIndex;
QT_END_NAMESPACE

class dlgAdminStyles : public QDialog
{
    Q_OBJECT
public:
    explicit dlgAdminStyles(QWidget *parent = 0);
public slots:
    void ChangeStyle(QString style = "");
    void LoadStyleFiles(QString name = "");
    void ActiveControlStyle(bool active = false);
    void setStyleName(QString style) { strStyle = style; }
    void setStyleSheetName(QString stylesheet) { strStylesheet = stylesheet; }
    void saveAparence();
    void deleteStyle();
    void insertStyle();
    void updateStyles();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void LoadStyleNames();
    void setAppStyle();
    void setAppStyleSheet();

    QComboBox *cmbStyle,
              *cmbControls,
              *cmbLanguage;

    QCheckBox *chkActiveCtrl;

    QSpacerItem *vSpacer;

    QPushButton *btnDelete,
                *btnEdit,
                *btnNew;

    QString strStyle,
            strStylesheet,
            stylesheet,
            curFile;

    bool isModifique;
};

#endif // DLGADMINSTYLES_H
