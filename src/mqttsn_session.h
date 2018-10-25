#include <stdint.h>

#ifndef _mqttsn_session__h
#define _mqttsn_session__h

namespace MQTTSN 
{

    /**
     * Pack the Connect message
     */
    uint16_t
    pack_connect(uint8_t* buffer, uint16_t buffer_len, 
                 uint8_t will, uint8_t clean_session, uint16_t duration, const char* client_id);

    /**
     * Unpack the Connect Acknowledge message
     */
    uint8_t
    unpack_connack(uint8_t* return_code, 
                   uint8_t* buffer, uint16_t buffer_len);

    /**
     * Pack the Connect message
     */
    uint16_t
    pack_disconnect(uint8_t* buffer, uint16_t buffer_len, 
                    uint16_t duration);

    /**
     * Pack the Ping Request message
     */
    uint16_t
    pack_pingreq(uint8_t* buffer, uint16_t buffer_len, 
                 const char* client_id);

    /**
     * Unpack the Ping Response message
     */
    uint8_t
    unpack_pingresp(uint8_t* buffer, uint16_t buffer_len);

} //!MQTTSN

#endif //!_mqttsn_session__h
