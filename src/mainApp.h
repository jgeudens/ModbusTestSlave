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
    void startApplication();

signals:
    void finished();
    void checkInput();
    void requestApplicationStop();

    void quitRequested();

private slots:
    void pollQuitStopped();
    void modbusStopped();

    void quitApplication();
    void updateData();

private:
    QTimer * _pollQuitTimer;
    PollQuit * _pollQuitThread;

    QTimer * _modbusTimer;
    ModbusSlaveTCP * _modbusThread;

    QTimer * _dataTimer;

    quint16 _data[5];

};

#endif // !MAIN_APP_H
