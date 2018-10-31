#ifndef TESTSLAVEDATA_H
#define TESTSLAVEDATA_H

#include <QObject>
#include <QMap>
#include <QModbusServer>

class TestSlaveData : public QObject
{
    Q_OBJECT
public:
    explicit TestSlaveData(QModbusServer *pModbusServer, quint32 registerCount);
    ~TestSlaveData();

    int size();

    void setRegisterState(int registerAddress, bool bState);
    void setRegisterValue(int registerAddress, quint16 value);

    bool registerState(int registerAddress);
    quint16 registerValue(int registerAddress);

signals:

public slots:

private slots:
    void onDataChanged(QModbusDataUnit::RegisterType reg, int address, int size);

private:

    typedef struct
    {
        bool bState;
        quint16 value;
    } registerData_t;

    void recreateModbusMap();
    void setValue(int registerAddress, quint16 value);

    QModbusServer *_pModbusServer;

    QVector<registerData_t> _registerList;

};

#endif // TESTSLAVEDATA_H
