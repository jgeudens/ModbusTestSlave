#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QButtonGroup>

#include "testslavemodbus.h"
#include "testslavedata.h"

#include "registerdatamodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void onConnectClicked(void);
    void onDisconnectClicked(void);

    void handleDeviceError(QModbusDevice::Error newError);
    void onStateChanged(QModbusDevice::State state);

    void handleAutoIncTick();
    void handleRequestProcessed();

private:

    Ui::MainWindow *_pUi;

    QTimer _autoIncTimer;
    bool _bAutoInc;

    bool _bErrorOnce;

    QButtonGroup _exceptionGroup;
    QButtonGroup _errorRecurrenceGroup;

    TestSlaveModbus *_pSlaveModbus;
    TestSlaveData *_pSlaveData;

    RegisterDataModel *_pRegisterDataModel;
};

#endif // MAINWINDOW_H
