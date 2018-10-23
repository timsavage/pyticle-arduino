#include "mqttsn_discovery.h"
#include "mqttsn_packet.h"

namespace MQTTSN
{

    /**
     * Unpack the Advertise message
     */
    uint8_t
    unpack_advertise(uint8_t* gateway_id, uint16_t* duration, uint8_t* buffer, uint16_t buffer_len)
    {
        uint8_t result = 0;    
        uint8_t* ptr = buffer;

        *gateway_id = read_uint8(&ptr);
        *duration = read_uint16(&ptr);

        return result;
    }

    /**
     * Pack the Search Gateway message
     */
    uint16_t
    pack_searchgw(uint8_t* buffer, uint16_t buffer_len, uint8_t radius)
    {
        uint16_t result = 0;
        uint8_t* ptr = buffer;

        if ((result = write_length(&ptr, buffer_len, 2)) == 0) {
            goto exit;
        }
        write_uint8(&ptr, MQTTSN_SEARCHGW);
        write_uint8(&ptr, radius);

    exit:
        return result;
    }

    /**
     * Unpack the Gateway Info message
     */
    uint8_t
    unpack_gwinfo(uint8_t* gateway_id, uint8_t* buffer, uint16_t buffer_len)
    {
        uint8_t result = 0;
        uint8_t* ptr = buffer;

        *gateway_id = read_uint8(&ptr);

        return result;
    }

} // !MQTTSN
