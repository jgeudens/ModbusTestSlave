#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QModbusTcpServer>
#include <QUrl>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _pUi(new Ui::MainWindow)
{
    _pUi->setupUi(this);

    connect(_pUi->btnConnect, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(_pUi->btnDisconnect, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);

    _pSlaveModbus = new TestSlaveModbus();
    _pSlaveData = new TestSlaveData(_pSlaveModbus);

    connect(_pSlaveModbus, &QModbusServer::stateChanged, this, &MainWindow::onStateChanged);
    connect(_pSlaveModbus, &QModbusServer::errorOccurred, this, &MainWindow::handleDeviceError);
}

MainWindow::~MainWindow()
{
    if (_pSlaveModbus)
    {
        _pSlaveModbus->disconnectDevice();
    }

    delete _pSlaveData;
    delete _pSlaveModbus;

    delete _pUi;
}

void MainWindow::onConnectClicked(void)
{
    qDebug() << "Connect clicked";

    const QUrl hostUrl = QUrl::fromUserInput("127.0.0.1:5002");
    if (!_pSlaveModbus->connect(hostUrl, 1))
    {
        qDebug() << "Connect failed: " << _pSlaveModbus->errorString();
    }
    else
    {
        _pUi->btnConnect->setEnabled(false);
        _pUi->btnDisconnect->setEnabled(true);
    }
}

void MainWindow::onDisconnectClicked(void)
{
    qDebug() << "Disconnect clicked";

    _pSlaveModbus->disconnectDevice();
    _pUi->btnConnect->setEnabled(true);
    _pUi->btnDisconnect->setEnabled(false);
}

void MainWindow::handleDeviceError(QModbusDevice::Error newError)
{
    if (newError == QModbusDevice::NoError || !_pSlaveModbus)
        return;

    qDebug() << _pSlaveModbus->errorString();
}

void MainWindow::onStateChanged(QModbusDevice::State state)
{
    bool connected = (state != QModbusDevice::UnconnectedState);
    _pUi->btnConnect->setEnabled(!connected);
    _pUi->btnDisconnect->setEnabled(connected);

    qDebug() << "State change: " << state;
}
