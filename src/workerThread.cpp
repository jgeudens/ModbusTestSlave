
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

void WorkerThread::startThread()
{
    if(_thread == NULL)
    {
        _thread = new QThread();
        _thread->start();
        connect(_thread, SIGNAL(finished()), _thread, SLOT(deleteLater()));
        connect(_thread, SIGNAL(finished()), this, SLOT(stopped()));
        moveToThread(_thread);
    }
}

void WorkerThread::wait()
{
    if(_thread)
    {
        _thread->wait();
    }
}

void WorkerThread::stopThread()
{
    _thread->quit();
}

void WorkerThread::stopped()
{
    /* thread is deleted using a connection between thread->finished and thread->deleteLater */
    _thread = NULL;

    emit threadStopped();
}



