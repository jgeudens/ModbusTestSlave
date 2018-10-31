#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "testslavemodbus.h"
#include "testslavedata.h"

#include "registerstatemodel.h"

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

private:

    Ui::MainWindow *_pUi;

    TestSlaveModbus *_pSlaveModbus;
    TestSlaveData *_pSlaveData;

    RegisterStateModel *_pRegisterStateModel;
};

#endif // MAINWINDOW_H
