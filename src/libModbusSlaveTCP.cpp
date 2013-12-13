

#include <errno.h>
#include <iostream>


#ifndef _MSC_VER
/* POSIX library call: needed for close */
#include <unistd.h>
#endif

#include "libModbusSlaveTCP.h"

using namespace std;


libModbusSlaveTCP::libModbusSlaveTCP()
{
    s = -1;
    ctx = NULL;
    mb_mapping = NULL;
}


libModbusSlaveTCP::~libModbusSlaveTCP()
{

}


int32_t libModbusSlaveTCP::Open(string ip, int32_t port)
{
    int32_t ret = -1;

    ctx = modbus_new_tcp(ip.c_str(), port);

    if (ctx != NULL)
    {
        modbus_set_debug(ctx, true); /* Enable debug */

        s = modbus_tcp_listen(ctx, 1); /* Accept only one connection */

        if (s != -1)
        {
            if (modbus_tcp_accept(ctx, &s) != -1)
            {
                mb_mapping = modbus_mapping_new(0, 0, MODBUS_MAX_READ_REGISTERS, 0);

                if (mb_mapping != NULL)
                {
                    ret = 0;
                }
                else
                {
                    cerr << "Failed to allocate the mapping: " << modbus_strerror(errno) << endl;
                }
            }
            else
            {
                cerr << "Client connection not accepted: " << modbus_strerror(errno) << endl;
            }

        }
        else
        {
            cerr << "Failed to create socket: " << modbus_strerror(errno) << endl;
        }
    }
    else
    {
        cerr << "Failed to create modbus context: " << modbus_strerror(errno) << endl;
    }

    if (ret == -1)
    {
        Close();
    }

    return ret;

}

int32_t libModbusSlaveTCP::Work()
{
    int32_t rc = -1;

    rc = modbus_receive(ctx, query);
    if (rc > 0)
    {
        modbus_reply(ctx, query, rc, mb_mapping);
    }

    return rc;
}


void libModbusSlaveTCP::Close()
{
    modbus_mapping_free(mb_mapping);
    if (s != -1)
    {
        close(s);
    }

    modbus_free(ctx);
}

