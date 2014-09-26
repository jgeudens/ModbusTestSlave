
#include <QThread>
#include <QCoreApplication>
#include "workerThread.h"

WorkerThread::WorkerThread(QObject *parent) :
    QObject(parent),
    _thread(NULL)
{
    // NEVER create object with new here
}


WorkerThread::~WorkerThread()
{

}

void WorkerThread::StartThread()
{
    if(_thread == NULL)
    {
        _thread = new QThread();
        _thread->start();
        connect(_thread, SIGNAL(finished()), _thread, SLOT(deleteLater()));
        connect(_thread, SIGNAL(finished()), this, SLOT(Stopped()));
        moveToThread(_thread);
    }
}

void WorkerThread::Wait()
{
    if(_thread)
    {
        _thread->wait();
    }
}

void WorkerThread::StopThread()
{
    _thread->quit();
}

void WorkerThread::Stopped()
{
    /* thread is deleted using a connection between thread->finished and thread->deleteLater */
    _thread = NULL;

    emit ThreadStopped();
}



