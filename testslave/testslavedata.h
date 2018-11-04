#ifndef TESTSLAVEDATA_H
#define TESTSLAVEDATA_H

#include <QObject>
#include <QMap>
#include <QModbusServer>

class TestSlaveData : public QObject
{
    Q_OBJECT
public:
    explicit TestSlaveData(quint32 registerCount = 100);
    ~TestSlaveData();

    uint size();

    void setRegisterState(uint registerAddress, bool bState);
    void setRegisterValue(uint registerAddress, quint16 value);

    bool registerState(uint registerAddress);
    quint16 registerValue(uint registerAddress);

private:

    typedef struct
    {
        bool bState;
        quint16 value;
    } registerData_t;

    QList<registerData_t> _registerList;

};

#endif // TESTSLAVEDATA_H
