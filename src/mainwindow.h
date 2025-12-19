#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>

#include "testslavemodbus.h"
#include "testslavedata.h"

#include "registerdatamodel.h"
#include "incgraph.h"
#include "sinegraph.h"

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

    void handleRequestProcessed();

private:

    Ui::MainWindow *_pUi;

    bool _bAutoInc;

    bool _bErrorOnce;

    IncGraph* _pIncGraph;
    SineGraph* _pSineGraph;

    QButtonGroup _exceptionGroup;
    QButtonGroup _errorRecurrenceGroup;

    TestSlaveModbus *_pSlaveModbus;
    TestSlaveData *_pSlaveData;

    RegisterDataModel *_pRegisterDataModel;
};

#endif // MAINWINDOW_H
