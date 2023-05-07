#ifndef MINEDITOR_H
#define MINEDITOR_H

#include <QWebView>

#include "alarife.h"

class MinEditor : public QWebView
{
    Q_OBJECT
public:
    explicit MinEditor(QWidget *parent = 0, Book *encyc = 0);

public slots:
    void saveData();
    void insertData(QString html);
    void insertMediaDlg();
    void setModified(bool m) { modified = m; }
    bool isModified() const { return modified; }
private slots:
    void isFirstTime();

private:
    QWebView* createWindow(QWebPage::WebWindowType);
    bool firstTime;
    bool modified;
    Book *encyclopedia;

signals:
    void loadFirstTime();
};

#endif // MINEDITOR_H
