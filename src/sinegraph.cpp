#include "sinegraph.h"
#include <QtMath>

SineGraph::SineGraph(TestSlaveData *pSlaveData, QObject *parent)
    : QObject{parent}
{
    _currentTick = 0;
    _tickTime = 1;
    _periodMs = 1000;
    _pSlaveData = pSlaveData;

    _address = 10;

    connect(&_tickTimer, &QTimer::timeout, this, QOverload<>::of(&SineGraph::timerTick));
    _tickTimer.start(_tickTime);
}

void SineGraph::timerTick()
{
    _currentTick++;
    _currentTick = _currentTick >= _periodMs ? 0: _currentTick;

    const double angle = (2 * M_PI) * _currentTick / _periodMs;
    double value = sin(angle)
            + sin(3 * angle) / 3
            + sin(5 * angle) / 5
            + sin(7 * angle) / 7
            + sin(9 * angle) / 9
            + sin(11 * angle) / 11
            + sin(13 * angle) / 13;

    _pSlaveData->setRegisterValue(_address, value * 1000);

    _tickTimer.start(_tickTime);
}

void SineGraph::setPeriod(uint32_t period)
{
    _currentTick = 0;
    _periodMs = period;
}

void SineGraph::setRegister(uint32_t address)
{
    _address = address;
}
