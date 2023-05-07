#include <QtGui>
#include <QSpacerItem>

#include "../includes/reference.h"
#include "../includes/database.h"
#include "../includes/sqlfunctions.h"
#include "../includes/strfunction.h"

Reference::Reference(QWidget *parent, int id) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__); //Qt::Dialog | Qt::WindowTitleHint);

    create_widgets();

    father = id;
    target = 0;

    getListValues();
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(btnAcept, SIGNAL(clicked()), this, SLOT(insert()));
    connect(btnFirst, SIGNAL(clicked()), this, SLOT(first()));
    connect(btnPrior, SIGNAL(clicked()), this, SLOT(prior()));
    connect(btnNext, SIGNAL(clicked()), this, SLOT(next()));
    connect(btnLast, SIGNAL(clicked()), this, SLOT(last()));
    connect(btnEdit, SIGNAL(clicked()), this, SLOT(edit()));
    connect(btnDelete, SIGNAL(clicked()), this, SLOT(del()));
    connect(lstArticles, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(create_link()));
    connect(btnLink, SIGNAL(clicked()), this, SLOT(delete_link()));
    connect(btnFilter, SIGNAL(clicked()), this, SLOT(filter()));
    connect(edtFilter,SIGNAL(textChanged(QString)),this, SLOT(empty_filter()));

    resize(400,250);
}

void Reference::insert()
{
    QString title, summary, link;

    title = tr("'%1'").arg(edtTile->text());
    summary = tr("'%1'").arg(txtSumary->toPlainText());
    int id = 0;//, target = 0;

    if (btnLink->isEnabled()) btnLink->setEnabled(false);

    if (!edtTile->text().isEmpty() || !txtSumary->toPlainText().isEmpty()/* || lstArticles->currentRow() > 0*/)
    {
        switch (m_mode)
        {
            case INSERT :
                            id = sqlAutoCountRow("referencias", QString("articulo = %1").arg(father)) + 1;
                            sqlInsertRecord_N("referencias",
                                            "id_referencia, articulo, enlace, nombre, resumen, texto",
                                            QString("%1, %2, %3, <&~%4~&>, <&~%5~&>, <&~%6~&>").
                                                    arg(id).
                                                    arg(father).
                                                    arg(target).
                                                    arg(edtTile->text()).
                                                    arg(txtSumary->toPlainText()).
                                                    arg(selectedText)
                                            );
                            link = QString("<reference id=\"%1\" title=\"Texto de referencia"
                                           "\">%2</reference>").arg(id).arg(selectedText);
                            emit linked(link);
                            close();
                break;
            case EDIT :
                            sqlUpdateRecord_N("referencias",
                                            QString(" enlace = %1, nombre = <&~%2~&>, resumen = <&~%3~&> ").
                                                     arg(target).
                                                     arg(edtTile->text()).
                                                     arg(txtSumary->toPlainText()),
                                            QString(" WHERE id_referencia = %1 AND articulo = %2 ").
                                                     arg(qryReference->value(0).toInt()).
                                                     arg(father)
                                            );
                            setMode(READONLY);
                            update_mode();
                break;
        case READONLY:

                break;
        }
    } else
        QMessageBox::information(this, tr("Existen campos requeridos vacios."),
                                 tr("Por favor, inserte una descripción, un resumen "
                                    "o seleccione un artículo relacionado."));
}

void Reference::first()
{
    step = 0;
    qryReference->first();
    load_data();
}

void Reference::prior()
{
    step--;
    if (step > 0) qryReference->previous();
    load_data();
}

void Reference::next()
{
    step++;
    if (step < count) qryReference->next();
    load_data();
}

void Reference::last()
{
    step = count;
    qryReference->last();
    load_data();
}

void Reference::edit()
{
    target = sqlGetIdValue("articulo",
                           "id_articulo",
                           QString("nombre = '%1'").arg(qryReference->value(4).toString()));
    setMode(EDIT);
    update_mode();
}

void Reference::del()
{
    int id = qryReference->value(0).toInt();
    QString text = qryReference->value(3).toString();
    text = strHtmlEncode(text);

    sqlDeleteRecord_N("referencias", QString("id_referencia = %1 AND articulo = %2").arg(id).arg(father));

    QString link = QString("<reference id=\"%1\" title=\"Texto de referencia"
                           "\">%2</reference>").arg(id).arg(text);
    emit unlinked(link, text);
}

void Reference::create_link()
{
    if (!btnLink->isEnabled()) btnLink->setEnabled(true);
    lblLink->setText(tr("Artículo relacionado: %1").arg(lstArticles->currentItem()->text()));
    target = sqlGetIdValue("articulo", "id_articulo", QString("nombre = '%1'").arg(lstArticles->currentItem()->text()));
}

void Reference::delete_link()
{
    if (btnLink->isEnabled()) btnLink->setEnabled(false);
    lblLink->setText(tr("Artículo relacionado:"));
    target = 0;
}

void Reference::filter()
{
    getListValues();
    qDebug() << "count: " << lstArticles->count() << endl;
    if (lstArticles->count() == 0)
        edtFilter->setStyleSheet(tr("background-color: rgb(255, 140, 140);"));
}

void Reference::empty_filter()
{
    if (edtFilter->text().isEmpty())
        getListValues();
    edtFilter->setStyleSheet(tr("background-color: rgb(255, 255, 255);"));
}

void Reference::setMode(Mode m)
{
    m_mode = m;
    update_mode();
}

void Reference::create_widgets()
{
    lblTitle = new QLabel(tr("Descripción:"));
    lblSumary = new QLabel(tr("Resumen:"));
    lblLink = new QLabel(tr("Artículo relacionado:"));
    lblInformation = new QLabel;

    edtTile = new QLineEdit;

    txtSumary = new QTextEdit;

    lstArticles = new QListWidget;
    lstArticles->setMinimumHeight(70);
    lstArticles->setToolTip(tr("Doble clic para relacionar artículos."));

    btnAcept = new QPushButton(tr("Aceptar"));
    btnCancel = new QPushButton(tr("Cancelar"));

    QSpacerItem *vSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    btnFirst = new QToolButton;
    btnPrior = new QToolButton;
    btnNext = new QToolButton;
    btnLast = new QToolButton;
    btnEdit = new QToolButton;
    btnDelete = new QToolButton;
    btnLink = new QToolButton;

    btnLink->setIcon(QPixmap(":/images/result-error.png"));
    btnLink->setEnabled(false);
    btnLink->setToolTip(tr("Eliminar relación"));

    btnFirst->setIcon(QIcon(QPixmap(":/images/go-first.png")));
    btnPrior->setIcon(QIcon(QPixmap(":/images/go-previous.png")));
    btnNext->setIcon(QIcon(QPixmap(":/images/go-next.png")));
    btnLast->setIcon(QIcon(QPixmap(":/images/go-last.png")));
    btnEdit->setIcon(QIcon(QPixmap(":/images/go-edit.png")));
    btnDelete->setIcon(QIcon(QPixmap(":/images/go-delete.png")));

    btnFirst->setToolTip(tr("Primera referencia"));
    btnPrior->setToolTip(tr("Referencia anterior"));
    btnNext->setToolTip(tr("Siguiente referencia"));
    btnLast->setToolTip(tr("Última referencia"));
    btnEdit->setToolTip(tr("Editar referencia"));
    btnDelete->setToolTip(tr("Eliminar referencia"));

    QHBoxLayout *lytButtons = new QHBoxLayout();
    lytButtons->addWidget(btnFirst);
    lytButtons->addWidget(btnPrior);
    lytButtons->addWidget(btnNext);
    lytButtons->addWidget(btnLast);
    lytButtons->addWidget(btnEdit);
    lytButtons->addWidget(btnDelete);
    lytButtons->addItem(hSpacer);
    lytButtons->addWidget(btnCancel);
    lytButtons->addWidget(btnAcept);

    QHBoxLayout *lytLink = new QHBoxLayout;
    lytLink->addWidget(lblLink);
    lytLink->addWidget(btnLink);

    lblFilter = new QLabel(tr("Buscar:"));
    edtFilter = new QLineEdit;
    btnFilter = new QToolButton;
    btnFilter->setToolTip(tr("Buscar"));
    btnFilter->setIcon(QIcon(":/images/action_find.png"));

    QHBoxLayout *lytFilter = new QHBoxLayout;
    lytFilter->addWidget(lblFilter);
    lytFilter->addWidget(edtFilter);
    lytFilter->addWidget(btnFilter);

    QVBoxLayout *lytMain = new QVBoxLayout;
    lytMain->addWidget(lblInformation);
    lytMain->addWidget(lblTitle);
    lytMain->addWidget(edtTile);
    lytMain->addWidget(lblSumary);
    lytMain->addWidget(txtSumary);
    lytMain->addLayout(lytLink);
    lytMain->addWidget(lstArticles);
    lytMain->addLayout(lytFilter);
    lytMain->addItem(vSpacer);
    lytMain->addLayout(lytButtons);

    setLayout(lytMain);

    setMinimumSize(400,340);
}

void Reference::update_mode()
{
    bool a = true;
    bool b = false;

    switch (m_mode)
    {
        case INSERT :
                        setWindowTitle(tr("Insertar referencias"));
                        edit_link(true);
            break;
        case EDIT :
                        setWindowTitle(tr("Editar referencias"));
                        edit_link(true);
            break;
        case READONLY :
                        a = false;
                        b = true;
                        setWindowTitle(tr("Mostrar referencias"));
                        edit_link(false);
                        qryReference = new QSqlQuery;
                        qryReference->prepare(SQL.arg(father));
                        qryReference->exec();
                        qryReference->first();
                        count = sqlAutoCountRow("referencias", QString("articulo = %1").arg(father));
                        step = 0;
                        load_data();

            break;
    }

    btnAcept->setVisible(a);
    btnCancel->setVisible(a);
    btnFirst->setVisible(b);
    btnPrior->setVisible(b);
    btnNext->setVisible(b);
    btnLast->setVisible(b);
    btnEdit->setVisible(b);
    btnDelete->setVisible(b);
    edtTile->setReadOnly(b);
    txtSumary->setReadOnly(b);
    lblInformation->setVisible(b);
}

void Reference::load_data()
{
    lblInformation->setText(tr("Texto seleccionado: %1").arg(qryReference->value(3).toString()));
    edtTile->setText(qryReference->value(1).toString());
    txtSumary->setText(qryReference->value(2).toString());

    lblLink->setText(tr("Artículo relacionado: %1").arg(qryReference->value(4).toString()));

    step == 0 ? btnFirst->setEnabled(false) : btnFirst->setEnabled(true);
    step == 0 ? btnPrior->setEnabled(false) : btnPrior->setEnabled(true);
    step == count ? btnLast->setEnabled(false) : btnLast->setEnabled(true);
    step == count ? btnNext->setEnabled(false) : btnNext->setEnabled(true);
}

void Reference::edit_link(bool mode)
{
    lstArticles->setVisible(mode);
    btnLink->setVisible(mode);
    lblFilter->setVisible(mode);
    btnFilter->setVisible(mode);
    edtFilter->setVisible(mode);
}


void Reference::getListValues()
{
    QString sqlArticle = QString("SELECT nombre FROM articulo "
                                 "WHERE id_articulo != %1").arg(father);
    if (!edtFilter->text().isEmpty())
    {
        sqlArticle.append(QString(" AND articulo.nombre LIKE '%%1%' ").
                         arg(edtFilter->text()));
    }
    sqlArticle.append(" ORDER BY articulo.nombre;");

    lstArticles->clear();

    QSqlQuery *qry = new QSqlQuery;
    qry->exec(sqlArticle);
    while (qry->next()) lstArticles->addItem(qry->value(0).toString());
    delete qry;
}

