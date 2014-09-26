#ifndef POLLQUIT_H
#define POLLQUIT_H


#include "workerThread.h"

class PollQuit : public WorkerThread
{
    Q_OBJECT

public slots:
    void DoWork();

signals:
    void QuitRequested();

private:
    int init_win32();
    int deinit_win32();
   
};

#endif // !POLLQUIT_H
