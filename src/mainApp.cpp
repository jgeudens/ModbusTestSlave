
#include "mainApp.h"
#include "pollQuit.h"
#include "modbusSlaveTCP.h"
#include "QTimer"
#include <QCoreApplication>


MainApp::MainApp()
{
    _pollQuitThread = new PollQuit();
    _pollQuitTimer = new QTimer();

    _modbusThread = new ModbusSlaveTCP();
    _modbusTimer = new QTimer();

    _dataTimer = new QTimer();
}


void MainApp::startApplication()
{
#ifdef QT_DEBUG_OUTPUT
    qDebug() << "Main App start";
#endif

    // Start thread for polling keyboard input
    connect(this, SIGNAL(requestApplicationStop()), _pollQuitThread, SLOT(stopThread()));
    connect(_pollQuitThread, SIGNAL(threadStopped()), this, SLOT(pollQuitStopped()));
    connect(_pollQuitThread, SIGNAL(threadStopped()), _pollQuitThread, SLOT(deleteLater()));
    _pollQuitThread->startThread();
    connect(_pollQuitThread, SIGNAL(quitRequested()), this, SLOT(quitApplication()));

    // Start thread for modbus slave TCP
    connect(this, SIGNAL(requestApplicationStop()), _modbusThread, SLOT(stopThread()));
    connect(_modbusThread, SIGNAL(threadStopped()), this, SLOT(modbusStopped()));
    connect(_modbusThread, SIGNAL(threadStopped()), _modbusThread, SLOT(deleteLater()));
    _modbusThread->startThread();

    if (_modbusThread->openPort("127.0.0.1", 1502) == -1)
    {
        //abort application
        emit quitRequested();
    }

    // Start timer for polling keyboard input
    _pollQuitTimer->setInterval(1000);
    connect(_pollQuitTimer, SIGNAL(timeout()), _pollQuitThread, SLOT(doWork()));
    _pollQuitTimer->start();

    // Start timer for modbus slave
    _modbusTimer->setInterval(10);
    connect(_modbusTimer, SIGNAL(timeout()), _modbusThread, SLOT(doWork()));
    _modbusTimer->start();

    // Start timer for updating data
    _dataTimer->setInterval(1000);
    connect(_dataTimer, SIGNAL(timeout()), this, SLOT(updateData()));
    _dataTimer->start();

}

void MainApp::pollQuitStopped()
{
    _pollQuitThread = NULL;
}

void MainApp::modbusStopped()
{
    _modbusThread = NULL;
}

void MainApp::updateData()
{
    for (quint32 i = 0; i < 5; i++)
    {
        _data[i]++;
        if (_data[i] > ((i + 1) * 10))
        {
            _data[i] = 0;
        }
        _modbusThread->setData(i, _data[i]);
    }

}

void MainApp::quitApplication()
{
    _pollQuitTimer->stop();
    _modbusTimer->stop();
    _dataTimer->stop();

    // Make sure modbus is closed
    _modbusThread->closePort();

    emit requestApplicationStop();

    if (_pollQuitThread)
    {
        _pollQuitThread->wait();
    }

    if (_modbusThread)
    {

        _modbusThread->wait();
    }

    delete _pollQuitTimer;
    delete _modbusTimer;
    delete _dataTimer;

    emit finished();

#ifdef QT_DEBUG_OUTPUT
    qDebug() << "Main App stop";
#endif
}

