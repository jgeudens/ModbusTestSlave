#ifndef MODBUSSLAVETCP_H
#define MODBUSSLAVETCP_H

#include <modbus.h>
#include "workerThread.h"
#include "string"

class ModbusSlaveTCP : public WorkerThread
{
    Q_OBJECT

    public:
        ModbusSlaveTCP();
        ~ModbusSlaveTCP();

        void setData(uint32_t index, uint16_t data);
        int32_t openPort(std::string ip, int32_t port);
        void closePort();

    public slots:
        void doWork();

    private:

        int setSocketBlockingEnabled(int fd, bool blocking);
#if defined(_WIN32)
        int modbus_tcp_deinit_win32(void);
#endif

        typedef enum
        {
            STATE_CONNECTED,
            STATE_DISCONNECTED
        } ConnectionState;

        int _socket;
        modbus_t * _ctx;
        modbus_mapping_t * _mb_mapping;

        ConnectionState _state;

        uint8_t _query[MODBUS_TCP_MAX_ADU_LENGTH];
};

#endif // MODBUSSLAVETCP_H
