#include <stdint.h>

#ifndef _mqttsn_will__h
#define _mqttsn_will__h

namespace MQTTSN 
{

    /**
     * Will Topic message
     */
    uint16_t 
    pack_will_topic(uint8_t* buffer, uint16_t buffer_len, 
                    const char* will_topic, uint8_t qos, uint8_t retain);

    /**
     * Will Topic Update message
     */
    uint16_t 
    pack_will_topic_update(uint8_t* buffer, uint16_t buffer_len, 
                           uint8_t qos, uint8_t retain, const char* will_topic);

    /**
     * Will Message message
     */
    uint16_t 
    pack_will_message(uint8_t* buffer, uint16_t buffer_len, 
                      const char* will_msg);

    /**
     * Will Message Update message
     */
    uint16_t 
    pack_will_message_update(uint8_t* buffer, uint16_t buffer_len, 
                             const char* will_msg);

    /**
     * Will Topic/Message Response message
     */
    uint8_t
    unpack_will_response(uint8_t* return_code,
                         uint8_t* buffer, uint16_t buffer_len);

} //!MQTTSN

#endif //!_mqttsn_will__h
