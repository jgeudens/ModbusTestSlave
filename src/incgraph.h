#ifndef INCGRAPH_H
#define INCGRAPH_H

#include <QObject>
#include <QTimer>
#include "testslavedata.h"

class IncGraph : public QObject
{
    Q_OBJECT
public:
    explicit IncGraph(TestSlaveData *pSlaveData, QObject *parent = nullptr);

    void setRegisters(uint32_t start, uint32_t count);
    void setState(bool bState);

private slots:
    void timerTick();

private:

    bool _bIncEnabled;
    QTimer _tickTimer;
    quint32 _periodMs;

    uint32_t _start;
    uint32_t _count;

    TestSlaveData *_pSlaveData;
};

#endif // INCGRAPH_H
