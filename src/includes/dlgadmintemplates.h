#ifndef DLGADMINTEMPLATES_H
#define DLGADMINTEMPLATES_H

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

class dlgAdminTemplates : public QDialog
{
    Q_OBJECT
public:
    explicit dlgAdminTemplates(QWidget *parent = 0);
    bool getModifique(){return isModifique;}
private slots:
    void getSelection(QString);
    void deleteTemplates();
    void insertTemplates();
private:
    QLabel *lblText;

    QSpacerItem *vSpacer;

    QPushButton *btnAdd,
                *btnDelete;

    QTextEdit *TemplateList;

    QString name;

    bool isModifique;

    TemplateView *view;
};

#endif // DLGADMINTEMPLATES_H
