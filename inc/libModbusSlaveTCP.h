#ifndef LIBMODBUSSLAVETCP_H
#define LIBMODBUSSLAVETCP_H

#include <modbus.h>
#include "string"

class libModbusSlaveTCP
{
    public:
        libModbusSlaveTCP();
        ~libModbusSlaveTCP();

        int32_t Open(std::string ip, int32_t port);
        int32_t Work();
        void Close();

        void SetData(uint32_t index, uint16_t data);

    private:
        int s;
        modbus_t *ctx;
        modbus_mapping_t *mb_mapping;

        uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
};

#endif // LIBMODBUSSLAVETCP_H
