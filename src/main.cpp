#include <iostream>

#include <errno.h>
#include <modbus.h>

#ifndef _MSC_VER
/* POSIX library call: needed for close */
#include <unistd.h>
#endif


#include <libModbusSlaveTCP.h>

using namespace std;

int main()
{

    cout << "Started" << endl;

    libModbusSlaveTCP slave;

    slave.Open("127.0.0.1", 1502);
    for(;;)
    {
        if (slave.Work() == -1)
        {
            break;
        }
    }

    slave.Close();

    cout << "Stopped" << endl;

    cin.get();
    return 0;
}
