#include "incgraph.h"
#include <QtMath>

IncGraph::IncGraph(TestSlaveData *pSlaveData, QObject *parent)
    : QObject{parent}
{
    _pSlaveData = pSlaveData;
    _bIncEnabled = false;
    _periodMs = 1000;

    connect(&_tickTimer, &QTimer::timeout, this, QOverload<>::of(&IncGraph::timerTick));
    _tickTimer.start(_periodMs);
}

void IncGraph::setState(bool bState)
{
    _bIncEnabled = bState;
}

void IncGraph::setRegisters(uint32_t start, uint32_t count)
{
    _start = start;
    _count = count;
}

void IncGraph::timerTick()
{
    if (_bIncEnabled)
    {
        for(uint32_t idx = 0; idx < _count; idx++)
        {
            quint32 regAddr = _start + idx;
            quint16 value = _pSlaveData->registerValue(regAddr) + 1;
            _pSlaveData->setRegisterValue(regAddr, value);
        }
    }

    _tickTimer.start(_periodMs);
}
