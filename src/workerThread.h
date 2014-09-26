#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QObject>

class WorkerThread : public QObject
{
    Q_OBJECT
public:
    explicit WorkerThread(QObject *parent = 0);
    virtual ~WorkerThread();

    void StartThread();
    void Wait();

signals:
    void ThreadStopped();

public slots:
    virtual void DoWork() = 0;
    void StopThread();

private slots:
    void Stopped();
    
private:
    QThread * _thread;
};

#endif // !WORKERTHREAD_H
