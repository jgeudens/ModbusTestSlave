
#include <errno.h>
#include "mainApp.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MainApp *myApp = new MainApp();

    // This will cause the application to exit when
    // the myApp signals finished.    
    QObject::connect(myApp, SIGNAL(finished()), &a, SLOT(quit()));

    // This will run the myApp from the application event loop.
    QTimer::singleShot(0, myApp, SLOT(StartApplication()));

    return a.exec();
}
