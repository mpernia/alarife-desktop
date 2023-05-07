/****************************************************************************
**
** Copyleft: 2014. Maikel Enrique Pernia Matos.
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

#include <QtGui>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QDateTime>
#include <QSqlQuery>
#include "../includes/database.h"
#include "../includes/sqlfunctions.h"
#include "../includes/strfunction.h"
#include "../includes/itemproperty.h"

ItemProperty::ItemProperty(ItemId i, QWidget *parent) : QDialog(parent){
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__); 

    createWidgets();
    setWindowTitle(tr("Propiedades del artículo"));
    setMinimumSize(487, 450);

    itemId = i;

    QSqlQuery *qry_0 = new QSqlQuery;

    switch (itemId.getMode()){
    case ItemId::READONLY : qry_0->clear();
                            qry_0->exec(QString("SELECT nombre FROM usuario WHERE id_usuario = %1").arg(usuario));
                            qry_0->first();
                            cmbUser->addItems(QStringList() << qry_0->value(0).toString());

                            getAllValues();
                            getAllListSelectedValues();
                            setUpdateInterface(true);
                            hideFilterToolBar();
        break;
    case ItemId::EDIT :     modified = false;
                            qry_0->clear();
                            qry_0->exec(QString("SELECT nombre FROM usuario"));
                            while (qry_0->next()) cmbUser->addItems(QStringList() << qry_0->value(0).toString());
                            cmbUser->setCurrentIndex(1);

                            getAllValues();
                            getAllListSelectedValues();
                            getAllListAvailableValues();
                            setUpdateInterface(false);
        break;
    case ItemId::INSERT :   modified = false;
                            itemId.setItem(sqlAutoIncremental("id_articulo", "articulo"));
                            qry_0->clear();
                            qry_0->exec(QString("SELECT nombre FROM usuario"));
                            while (qry_0->next()) cmbUser->addItems(QStringList() << qry_0->value(0).toString());
                            cmbUser->count() == 2 ? cmbUser->setCurrentIndex(1) : cmbUser->setCurrentIndex(0);
                            currentIndexChanged(cmbUser->currentText());

                            getAllListAvailableValues();
                            setUpdateInterface(false);
                            modifiqueDate->setVisible(false);
                            label_5->setVisible(false);
        break;
    }

    delete qry_0;

    connect(btnAcept, SIGNAL(clicked()), this, SLOT(save()));
    connect(btnAddItems, SIGNAL(clicked()), this, SLOT(insertArticles()));
    connect(btnAddAuthors, SIGNAL(clicked()), this, SLOT(insertAuthors()));
    connect(btnDelAuthor, SIGNAL(clicked()), this, SLOT(deleteAuthor()));
    connect(btnDelItems, SIGNAL(clicked()), this, SLOT(deleteArticles()));
    connect(cmbUser, SIGNAL(currentIndexChanged(QString)), this, SLOT(currentIndexChanged(const QString&)));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(btnFilter, SIGNAL(clicked()), this, SLOT(filterMediaExec()));
    connect(edtFilter,SIGNAL(textChanged(QString)),this, SLOT(emptyFilter()));
    connect(rdbAuthor, SIGNAL(clicked()), this, SLOT(updateList()));
    connect(rdbItems, SIGNAL(clicked()), this, SLOT(updateList()));
}

ItemProperty::~ItemProperty()
{
}

void ItemProperty::closeEvent(QCloseEvent *event)
{
    if (modified == true) emit refreshValues();
    event->accept();
}

void ItemProperty::createWidgets()
{
    tabWidget = new QTabWidget(this);

    tab = new QWidget();
    edtTitle = new QLineEdit;
    edtSource = new QLineEdit;

    mmKeywords = new QPlainTextEdit;
    mmKeywords->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mmKeywords->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    label = new QLabel(tr("Título:"), tab);
    label_2 = new QLabel(tr("Tomado de:"), tab);
    label_3 = new QLabel(tr("Palabras claves:"), tab);
    label_4 = new QLabel(tr("Fecha de creación:"), tab);
    label_5 = new QLabel(tr("Fecha de modificación:"), tab);
    lblNombrePropietario = new QLabel(tr("Propietario: "));

    cmbUser = new QComboBox;

    creationDate = new QDateTimeEdit(tab);
    creationDate->setCalendarPopup(true);
    creationDate->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    creationDate->setDateTime(QDateTime::currentDateTime());

    modifiqueDate = new QDateTimeEdit(tab);
    modifiqueDate->setCalendarPopup(true);
    modifiqueDate->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    modifiqueDate->setDateTime(QDateTime::currentDateTime());

    QSpacerItem *hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QGridLayout *lytDate = new QGridLayout;
    lytDate->addWidget(label_4, 0, 0);
    lytDate->addItem(hSpacer, 0,1);
    lytDate->addWidget(label_5, 0, 2);
    lytDate->addWidget(creationDate, 1, 0);
    lytDate->addWidget(modifiqueDate, 1, 2);

    QVBoxLayout *lytMain_1 = new QVBoxLayout(tab);
    lytMain_1->addWidget(label);
    lytMain_1->addWidget(edtTitle);
    lytMain_1->addWidget(label_2);
    lytMain_1->addWidget(edtSource);
    lytMain_1->addWidget(label_3);
    lytMain_1->addWidget(mmKeywords);
    lytMain_1->addLayout(lytDate);
    lytMain_1->addWidget(lblNombrePropietario);
    lytMain_1->addWidget(cmbUser);

    tabWidget->addTab(tab, tr("General"));

    mmSummary = new QPlainTextEdit;
    mmSummary->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tabWidget->addTab(mmSummary, tr("Resumen"));

    tab_2 = new QWidget();

    rdbAuthor = new QRadioButton(tr("Autores"));
    rdbItems = new QRadioButton(tr("Artículos"));

    edtFilter = new QLineEdit;

    btnFilter = new QToolButton;

    btnFilter->setIcon(QIcon(":/images/action_find.png"));
    btnFilter->setToolTip(tr("Buscar"));

    QHBoxLayout *lytFilter = new QHBoxLayout;
    label_6 = new QLabel(tr("Buscar en:"));
    lytFilter->addWidget(label_6);
    lytFilter->addWidget(rdbAuthor);
    lytFilter->addWidget(rdbItems);
    lytFilter->addWidget(edtFilter);
    lytFilter->addWidget(btnFilter);

    groupBox = new QGroupBox;
    groupBox->setTitle(tr("Artículos:"));

    label_7 = new QLabel(tr("Artículos relacionados:"), tab_2);
    label_8 = new QLabel(tr("Artículos disponibles:"), tab_2);

    lstSelectedArticles = new QListWidget;

    btnAddItems = new QToolButton;
    btnAddItems->setText("...");
    btnAddItems->setToolTip(tr("Relacionar artículo"));
    btnAddItems->setArrowType(Qt::LeftArrow);

    btnDelItems = new QToolButton;
    btnDelItems->setText("...");
    btnDelItems->setToolTip(tr("Eliminar relación de artículo"));
    btnDelItems->setArrowType(Qt::RightArrow);

    lstAvailableArticles = new QListWidget;

    QVBoxLayout *lytItemsLeft = new QVBoxLayout;
    lytItemsLeft->addWidget(label_7);
    lytItemsLeft->addWidget(lstSelectedArticles);

    QVBoxLayout *lytItemsRight = new QVBoxLayout;
    lytItemsRight->addWidget(label_8);
    lytItemsRight->addWidget(lstAvailableArticles);

    QVBoxLayout *lytButtonItems = new QVBoxLayout;
    lytButtonItems->addWidget(btnAddItems);
    lytButtonItems->addWidget(btnDelItems);

    QHBoxLayout *lytItems = new QHBoxLayout(groupBox);
    lytItems->addLayout(lytItemsLeft);
    lytItems->addLayout(lytButtonItems);
    lytItems->addLayout(lytItemsRight);

    groupBox_2 = new QGroupBox;
    groupBox_2->setTitle(tr("Autores:"));

    label_9 = new QLabel(tr("Autores relacionados:"), tab_2);
    label_10 = new QLabel(tr("Autores disponibles:"), tab_2);

    lstSelectedAuthors = new QListWidget;

    btnAddAuthors = new QToolButton;
    btnAddAuthors->setEnabled(true);
    btnAddAuthors->setText("...");
    btnAddAuthors->setToolTip(tr("Relacionar autor"));
    btnAddAuthors->setArrowType(Qt::LeftArrow);

    btnDelAuthor = new QToolButton;
    btnDelAuthor->setText("...");
    btnDelAuthor->setToolTip(tr("Eliminar relación de autor"));
    btnDelAuthor->setArrowType(Qt::RightArrow);


    lstAvailableAuthors = new QListWidget;

    QVBoxLayout *lytAuthorsLeft = new QVBoxLayout;
    lytAuthorsLeft->addWidget(label_9);
    lytAuthorsLeft->addWidget(lstSelectedAuthors);

    QVBoxLayout *lytAuthorsRight = new QVBoxLayout;
    lytAuthorsRight->addWidget(label_10);
    lytAuthorsRight->addWidget(lstAvailableAuthors);

    QVBoxLayout *lytButtonAuthors = new QVBoxLayout;
    lytButtonAuthors->addWidget(btnAddAuthors);
    lytButtonAuthors->addWidget(btnDelAuthor);

    QHBoxLayout *lytAutors = new QHBoxLayout(groupBox_2);
    lytAutors->addLayout(lytAuthorsLeft);
    lytAutors->addLayout(lytButtonAuthors);
    lytAutors->addLayout(lytAuthorsRight);

    QVBoxLayout *lytMain_2 = new QVBoxLayout(tab_2);
    lytMain_2->addLayout(lytFilter);
    lytMain_2->addWidget(groupBox);
    lytMain_2->addWidget(groupBox_2);

    tabWidget->addTab(tab_2, tr("Relaciones"));
    tabWidget->setCurrentIndex(0);

    btnAcept = new QPushButton;
    btnAcept->setText(tr("Aceptar"));

    btnCancel = new QPushButton;
    btnCancel->setText("Cancelar");

    QHBoxLayout *lytButtons = new QHBoxLayout;
    lytButtons->setAlignment(Qt::AlignRight);
    lytButtons->addWidget(btnAcept);
    lytButtons->addWidget(btnCancel);

    QVBoxLayout *lytMain = new QVBoxLayout;
    lytMain->addWidget(tabWidget);
    lytMain->addLayout(lytButtons);
    setLayout(lytMain);
}

void ItemProperty::setUpdateInterface(bool b)
{
    edtTitle->setReadOnly(b);
    mmSummary->setReadOnly(b);
    mmKeywords->setReadOnly(b);
    edtSource->setReadOnly(b);

    cmbUser->setDisabled(b);
    creationDate->setDisabled(b);
    modifiqueDate->setDisabled(b);


    btnAddAuthors->setDisabled(b);
    btnDelAuthor->setDisabled(b);
    btnAddItems->setDisabled(b);
    btnDelItems->setDisabled(b);
}

void ItemProperty::getAllValues()
{
    QSqlQuery *qry_1 = new QSqlQuery;
    qry_1->exec(QString("SELECT nombre, resumen, claves, fuente, fecha1, fecha2, usuario "
                        "FROM articulo WHERE id_articulo = %1").
                arg(itemId.getItem()));
    qry_1->first();

    edtTitle->setText(qry_1->value(0).toString());
    mmSummary->setPlainText(qry_1->value(1).toString());
    mmKeywords->setPlainText(qry_1->value(2).toString());
    edtSource->setText(qry_1->value(3).toString());
    QVariant fecha1 = qry_1->value(4);
    QVariant fecha2 = qry_1->value(5);
    usuario = qry_1->value(6).toInt();

    creationDate->setDateTime(fecha1.toDateTime());
    modifiqueDate->setDateTime(fecha2.toDateTime());

    QSqlQuery query;
    QString q = QString("SELECT nombre FROM usuario WHERE id_usuario = %1").arg(QString::number(usuario));
    query.exec(q);
    query.first();
    lblNombrePropietario->setText(tr("Propietario: %1").arg(query.value(0).toString()));
}

void ItemProperty::getAllListSelectedValues()
{
    getListWidgetValues(QString("SELECT DISTINCT articulo.nombre FROM articulo INNER JOIN "
                                "articuloarticulos ON articulo.id_articulo == articuloarticulos."
                                "IdArticuloLink WHERE articuloarticulos.articulo == %1 ORDER BY "
                                "articulo.nombre").arg(itemId.getItem()), lstSelectedArticles);

    getListWidgetValues(QString("SELECT DISTINCT autor.nombre FROM autor INNER JOIN "
                                "articuloautores ON autor.id_autor == articuloautores.autor WHERE "
                                "articuloautores.articulo == %1 ORDER BY autor.nombre").
                        arg(itemId.getItem()), lstSelectedAuthors);
}

void ItemProperty::getAllListAvailableValues()
{
    getListWidgetValues(SQL_TitlesList.arg(itemId.getItem()).arg(itemId.getItem()), lstAvailableArticles);

    getListWidgetValues(SQL_AuthorsList.arg(itemId.getItem()), lstAvailableAuthors);
}

void ItemProperty::getListWidgetValues(QString sql, QListWidget *listWidget)
{
    QSqlQuery *qryList = new QSqlQuery;
    listWidget->clear();
    qryList->exec(sql);
    while (qryList->next()) new QListWidgetItem(qryList->value(0).toString(), listWidget);
    delete qryList;
}

void ItemProperty::getListValues() //maikel
{
    QString sqlAuthor = SQL_AuthorsList.arg(itemId.getItem()),
            sqlTitle = SQL_TitlesList.arg(itemId.getItem()).arg(itemId.getItem());

    if (rdbAuthor->isChecked())
    {
        if (!edtFilter->text().isEmpty())
        {
            sqlAuthor.append(QString(" AND autor.nombre LIKE '%%1%' ORDER BY autor.nombre").
                                    arg(edtFilter->text()));
        }
        getListWidgetValues(sqlAuthor, lstAvailableAuthors);
    }

    if (rdbItems->isChecked())
    {
        if (!edtFilter->text().isEmpty())
        {
            sqlTitle.append(QString(" AND articulo.nombre LIKE '%%1%' ORDER BY articulo.nombre").
                                   arg(edtFilter->text()));
        }
        getListWidgetValues(sqlTitle, lstAvailableArticles);
    }
}

void ItemProperty::hideFilterToolBar()
{
    label_6->setVisible(false);
    edtFilter->setVisible(false);
    rdbAuthor->setVisible(false);
    rdbItems->setVisible(false);
    btnFilter->setVisible(false);
    btnCancel->setText(tr("Cerrar"));
    btnAcept->setVisible(false);

    btnAddAuthors->setVisible(false);
    btnDelAuthor->setVisible(false);
    btnAddItems->setVisible(false);
    btnDelItems->setVisible(false);
    label_8->setVisible(false);
    label_10->setVisible(false);
    lstAvailableAuthors->setVisible(false);
    lstAvailableArticles->setVisible(false);
    cmbUser->setVisible(false);
}

void ItemProperty::save()
{

    if(edtTitle->text() == ""){
        QMessageBox::warning(this,"error",tr("Ponga el título del artículo"));
        return;
    }else if(lblNombrePropietario->text() == "Propietario: " ){
        QMessageBox::warning(this,"error",tr("Seleccione el propietario"));
        return;
    }

    int id_user = sqlGetIdValue("usuario",
                            "id_usuario",
                            QString("nombre = '%1'").arg(lblNombrePropietario->text().remove("Propietario: ")));

    switch (itemId.getMode()){
    case ItemId::EDIT :

                sqlUpdateRecord_N("articulo",
                                QString("nombre=<&~%1~&>, resumen=<&~%2~&>, claves=<&~%3~&>, fuente=<&~%4~&>, "
                                        "fecha1=<&~%5~&>, fecha2=<&~%6~&>, usuario=<&~%7~&>, nivel=%8, "
                                        "enciclopedia=%9").arg(edtTitle->text()).
                                                            arg(mmSummary->toPlainText()).
                                                            arg(mmKeywords->toPlainText()).
                                                            arg(edtSource->text()).
                                                            arg(creationDate->text()).
                                                            arg(modifiqueDate->text()).
                                                            //arg(itemId.getItem()).
                                                            arg(id_user).
                                                            arg(itemId.getFather()).
                                                            arg(itemId.getEnciclopedia()),
                                QString("WHERE id_articulo=%1").arg(itemId.getItem()));
                modified = true;
        break;
    case ItemId::INSERT :
                sqlInsertRecord_N("articulo",
                                "id_articulo, nombre, resumen, claves, fuente, "
                                "fecha1, fecha2, usuario, nivel, enciclopedia",
                                QString("%1, <&~%2~&>, <&~%3~&>, <&~%4~&>, <&~%5~&>, "
                                        "<&~%6~&>, <&~%7~&>, <&~%8~&>, %9, %10").
                                        arg(itemId.getItem()).
                                        arg(edtTitle->text()).
                                        arg(mmSummary->toPlainText()).
                                        arg(mmKeywords->toPlainText()).
                                        arg(edtSource->text()).
                                        arg(creationDate->text()).
                                        arg(creationDate->text()).
                                        arg(id_user).
                                        arg(itemId.getFather()).
                                        arg(itemId.getEnciclopedia()));

                itemId.setMode(ItemId::EDIT);
                emit inserted(itemId);
        break;
    }

    insertNewAuthors();
    insertNewItems();

    modified = true;
    close();
}

void ItemProperty::insertNewAuthors()
{
    QSqlQuery query;
    QString autor;
    QString id_autor;
    QString q;
    sqlDeleteRecord_N("articuloautores",
                    QString("articulo=%1").arg(itemId.getItem()));

    for(int i =0; i < lstSelectedAuthors->count(); i++)
    {
        autor = lstSelectedAuthors->item(i)->text();
        q = QString("SELECT id_autor FROM autor WHERE nombre='%1'").arg(autor);
        query.exec(q);
        query.first();
        id_autor = query.value(0).toString();
        sqlInsertRecord_N("articuloautores",
                        "articulo, autor",
                        QString("%1,%2").arg(itemId.getItem()).arg(id_autor));
     }
}

void ItemProperty::insertNewItems()
{
    QSqlQuery query;
    QString name_item;
    QString id_item;
    QString q;
    sqlDeleteRecord_N("articuloarticulos",
                    QString("articulo=%1").arg(itemId.getItem()));

    for(int i =0; i < lstSelectedArticles->count(); i++)
    {
        name_item = lstSelectedArticles->item(i)->text();
        q = QString("SELECT id_articulo FROM articulo WHERE nombre='%1'").arg(name_item);
        query.exec(q);
        query.first();
        id_item = query.value(0).toString();
        sqlInsertRecord_N("articuloarticulos",
                        "articulo, IdArticuloLink",
                        QString("%1, %2").arg(itemId.getItem()).arg(id_item));
     }
}

void ItemProperty::insertArticles()
{
    if(lstAvailableArticles->currentItem() == NULL)
        return;

    lstSelectedArticles->addItem(lstAvailableArticles->currentItem()->text());
    delete lstAvailableArticles->currentItem();
}

void ItemProperty::insertAuthors()
{
    if(lstAvailableAuthors->currentItem() == NULL)
        return;

    lstSelectedAuthors->addItem(lstAvailableAuthors->currentItem()->text());
    delete lstAvailableAuthors->currentItem();
}

void ItemProperty::deleteAuthor()
{
    if(lstSelectedAuthors->currentItem() == NULL)
        return;

    lstAvailableAuthors->addItem(lstSelectedAuthors->currentItem()->text());
    delete lstSelectedAuthors->currentItem();
}

void ItemProperty::deleteArticles()
{
    if(lstSelectedArticles->currentItem() == NULL)
        return;

    lstAvailableArticles->addItem(lstSelectedArticles->currentItem()->text());
    delete lstSelectedArticles->currentItem();
}

void ItemProperty::currentIndexChanged(const QString &arg1)
{
    lblNombrePropietario->setText(tr("Propietario: %1").arg(arg1));
}


void ItemProperty::enableArticles()
{
    lstSelectedArticles->setEnabled(true);
}

void ItemProperty::filterMediaExec()
{
    getListValues();

    if (rdbAuthor->isChecked() && lstAvailableAuthors->count() == 0)
        edtFilter->setStyleSheet(tr("background-color: rgb(255, 140, 140);"));

    if (rdbItems->isChecked() && lstAvailableArticles->count() == 0)
        edtFilter->setStyleSheet(tr("background-color: rgb(255, 140, 140);"));
}

void ItemProperty::emptyFilter()
{
    if (edtFilter->text().isEmpty()) getListValues();
    edtFilter->setStyleSheet(tr("background-color: rgb(255, 255, 255);"));

    rdbAuthor->setChecked(false);
    rdbItems->setChecked(false);
}

void ItemProperty::updateList()
{
    if (rdbAuthor->isChecked())
        getListWidgetValues(SQL_TitlesList.arg(itemId.getItem()).arg(itemId.getItem()), lstAvailableArticles);
    if (rdbItems->isChecked())
        getListWidgetValues(SQL_AuthorsList.arg(itemId.getItem()), lstAvailableAuthors);
}
