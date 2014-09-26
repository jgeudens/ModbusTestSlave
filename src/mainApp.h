#ifndef MAIN_APP_H
#define MAIN_APP_H

#include <QtCore>


// forward declaration
class QTimer;
class PollQuit;
class ModbusSlaveTCP;

class MainApp : public QObject
{
    Q_OBJECT
    
public:
    MainApp();

public slots:
    void StartApplication();

signals:
    void finished();
    void CheckInput();
    void RequestApplicationStop();

    void QuitRequested();

private slots:
    void PollQuitStopped();
    void ModbusStopped();

    void QuitApplication();
    void UpdateData();

private:
    QTimer * _pollQuitTimer;
    PollQuit * _pollQuitThread;

    QTimer * _modbusTimer;
    ModbusSlaveTCP * _modbusThread;

    QTimer * _dataTimer;

};

#endif // !MAIN_APP_H
