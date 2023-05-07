#ifndef DLGADMINUSER_H
#define DLGADMINUSER_H

#include <QDialog>
#include <QWidget>
#include <QtSql>

#include "alarife.h"

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

class dlgAdminUser : public QDialog
{
    Q_OBJECT
public:
    explicit dlgAdminUser(QWidget *parent = 0);

private slots:
    void ActiveUserExec(bool mode=false);
    void EditUserExec();
    void InsertUserExec();
    void DeleteUserExec();
    void ShowUser(QModelIndex index);
    void getUserIndex(QModelIndex index);
private:

    void ShowUserValue(bool type = false);

    QSpacerItem *vSpacer;

    QLineEdit *edtPass,
              *edtLogin;

    QCheckBox *chkActiveUser;

    QPushButton *btnEdit,
                *btnIndert,
                *btnDelete;

    QTableView *tblUserView;

    QSqlQueryModel *users;

    QModelIndex idUsr;

};

#endif // DLGADMINUSER_H
