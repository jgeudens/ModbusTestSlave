#include <iostream>

#include <errno.h>
#include <modbus.h>

#ifndef _MSC_VER
/* POSIX library call: needed for close */
#include <unistd.h>
#endif

#include <math.h>

#include <libModbusSlaveTCP.h>

using namespace std;


uint32_t GetMilliSeconds(void);


int main()
{

    cout << "Started" << endl;

    libModbusSlaveTCP slave;

    slave.Open("127.0.0.1", 1502);

    uint32_t startTime = GetMilliSeconds();
    uint16_t data = 0;
    slave.SetData(0, data);
    slave.SetData(1, -1 * data);

    for(;;)
    {

        if ((GetMilliSeconds() - startTime) > 500u) /* Update every 500 ms */
        {
            startTime = GetMilliSeconds();

            data++;
            if (data > 20)
            {
                data = 0;
            }
        }

        slave.SetData(0, data);
        slave.SetData(1, -1 * data);

        slave.Work();

    }

    slave.Close();

    cout << "Stopped" << endl;

    cin.get();
    return 0;
}


uint32_t GetMilliSeconds(void)
{
    uint32_t ms; // Milliseconds
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    const time_t s  = spec.tv_sec;
    ms = (s * 1000) + round(spec.tv_nsec / 1.0e6);

    return ms;
}
