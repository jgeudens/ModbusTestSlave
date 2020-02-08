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

    QList<uint> registerList = QList<uint>() << 0 << 1 << 2 << 3 << 4;
    _pSlaveData->setRegisterState(registerList, true);

    _pRegisterDataModel = new RegisterDataModel(_pSlaveData);

    connect(_pSlaveData, &TestSlaveData::dataChanged, _pRegisterDataModel, &RegisterDataModel::handleDataChange);

    connect(_pUi->btnListen, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(_pUi->btnDisconnect, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);

    connect(_pSlaveModbus, &QModbusServer::stateChanged, this, &MainWindow::onStateChanged);
    connect(_pSlaveModbus, &QModbusServer::errorOccurred, this, &MainWindow::handleDeviceError);

    /** Set exception group **/
    _exceptionGroup.addButton(_pUi->chkNone, 0);
    _exceptionGroup.addButton(_pUi->chkIllegalFunction, QModbusPdu::IllegalFunction);
    _exceptionGroup.addButton(_pUi->chkIllegalDataAddress, QModbusPdu::IllegalDataAddress);
    _exceptionGroup.addButton(_pUi->chkIllegalDataValue, QModbusPdu::IllegalDataValue);
    _exceptionGroup.addButton(_pUi->chkTargetNoResponse, QModbusPdu::GatewayTargetDeviceFailedToRespond);
    _exceptionGroup.addButton(_pUi->chkGatewayPathUnavailable, QModbusPdu::GatewayPathUnavailable);

    connect(&_exceptionGroup, QOverload<int, bool>::of(&QButtonGroup::buttonToggled),
        [=](int id, bool checked){
            if (checked)
            {
                _pSlaveModbus->setException(static_cast<QModbusPdu::ExceptionCode>(id));
            }
        });

    /** Handle error recurrence group **/
    _bErrorOnce = true;
    _errorRecurrenceGroup.addButton(_pUi->optErrorOnce, true);
    _errorRecurrenceGroup.addButton(_pUi->optErrorPersistent, false);

    connect(&_errorRecurrenceGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
        [=](int id){
        _bErrorOnce = static_cast<bool>(id);
        });

    connect(_pSlaveModbus, &TestSlaveModbus::requestProcessed, this, &MainWindow::handleRequestProcessed);

    /** Auto increment **/
    connect(&_autoIncTimer, &QTimer::timeout, this, &MainWindow::handleAutoIncTick);
    _autoIncTimer.start(1000);
    _bAutoInc = false;
    connect(_pUi->checkAutoIncrement, &QCheckBox::stateChanged,
        [=](int state){
            _bAutoInc = (state == Qt::Checked);
        });

    /** Setup registerView **/
    _pUi->tblRegData->setModel(_pRegisterDataModel);
    _pUi->tblRegData->verticalHeader()->show();
    _pUi->tblRegData->horizontalHeader()->show(); // Not sure yet (show or not to show?)

    /* Don't stretch columns, resize to contents */
    _pUi->tblRegData->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QString windowCaption;
    windowCaption = QString("ModbusTestSlave");

    setWindowTitle(windowCaption);
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

void MainWindow::handleAutoIncTick()
{
    if (_bAutoInc)
    {
        _pSlaveData->incrementAllEnabledRegisters();
    }
}

void MainWindow::handleRequestProcessed()
{
    if (_bErrorOnce)
    {
        _pUi->chkNone->animateClick(Qt::Checked);
    }
}
