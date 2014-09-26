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

        void SetData(uint32_t index, uint16_t data);
        int32_t Open(std::string ip, int32_t port);
        void Close();

    public slots:
        void DoWork();

    private:

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
