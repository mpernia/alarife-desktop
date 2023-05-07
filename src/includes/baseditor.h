#ifndef BASEDITOR_H
#define BASEDITOR_H

#include <QWebView>

#include "alarife.h"

class BasEditor : public QWebView
{
    Q_OBJECT
public:
    explicit BasEditor(QWidget *parent = 0, int id = 0, Book **encyc = 0);

public slots:
    void saveData();
    void setMediaPath(QString path) { mediaPath = path; }
    void insertData(QString html);
    void insertMediaDlg();
    void newData(int);
    void setModified(bool m) { modified = m; }
    bool isModified() const { return modified; }

private slots:
    void isFirstTime();

private:
    bool firstTime;
    bool modified;
    int index;
    QString mediaPath;
    Book *encyclopedia;

signals:
    void loadFirstTime();
};

#endif // BASEDITOR_H
