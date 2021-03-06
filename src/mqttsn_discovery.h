#include <stdint.h>

#ifndef _mqttsn_dicovery__h
#define _mqttsn_dicovery__h

namespace MQTTSN 
{

    /**
     * Advertise message
     */
    uint8_t
    unpack_advertise(uint8_t* gateway_id, uint16_t* duration, 
                     uint8_t* buffer, uint16_t buffer_len);

    /**
     * Search Gateway message
     */
    uint16_t
    pack_searchgw(uint8_t* buffer, uint16_t buffer_len, 
                  uint8_t radius=1);

    /**
     * Gateway Info message
     */
    uint8_t
    unpack_gwinfo(uint8_t* gateway_id, 
                  uint8_t* buffer, uint16_t buffer_len);

} //!MQTTSN

#endif //!_mqttsn_dicovery__h
