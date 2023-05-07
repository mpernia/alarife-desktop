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

/*
colores  R     G     B       HTML
1        253   255   189     fdffbd
2        201   247   247     c9f7f7
3        0     0     179     0000b3
4        0     204   255     00ccff
5        51    102   153     336699
6        0     0     255     0000ff
7        246   105   0       f66900
8        191   85    4       bf5504
9        235   242   22      ebf216
*/
#include <QtGui>

#include "../includes/stadistic.h"

Stadistic::Stadistic(QWidget *parent, Book *encyc) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    setMaximumSize(520,360);
    setMinimumSize(520,360);

    color1 = "#fdffbd";
    color2 = "#ebf216";
    color3 = "#c9f7f7";
    color4 = "#0000b3";
    color5 = "#00ccff";
    color6 = "#336699";
    color7 = "#0000ff";
    color8 = "#f66900";
    color9 = "#bf5504";

    encyclopedia = encyc;

    extImagen = encyclopedia->getExtImagen();
    extSonido = encyclopedia->getExtSonido();
    extVideo = encyclopedia->getExtVideo();
    extXMedia = encyclopedia->getExtXMedia();
    qDebug() << "audio:" << extSonido << " image:" << extImagen
             << " video:" << extVideo << "xmedia:"<< extXMedia
             << endl;

    clearFilter();

    setWindowTitle(tr("Estadística"));

    model = new QStandardItemModel(8, 2, this);
    pieChart = new Diagram(this);

    pieChart->setMaximumWidth(500);
    pieChart->setModel(model);

    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    pieChart->setSelectionModel(selectionModel);

    QVBoxLayout *lytDiagram = new QVBoxLayout;
    lytDiagram->addWidget(pieChart);
    setLayout(lytDiagram);

    model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());

    int row = 0;

    qryTmp = new QSqlQuery;

    qryTmp->exec(QString("SELECT COUNT(id_articulo) FROM articulo "
                         "WHERE enciclopedia = %1").arg(encyclopedia->getIndex()));
    qryTmp->first();
    model->insertRows(row, 1, QModelIndex());
    model->setData(model->index(row, 0, QModelIndex()), tr(" Artículos - [")+qryTmp->value(0).toString()+"]");
    model->setData(model->index(row, 1, QModelIndex()), qryTmp->value(0).toString());
    model->setData(model->index(row, 0, QModelIndex()), QColor(color1), Qt::DecorationRole);
    delete qryTmp;
    row++;

    qryTmp = new QSqlQuery;
    qryTmp->exec(QString("SELECT COUNT(DISTINCT id_autor) FROM autor INNER JOIN "
                         "articuloautores ON articuloautores.autor = autor.id_autor"));
    qryTmp->first();
    model->insertRows(row, 1, QModelIndex());
    model->setData(model->index(row, 0, QModelIndex()), tr(" Autores - [")+qryTmp->value(0).toString()+"]");
    model->setData(model->index(row, 1, QModelIndex()), qryTmp->value(0).toString());
    model->setData(model->index(row, 0, QModelIndex()), QColor(color2), Qt::DecorationRole);
    delete qryTmp;
    row++;

    qryTmp = new QSqlQuery;
    qryTmp->exec(QString("SELECT COUNT(id_autor) FROM autor INNER JOIN enc_autor ON "
                         "enc_autor.enciclopedia = %1 AND enc_autor.autor = autor.id_autor "
                         "WHERE enc_autor.colaborador = 1").arg(encyclopedia->getIndex()));
    qryTmp->first();
    model->insertRows(row, 1, QModelIndex());
    model->setData(model->index(row, 0, QModelIndex()), tr(" Colaboradores - [")+qryTmp->value(0).toString()+"]");
    model->setData(model->index(row, 1, QModelIndex()), qryTmp->value(0).toString());
    model->setData(model->index(row, 0, QModelIndex()), QColor(color3), Qt::DecorationRole);
    delete qryTmp;
    row++;

    qryTmp = new QSqlQuery;
    qryTmp->exec(QString("SELECT COUNT(id_autor) FROM autor INNER JOIN enc_autor ON "
                         "enc_autor.enciclopedia = %1 AND enc_autor.autor = autor.id_autor "
                         "WHERE enc_autor.equipo = 1").arg(encyclopedia->getIndex()));
    qryTmp->first();
    model->insertRows(row, 1, QModelIndex());
    model->setData(model->index(row, 0, QModelIndex()), tr(" Equipo - [")+qryTmp->value(0).toString()+"]");
    model->setData(model->index(row, 1, QModelIndex()), qryTmp->value(0).toString());
    model->setData(model->index(row, 0, QModelIndex()), QColor(color4), Qt::DecorationRole);
    delete qryTmp;
    row++;

    qryTmp = new QSqlQuery;
    qryTmp->exec(QString("SELECT count(id_adjunto) FROM adjuntos WHERE "
                         "id_adjunto > 0 AND  (%1')").arg(extImagen));

    qryTmp->first();
    model->insertRows(row, 1, QModelIndex());
    model->setData(model->index(row, 0, QModelIndex()), tr(" Imágenes - [")+qryTmp->value(0).toString()+"]");
    model->setData(model->index(row, 1, QModelIndex()), qryTmp->value(0).toString());
    model->setData(model->index(row, 0, QModelIndex()), QColor(color5), Qt::DecorationRole);
    delete qryTmp;
    row++;

    qryTmp = new QSqlQuery;
    qryTmp->exec(QString("SELECT COUNT(id_nivel) FROM nivel "
                         "WHERE enciclopedia = %1").arg(encyclopedia->getIndex()));
    qryTmp->first();
    model->insertRows(row, 1, QModelIndex());
    model->setData(model->index(row, 0, QModelIndex()), tr(" Temas - [")+qryTmp->value(0).toString()+"]");
    model->setData(model->index(row, 1, QModelIndex()), qryTmp->value(0).toString());
    model->setData(model->index(row, 0, QModelIndex()), QColor(color6), Qt::DecorationRole);
    delete qryTmp;
    row++;

    qryTmp = new QSqlQuery;
    qryTmp->exec(QString("SELECT count(id_adjunto) FROM adjuntos WHERE %1'").arg(extSonido));
    qryTmp->first();
    model->insertRows(row, 1, QModelIndex());
    model->setData(model->index(row, 0, QModelIndex()), tr(" Sonido - [")+qryTmp->value(0).toString()+"]");
    model->setData(model->index(row, 1, QModelIndex()), qryTmp->value(0).toString());
    model->setData(model->index(row, 0, QModelIndex()), QColor(color7), Qt::DecorationRole);
    delete qryTmp;
    row++;

    qryTmp = new QSqlQuery;
    qryTmp->exec(QString("SELECT count(id_adjunto) FROM adjuntos WHERE %1'").arg(extVideo));
    qryTmp->first();
    model->insertRows(row, 1, QModelIndex());
    model->setData(model->index(row, 0, QModelIndex()), tr(" Videos - [")+qryTmp->value(0).toString()+"]");
    model->setData(model->index(row, 1, QModelIndex()), qryTmp->value(0).toString());
    model->setData(model->index(row, 0, QModelIndex()), QColor(color8), Qt::DecorationRole);
    delete qryTmp;
    row++;

    qryTmp = new QSqlQuery;
    qryTmp->exec(QString("SELECT count(id_adjunto) FROM adjuntos WHERE %1'").arg(extXMedia));
    qryTmp->first();
    model->insertRows(row, 1, QModelIndex());
    model->setData(model->index(row, 0, QModelIndex()), tr(" X-medias - [")+qryTmp->value(0).toString()+"]");
    model->setData(model->index(row, 1, QModelIndex()), qryTmp->value(0).toString());
    model->setData(model->index(row, 0, QModelIndex()), QColor(color9), Qt::DecorationRole);
    delete qryTmp;
}

void Stadistic::clearFilter()
{
    extImagen.replace("*.","extension = '");
    extImagen.replace(";","' OR ");
    extSonido.replace("*.","extension = '");
    extSonido.replace(";","' OR ");
    extVideo.replace("*.","extension = '");
    extVideo.replace(";","' OR ");
    extXMedia.replace("*.","extension = '");
    extXMedia.replace(";","' OR ");
}

