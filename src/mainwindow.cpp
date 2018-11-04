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

    _pRegisterDataModel = new RegisterDataModel(_pSlaveData);

    connect(_pSlaveData, &TestSlaveData::dataChanged, _pRegisterDataModel, &RegisterDataModel::handleDataChange);

    connect(_pUi->btnListen, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(_pUi->btnDisconnect, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);

    connect(_pSlaveModbus, &QModbusServer::stateChanged, this, &MainWindow::onStateChanged);
    connect(_pSlaveModbus, &QModbusServer::errorOccurred, this, &MainWindow::handleDeviceError);

    /*** Auto increment ***/
    connect(&_autoIncTimer, &QTimer::timeout, this, &MainWindow::handleAutoIncTick);
    _autoIncTimer.start(1000);
    bAutoInc = false;
    connect(_pUi->checkAutoIncrement, &QCheckBox::stateChanged, this, &MainWindow::handleAutoIncChanged);

    /*** Setup registerView **/
    _pUi->tblRegData->setModel(_pRegisterDataModel);
    _pUi->tblRegData->verticalHeader()->show();
    _pUi->tblRegData->horizontalHeader()->show(); // Not sure yet (show or not to show?)

    /* Don't stretch columns, resize to contents */
    _pUi->tblRegData->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

MainWindow::~MainWindow()
{
    _autoIncTimer.stop();

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

void MainWindow::handleAutoIncChanged(int state)
{
    if (state == Qt::Checked)
    {
        bAutoInc = true;
    }
    else
    {
        bAutoInc = false;
    }
}

void MainWindow::handleAutoIncTick()
{
    if (bAutoInc)
    {
        _pSlaveData->incrementAllEnabledRegisters();
    }
}
