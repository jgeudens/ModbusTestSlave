

#include <errno.h>
#include <iostream>


#ifndef _MSC_VER
/* POSIX library call: needed for close */
#include <unistd.h>
#endif

#include "modbusSlaveTCP.h"

using namespace std;

#include <fcntl.h>

/** Returns true on success, or false if there was an error */
int ModbusSlaveTCP::setSocketBlockingEnabled(int fd, bool blocking)
{
   if (fd < 0) return false;

#ifdef WIN32
   unsigned long mode = blocking ? 0 : 1;
   return ioctlsocket(fd, FIONBIO, &mode);
#else
   int flags = fcntl(fd, F_GETFL, 0);
   if (flags < 0) return -1;
   flags = blocking ? (flags&~O_NONBLOCK) : (flags|O_NONBLOCK);
   return fcntl(fd, F_SETFL, flags);
#endif
}


#if defined(_WIN32)
int ModbusSlaveTCP::modbus_tcp_deinit_win32(void)
{
    /* Clean up Windows socket API */
    if (WSACleanup() != 0)
    {
        fprintf(stderr, "WSACleanup() returned error code %d\n",
                    (unsigned int)WSAGetLastError());
        errno = EIO;
        return -1;
    }
    return 0;
}
#endif



ModbusSlaveTCP::ModbusSlaveTCP()
{
    _socket = -1;
    _ctx = NULL;
    _state = STATE_DISCONNECTED;
    _mb_mapping = NULL;

    _mb_mapping = modbus_mapping_new(0, 0, MODBUS_MAX_READ_REGISTERS, 0);

}


ModbusSlaveTCP::~ModbusSlaveTCP()
{
    modbus_mapping_free(_mb_mapping);
}


int32_t ModbusSlaveTCP::openPort(string ip, int32_t port)
{
    int32_t ret = -1;

    _ctx = modbus_new_tcp(ip.c_str(), port);

    if (_ctx != NULL)
    {
        modbus_set_debug(_ctx, true); /* Enable debug */

        _socket = modbus_tcp_listen(_ctx, 1); /* Accept only one connection */

        if (_socket != -1)
        {
            ret = setSocketBlockingEnabled(_socket, false);
            if (ret == -1)
            {
                cerr << "Socket (non-)blocking mode set failed" << endl;
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
        closePort();
    }

    return ret;

}


void ModbusSlaveTCP::setData(uint32_t index, uint16_t data)
{
    _mb_mapping->tab_registers[index] = data;
}

void ModbusSlaveTCP::doWork()
{

    switch(_state)
    {

    case STATE_DISCONNECTED:
        if (modbus_tcp_accept(_ctx, &_socket) != -1)
        {
            _state = STATE_CONNECTED;
        }
        else
        {
            //cerr << "Client connection not accepted: " << modbus_strerror(errno) << endl;
        }
        break;

    case STATE_CONNECTED:
        {
            const int rc = modbus_receive(_ctx, _query);
            if (rc > 0)
            {
                modbus_reply(_ctx, _query, rc, _mb_mapping);
            }
            else
            {
                modbus_close(_ctx);
                _state = STATE_DISCONNECTED;
            }
        }
        break;

   default:
        _state = STATE_DISCONNECTED;
        break;

    }
}


void ModbusSlaveTCP::closePort()
{
#if defined(_WIN32)
    modbus_tcp_deinit_win32();
#endif

    if (_socket != -1)
    {
        close(_socket);
    }    

    modbus_free(_ctx);
}

