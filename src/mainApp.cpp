
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


void MainApp::StartApplication()
{
#ifdef QT_DEBUG_OUTPUT
    qDebug() << "Main App start";
#endif

    // Start thread for polling keyboard input
    connect(this, SIGNAL(RequestApplicationStop()), _pollQuitThread, SLOT(StopThread()));
    connect(_pollQuitThread, SIGNAL(ThreadStopped()), this, SLOT(PollQuitStopped()));
    connect(_pollQuitThread, SIGNAL(ThreadStopped()), _pollQuitThread, SLOT(deleteLater()));
    _pollQuitThread->StartThread();
    connect(_pollQuitThread, SIGNAL(QuitRequested()), this, SLOT(QuitApplication()));

    // Start thread for modbus slave TCP
    connect(this, SIGNAL(RequestApplicationStop()), _modbusThread, SLOT(StopThread()));
    connect(_modbusThread, SIGNAL(ThreadStopped()), this, SLOT(ModbusStopped()));
    connect(_modbusThread, SIGNAL(ThreadStopped()), _modbusThread, SLOT(deleteLater()));
    _modbusThread->StartThread();

    if (_modbusThread->Open("127.0.0.1", 1502) == -1)
    {
        //abort application
        emit QuitRequested();
    }

    // Start timer for polling keyboard input
    _pollQuitTimer->setInterval(1000);
    connect(_pollQuitTimer, SIGNAL(timeout()), _pollQuitThread, SLOT(DoWork()));
    _pollQuitTimer->start();

    // Start timer for modbus slave
    _modbusTimer->setInterval(10);
    connect(_modbusTimer, SIGNAL(timeout()), _modbusThread, SLOT(DoWork()));
    _modbusTimer->start();

    // Start timer for updating data
    _dataTimer->setInterval(1000);
    connect(_dataTimer, SIGNAL(timeout()), this, SLOT(UpdateData()));
    _dataTimer->start();

}

void MainApp::PollQuitStopped()
{
    _pollQuitThread = NULL;
}

void MainApp::ModbusStopped()
{
    _modbusThread = NULL;
}

void MainApp::UpdateData()
{
    for (quint32 i = 0; i < 5; i++)
    {
        _data[i]++;
        if (_data[i] > ((i + 1) * 10))
        {
            _data[i] = 0;
        }
        _modbusThread->SetData(i, _data[i]);
    }

}

void MainApp::QuitApplication()
{
    _pollQuitTimer->stop();
    _modbusTimer->stop();
    _dataTimer->stop();

    // Make sure modbus is closed
    _modbusThread->Close();

    emit RequestApplicationStop();

    if (_pollQuitThread)
    {
        _pollQuitThread->Wait();
    }

    if (_modbusThread)
    {

        _modbusThread->Wait();
    }

    delete _pollQuitTimer;
    delete _modbusTimer;
    delete _dataTimer;

    emit finished();

#ifdef QT_DEBUG_OUTPUT
    qDebug() << "Main App stop";
#endif
}

