#ifndef REFERENCE_H
#define REFERENCE_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QComboBox;
class QListWidget;
class QTextEdit;
class QLineEdit;
class QToolButton;
class QSqlQuery;
QT_END_NAMESPACE

const QString SQL = "SELECT referencias.id_referencia, referencias.nombre, referencias.resumen, "
                    "referencias.texto, articulo.nombre FROM referencias LEFT OUTER JOIN "
                    "articulo ON referencias.enlace = articulo.id_articulo WHERE articulo = %1 "
                    "ORDER BY id_referencia ASC";

class Reference : public QDialog
{
    Q_OBJECT
    Q_ENUMS(Mode)
    Q_PROPERTY(Mode mode READ mode WRITE setMode)
public:
    enum Mode { INSERT, EDIT, READONLY };
    explicit Reference(QWidget *parent = 0, int id = 0);
    void setSelectedText(QString text) { selectedText = text; }
    void setMode(Mode m);
    Mode mode() const { return m_mode; }

signals:
    void linked(QString);
    void unlinked(QString, QString);

private slots:
    void insert();
    void first();
    void prior();
    void next();
    void last();
    void edit();
    void del();
    void create_link();
    void delete_link();
    void filter();
    void empty_filter();

private:
    void create_widgets();
    void update_mode();
    void load_data();
    void edit_link(bool);
    void getListValues();

    QLabel *lblTitle;
    QLabel *lblSumary;
    QLabel *lblLink;
    QLabel *lblInformation;
    QLabel *lblInfoLink;
    QLabel *lblFilter;
    QLineEdit *edtTile;
    QLineEdit *edtFilter;
    QTextEdit *txtSumary;
    QListWidget *lstArticles;
    QPushButton *btnAcept;
    QPushButton *btnCancel;

    QToolButton *btnFirst;
    QToolButton *btnPrior;
    QToolButton *btnNext;
    QToolButton *btnLast;
    QToolButton *btnEdit;
    QToolButton *btnDelete;
    QToolButton *btnLink;
    QToolButton *btnFilter;

    QString selectedText;
    int father;
    int count;
    int step;
    int target;

    QSqlQuery *qryReference;

    Mode m_mode;
};

#endif // REFERENCE_H
