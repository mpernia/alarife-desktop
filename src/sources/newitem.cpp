#include <QSqlQuery>

#include "includes/newitem.h"
#include "ui_newitem.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QDateTime>
#include "includes/database.h"
#include "includes/sqlfunctions.h"


NewItem::NewItem(ItemId *i, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewItem)
{
    ui->setupUi(this);
    itemId = i;

    QSqlQuery *qry_0 = new QSqlQuery;

    qry_0->clear();
    qry_0->exec(QString("SELECT nombre FROM usuario"));
    while (qry_0->next()) ui->cmbUser->addItems(QStringList() << qry_0->value(0).toString());
    ui->cmbUser->setCurrentIndex(1);

    getAllValues();
    setUpdateInterface(false);

    qry_0->clear();
    qry_0->exec(QString("SELECT nombre FROM usuario WHERE id_usuario = %1").arg(itemId->getUser()));
    qry_0->first();
    ui->nombrePropietario->setText(qry_0->value(0).toString());

    delete qry_0;
}

NewItem::~NewItem()
{
    delete ui;
}

void NewItem::setUpdateInterface(bool b){
    ui->title->setReadOnly(b);
    ui->summary->setReadOnly(b);
    ui->keywords->setReadOnly(b);
    ui->source->setReadOnly(b);
}

void NewItem::getAllValues(){
    QSqlQuery *qry_1 = new QSqlQuery;
    qry_1->exec(QString("SELECT nombre, resumen, claves, fuente, fecha1, fecha2, usuario "
                      "FROM articulo WHERE id_articulo = %1").
              arg(itemId->getItem()));
    qry_1->first();
    ui->title->setText(qry_1->value(0).toString());
    ui->summary->setPlainText(qry_1->value(1).toString());
    ui->keywords->setPlainText(qry_1->value(2).toString());
    ui->source->setText(qry_1->value(3).toString());
    usuario = qry_1->value(6).toInt();

    ui->creationDate->setDateTime(QDateTime::currentDateTime());

    qry_1->clear();
    qry_1->exec(QString("SELECT DISTINCT articulo.nombre FROM articulo INNER JOIN "
                      "articuloarticulos ON articulo.id_articulo == articuloarticulos."
                      "IdArticuloLink WHERE articuloarticulos.articulo == %1 ORDER BY "
                      "articulo.id_articulo").arg(itemId->getItem()));
    while (qry_1->next()) new QListWidgetItem(qry_1->value(0).toString(), ui->items);

    qry_1->clear();
    qry_1->exec(QString("SELECT DISTINCT autor.nombre FROM autor INNER JOIN "
                      "articuloautores ON autor.id_autor == articuloautores.autor WHERE "
                      "articuloautores.articulo == %1 ORDER BY autor.id_autor").
              arg(itemId->getItem()));
    while (qry_1->next()) new QListWidgetItem(qry_1->value(0).toString(), ui->authors);

    qry_1->clear();
    qry_1->exec("SELECT nombre FROM articulo");
    while (qry_1->next())
        new QListWidgetItem(qry_1->value(0).toString(), ui->allItems);

    qry_1->clear();
    qry_1->exec("SELECT nombre FROM autor");
    while (qry_1->next())
        new QListWidgetItem(qry_1->value(0).toString(), ui->allAuthors);

    delete qry_1;
}

void NewItem::on_aceptButton_clicked()
{
    if(ui->title->text() == ""){
        QMessageBox::warning(this,"error",tr("Ponga el título del artículo"));
        return;
    }else if(ui->nombrePropietario->text() == "" ){
        QMessageBox::warning(this,"error",tr("Seleccione el propietario"));
        return;
    }
    
    QSqlQuery query;
    QString q = QString("SELECT id_usuario FROM usuario WHERE nombre = '%1'").arg(ui->nombrePropietario->text());
    query.exec(q);
    QString id_user;
    if(query.next())
        id_user = query.value(0).toString();

    itemId->setItem(sqlAutoIncremental("id_articulo", "articulo"));
    query.clear();
    q = QString("INSERT INTO articulo (id_articulo, nombre, resumen, claves, fuente, fecha1, fecha2, usuario, nivel, enciclopedia) "
                            "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10')"
                            ).arg(QString::number(itemId->getItem())).arg( ui->title->text()).arg(ui->summary->toPlainText()).arg(ui->keywords->toPlainText()).arg(ui->source->text())
                             .arg(ui->creationDate->text()).arg(ui->creationDate->text()).arg(id_user).arg(QString::number(itemId->getFather())).arg(QString::number(itemId->getEnciclopedia()));

    query.exec(q);

    insertNewAuthors();
    insertNewItems();

    itemId->setMode(ItemId::EDIT);
    emit insertado(*itemId);

    close();
}

void NewItem::insertNewAuthors()
{
    QSqlQuery query;
    QString autor;
    QString id_autor;
    QString q;
    for(int i =0; i < ui->authors->count(); i++){
        autor = ui->authors->item(i)->text();
        q = QString("SELECT id_autor FROM autor WHERE nombre='%1'").arg(autor);
        
        query.exec(q);
        query.first();
        id_autor = query.value(0).toString();
        
        query.exec(QString("INSERT INTO articuloautores(articulo,autor) VALUES(%1,%2)")
                   .arg(itemId->getItem()).arg(id_autor));
     }
}

void NewItem::insertNewItems()
{
    QSqlQuery query;
    QString item;
    QString id_item;
    QString q;
    for(int i =0; i < ui->items->count(); i++){
        item = ui->items->item(i)->text();
        q = QString("SELECT id_articulo FROM articulo WHERE nombre='%1'").arg(item);
        query.exec(q);
        query.first();
        id_item = query.value(0).toString();
        query.exec(QString("INSERT INTO articuloarticulos(articulo,IdArticuloLink) VALUES(%1,%2)")
                   .arg(itemId->getItem()).arg(id_item));
     }
}

void NewItem::on_insertItems_clicked()
{
    if(ui->allItems->currentItem() == NULL)
        return;

    ui->items->addItem(ui->allItems->currentItem()->text());
    delete ui->allItems->currentItem();
}

void NewItem::on_insertAuthors_clicked()
{
    if(ui->allAuthors->currentItem() == NULL)
        return;

    ui->authors->addItem(ui->allAuthors->currentItem()->text());
    delete ui->allAuthors->currentItem();
}

void NewItem::on_delAuthor_clicked()
{
    if(ui->authors->currentItem() == NULL)
        return;

    ui->allAuthors->addItem(ui->authors->currentItem()->text());
    delete ui->authors->currentItem();
}

void NewItem::on_delItems_clicked()
{
    if(ui->items->currentItem() == NULL)
        return;

    ui->allItems->addItem(ui->items->currentItem()->text());
    delete ui->items->currentItem();
}

void NewItem::on_cmbUser_currentIndexChanged(const QString &arg1)
{
    ui->nombrePropietario->setText(arg1);
}
