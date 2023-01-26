#ifndef SINEGRAPH_H
#define SINEGRAPH_H

#include <QObject>
#include <QTimer>
#include "testslavedata.h"

class SineGraph : public QObject
{
    Q_OBJECT
public:
    explicit SineGraph(TestSlaveData *pSlaveData, QObject *parent = nullptr);

    void setPeriod(uint32_t period);
    void setRegister(uint32_t address);

signals:
    void valueUpdate();

private slots:
    void timerTick();

private:

    QTimer _tickTimer;
    quint32 _tickTime;
    quint32 _currentTick;
    quint32 _periodMs;

    uint32_t _address;
    TestSlaveData *_pSlaveData;

};

#endif // SINEGRAPH_H
