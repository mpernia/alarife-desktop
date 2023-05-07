#ifndef GALLERY_H
#define GALLERY_H

#include <QScrollArea>
#include<QtGui>

QT_BEGIN_NAMESPACE
class QString;
class QStringList;
class QFile;
class QFileInfo;
class QSqlQuery;
class QWidget;
class QLabel;
class QCheckBox;
class QToolButton;
class QLineEdit;
QT_END_NAMESPACE

#include "alarife.h"

class GalleryItem : public QWidget
{
Q_OBJECT
Q_PROPERTY(QString image READ image WRITE setImage)
Q_PROPERTY(QString title READ title WRITE setTitle)
public:
    explicit GalleryItem(QWidget *parent = 0, QString image = "", QString title = "");

protected:
     void mousePressEvent(QMouseEvent *event);

signals:
    void selected(QString);

public slots:
    QString image() const { return m_image; }
    QString title() const { return m_title; }
    void setImage(QString str = "") { m_image = str; }
    void setTitle(QString str = "") { m_title = str; }
    void setSelected();

private:
    QLabel *lblImage,
           *lblTitle;

    QCheckBox *chkTitle;

    QString m_image,
            m_title;
    bool m_select;
};


class Gallery : public QScrollArea
{
Q_OBJECT
Q_PROPERTY(int col READ col WRITE setCol)
Q_PROPERTY(QStringList selecction READ selecction)
public:
    explicit Gallery(QWidget *parent = 0);
    void setMediaPath(QString path) { m_mediaPath = path; }
    void prepare(QString str = "");
    void clear();
signals:

public slots:
    void selectAll();
    void clearAll();
    int col() const { return m_col; }
    void setCol(int c = 1) { m_col = c; }
    QStringList selecction() const { return m_selection; }

private slots:
    void selectedItems(QString);

private:
    QGridLayout *lytMain;
    QWidget *widget;

    QStringList m_selection;
    QString m_mediaPath;
    int m_col;

    QList<GalleryItem*>items;
    QString predicate;
};

#endif // QGALLERY_H
