#include "testslavedata.h"
#include <QDebug>

TestSlaveData::TestSlaveData(QModbusServer *pModbusServer, quint32 registerCount) : QObject(nullptr)
{
    /* Save reference */
    _pModbusServer = pModbusServer;


    for(quint32 idx = 0u; idx < registerCount; idx++)
    {
        _registerList.append({false, 0});
    }
    QVector<registerData_t> _registerList;

    recreateModbusMap();

    connect(_pModbusServer, &QModbusServer::dataWritten, this, &TestSlaveData::onDataChanged);
}

TestSlaveData::~TestSlaveData()
{

}

int TestSlaveData::size()
{
    return _registerList.size();
}

void TestSlaveData::setRegisterState(int registerAddress, bool bState)
{
    if (registerAddress < _registerList.size())
    {
        if (_registerList[registerAddress].bState != bState)
        {
            _registerList[registerAddress].bState = bState;

            recreateModbusMap();
        }
    }
}

void TestSlaveData::setRegisterValue(int registerAddress, quint16 value)
{
    if (registerAddress < _registerList.size())
    {
        if (_registerList[registerAddress].value != value)
        {
            setValue(registerAddress, value);
        }
    }
}

bool TestSlaveData::registerState(int registerAddress)
{
    if (registerAddress < _registerList.size())
    {
        return _registerList[registerAddress].bState;
    }

    return false;
}

quint16 TestSlaveData::registerValue(int registerAddress)
{
    if (registerAddress < _registerList.size())
    {
        return _registerList[registerAddress].value;
    }

    return 0;
}

void TestSlaveData::onDataChanged(QModbusDataUnit::RegisterType reg, int address, int size)
{
    qDebug() << "reg: " << reg << ", addr: " << address << ", size: " << size;
}

void TestSlaveData::recreateModbusMap()
{
    QModbusDataUnitMap registerMap;

    for (int reg = 0; reg < static_cast<int>(size()); reg++)
    {
        if (_registerList[reg].bState)
        {
            QModbusDataUnit dataUnit(QModbusDataUnit::HoldingRegisters, reg, QVector<quint16>(_registerList[reg].value));

            registerMap.insert(QModbusDataUnit::HoldingRegisters, dataUnit);
        }
    }

    _pModbusServer->setMap(registerMap);
}

void TestSlaveData::setValue(int registerAddress, quint16 value)
{
    _registerList[registerAddress].value = value;

    QModbusDataUnit dataUnit(QModbusDataUnit::HoldingRegisters, registerAddress, QVector<quint16>(value));
    _pModbusServer->setData(dataUnit);
}
