
#include "../includes/gallerydlg.h"
#include "../includes/gallery.h"
#include "../includes/sqlfunctions.h"


GalleryDlg::GalleryDlg(QWidget *parent, QString path) : QDialog(parent)
{
    QWidget::setWindowModality(Qt::WindowModal);
    QWidget::setWindowFlags(__WIN_FLAG__);

    this->setWindowTitle(tr("Galeria de imagenes"));
    this->setMinimumSize(630, 400);

    imagePath = path;

    QVBoxLayout *lyt = new QVBoxLayout(this);

    edtFilter = new QLineEdit;
    btnFilter = new QToolButton;
    btnFilter->setIcon(QIcon(":/images/action_find.png"));
    btnFilter->setToolTip(tr("Buscar"));

    btnSelectAll = new QToolButton;
    btnClear = new QToolButton;

    btnSelectAll->setIcon(QIcon(":/images/result-ok.png"));
    btnClear->setIcon(QIcon(":/images/result-error.png"));

    btnSelectAll->setToolTip(tr("Marcar todas las imagenes"));
    btnClear->setToolTip(tr("Desmarcar todas las imagenes"));

    QLabel *lblFilter = new QLabel(tr("Buscar:"));

    QSpacerItem *hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QHBoxLayout *lytFilter = new QHBoxLayout();
    lytFilter->addWidget(lblFilter);
    lytFilter->addWidget(edtFilter);
    lytFilter->addWidget(btnFilter);
    lytFilter->addSpacerItem(hSpacer);
    lytFilter->addWidget(btnSelectAll);
    lytFilter->addWidget(btnClear);

    lyt->addLayout(lytFilter);

    gallery = new Gallery;
    gallery->setMediaPath(imagePath);
    gallery->setCol(3);
    gallery->prepare();

    lyt->addWidget(gallery);

    setLayout(lyt);

    connect(btnFilter, SIGNAL(clicked()), this, SLOT(filterItems()));
    connect(edtFilter,SIGNAL(textChanged(QString)),this, SLOT(emptyFilter()));
    connect(edtFilter, SIGNAL(returnPressed()), this, SLOT(filterItems()));
    connect(btnClear, SIGNAL(clicked()), gallery, SLOT(clearAll()));
    connect(btnSelectAll, SIGNAL(clicked()), gallery, SLOT(selectAll()));
}

GalleryDlg::~GalleryDlg()
{

}


void GalleryDlg::filterItems()
{
    if(edtFilter->text() != "")
    {
        gallery->clear();
        gallery->prepare(QString(" AND (adjuntos.descripcion LIKE '%%1%') ").arg(edtFilter->text()));
    }
}

void GalleryDlg::emptyFilter()
{
    if (edtFilter->text() == "")
    {
        gallery->clear();
        gallery->prepare();
        edtFilter->setStyleSheet(tr("background-color: rgb(255, 255, 255);"));
    }else
        edtFilter->setStyleSheet(tr("background-color: rgb(255, 255, 255);"));
}


