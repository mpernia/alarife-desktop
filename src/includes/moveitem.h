#ifndef MOVEELEMENT_H
#define MOVEELEMENT_H

#include <QtGui>
#include <QDialog>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QTableWidget;
class QSqlDatabase;
class QLabel;
class QToolButton;
class QCheckBox;
class QSpacerItem;
class QModelIndex;
QT_END_NAMESPACE

const QString sqlLevels = "SELECT nombre, id_nivel, padre FROM nivel WHERE padre = %1 ORDER BY orden ASC";
const QString sqlArticles = "SELECT nombre, id_articulo, nivel FROM articulo WHERE nivel = %1 ORDER BY nombre ASC";

class MoveElement : public QDialog
{
    Q_OBJECT
    
public:
    MoveElement(QWidget *parent = 0);
    ~MoveElement();

signals:
    void isMoved();

private slots:
    void move_left_to_right();
    void move_right_to_left();
    void show_left_items();
    void show_right_items();

private:
    void create_widget();
    void load_records(QTableWidget **, int);
    void update_list();
    void move_itemWidget(const QTableWidget*, int, int);

    QCheckBox *chkSynchronize;
    QTableWidget *tblLeft;
    QTableWidget *tblRight;
    QToolButton *btnToRight;
    QToolButton *btnToLeft;

    QSqlQueryModel *modelLeft;
    QSqlQueryModel *modelRight;

    QSqlQuery *qryData;

    QStack<int> backLeft;
    QStack<int> backRight;
};

#endif // MOVEELEMENT_H
