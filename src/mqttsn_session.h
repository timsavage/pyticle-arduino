#include <stdint.h>

#ifndef _mqttsn_session__h
#define _mqttsn_session__h

namespace MQTTSN 
{

    /**
     * Connect message
     */
    uint16_t
    pack_connect(uint8_t* buffer, uint16_t buffer_len, 
                 uint8_t will, uint8_t clean_session, uint16_t duration, const char* client_id);

    /**
     * Connect Acknowledge message
     */
    uint8_t
    unpack_connack(ReturnCode* return_code, 
                   uint8_t* buffer, uint16_t buffer_len);

    /**
     * Connect message
     */
    uint16_t
    pack_disconnect(uint8_t* buffer, uint16_t buffer_len, 
                    uint16_t duration);

    /**
     * Ping Request message
     */
    uint16_t
    pack_pingreq(uint8_t* buffer, uint16_t buffer_len, 
                 const char* client_id);

} //!MQTTSN

#endif //!_mqttsn_session__h
