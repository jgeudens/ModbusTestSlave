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

    _pSlaveData = new TestSlaveData();
    _pSlaveModbus = new TestSlaveModbus(_pSlaveData);

    _pSlaveData->setRegisterState(0, true);
    _pSlaveData->setRegisterState(1, true);
    _pSlaveData->setRegisterState(2, true);
    _pSlaveData->setRegisterState(3, true);
    _pSlaveData->setRegisterState(4, true);

    _pRegisterStateModel = new RegisterStateModel(_pSlaveData);

    connect(_pUi->btnListen, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(_pUi->btnDisconnect, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);

    connect(_pSlaveModbus, &QModbusServer::stateChanged, this, &MainWindow::onStateChanged);
    connect(_pSlaveModbus, &QModbusServer::errorOccurred, this, &MainWindow::handleDeviceError);

    // Setup registerView
    _pUi->tableViewState->setModel(_pRegisterStateModel);
    _pUi->tableViewState->verticalHeader()->show();
    _pUi->tableViewState->horizontalHeader()->hide(); // Not sure yet (show or not to show?)

    // Reduce minimum size
    _pUi->tableViewState->horizontalHeader()->setMinimumSectionSize(10);

    /* Don't stretch columns, resize to contents */
    _pUi->tableViewState->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
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
    QUrl hostUrl;
    hostUrl.setPort(_pUi->spinSlavePort->value());
    hostUrl.setHost("127.0.0.1");

    if (_pSlaveModbus->connect(hostUrl, _pUi->spinSlaveId->value()))
    {
        _pUi->btnListen->setEnabled(false);
        _pUi->btnDisconnect->setEnabled(true);
    }
    else
    {
        qDebug() << "Connect failed: " << _pSlaveModbus->errorString();
    }
}

void MainWindow::onDisconnectClicked(void)
{
    _pSlaveModbus->disconnectDevice();
    _pUi->btnListen->setEnabled(true);
    _pUi->btnDisconnect->setEnabled(false);
}

void MainWindow::handleDeviceError(QModbusDevice::Error newError)
{
    if (newError == QModbusDevice::NoError || !_pSlaveModbus)
    {
        return;
    }

    qDebug() << "Error: " << _pSlaveModbus->errorString();
}

void MainWindow::onStateChanged(QModbusDevice::State state)
{
    qDebug() << "State: " << state;

    bool connected = (state != QModbusDevice::UnconnectedState);
    _pUi->btnListen->setEnabled(!connected);
    _pUi->spinSlaveId->setEnabled(!connected);
    _pUi->spinSlavePort->setEnabled(!connected);
    _pUi->btnDisconnect->setEnabled(connected);
}
