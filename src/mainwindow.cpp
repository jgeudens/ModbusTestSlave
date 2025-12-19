#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QDebug>
#include <QModbusTcpServer>
#include <QUrl>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), _pUi(new Ui::MainWindow)
{
    _pUi->setupUi(this);

    _pSlaveData = new TestSlaveData();
    _pSlaveModbus = new TestSlaveModbus(_pSlaveData);

    _pRegisterDataModel = new RegisterDataModel(_pSlaveData);

    _pIncGraph = new IncGraph(_pSlaveData);
    _pSineGraph = new SineGraph(_pSlaveData);

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

    connect(&_exceptionGroup, QOverload<int, bool>::of(&QButtonGroup::idToggled), this,
            [=, this](int id, bool checked) {
                if (checked)
                {
                    _pSlaveModbus->setException(static_cast<QModbusPdu::ExceptionCode>(id));
                }
            });

    /** Handle error recurrence group **/
    _bErrorOnce = true;
    _errorRecurrenceGroup.addButton(_pUi->optErrorOnce, true);
    _errorRecurrenceGroup.addButton(_pUi->optErrorPersistent, false);

    connect(&_errorRecurrenceGroup, QOverload<int>::of(&QButtonGroup::idClicked), this,
            [=, this](int id) { _bErrorOnce = static_cast<bool>(id); });

    connect(_pSlaveModbus, &TestSlaveModbus::requestProcessed, this, &MainWindow::handleRequestProcessed);

    /** Auto increment **/
    _bAutoInc = false;

    connect(_pUi->checkAutoIncrement, &QCheckBox::checkStateChanged, this, [this](Qt::CheckState state) {
        _bAutoInc = state;
        _pIncGraph->setState(_bAutoInc);
    });

    /** Setup registerView **/
    _pUi->tblRegData->setModel(_pRegisterDataModel);
    _pUi->tblRegData->verticalHeader()->show();
    _pUi->tblRegData->horizontalHeader()->show();
    _pUi->tblRegData->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QList<uint> registerList = QList<uint>() << 0 << 1 << 2 << 3 << 4;
    _pSlaveData->setRegisterState(registerList, true);

    _pIncGraph->setRegisters(0, 10);

    _pSlaveData->setRegisterState(10, true);
    _pSineGraph->setRegister(10);
    _pSineGraph->setPeriod(10000);

    QString windowCaption;
    windowCaption = QString("ModbusSim");

    setWindowTitle(windowCaption);
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

void MainWindow::handleRequestProcessed()
{
    if (_bErrorOnce)
    {
        _pUi->chkNone->click();
    }
}
