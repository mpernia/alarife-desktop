#ifndef WAIT_H
#define WAIT_H

#include <QObject>
#include <QTimer>

class Wait : public QObject
{
    Q_OBJECT
public:
    explicit Wait(int interval = 1000)
    {
        timer = new QTimer;
        timer->setInterval(interval);
        timer->setSingleShot(true);
        connect(timer, SIGNAL(timeout()),this, SLOT(stop()));
        timer->start();
    }

signals:
    void stopped();

private slots:
    void stop() { emit stopped(); }

private:
    QTimer *timer;
};

#endif // WAIT_H
