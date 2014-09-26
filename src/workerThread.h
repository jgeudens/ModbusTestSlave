#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QObject>

class WorkerThread : public QObject
{
    Q_OBJECT
public:
    explicit WorkerThread(QObject *parent = 0);
    virtual ~WorkerThread();

    void startThread();
    void wait();

signals:
    void threadStopped();

public slots:
    virtual void doWork() = 0;
    void stopThread();

private slots:
    void stopped();
    
private:
    QThread * _thread;
};

#endif // !WORKERTHREAD_H
