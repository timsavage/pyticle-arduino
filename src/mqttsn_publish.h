#include <stdint.h>

#ifndef _mqttsn_publish__h
#define _mqttsn_publish__h

namespace MQTTSN 
{

    /**
     * Pack the Publish message
     */
    uint16_t 
    pack_publish(uint8_t* buffer, uint16_t buffer_len, 
                 uint8_t dup, uint8_t qos, uint16_t msg_id, const char* topid_name);

    /**
     * Unpack the Publish message
     */
    uint8_t
    unpack_publish(uint8_t* qos, uint16_t* topic_id, uint8_t* msg_id, uint8_t* return_code, 
                   uint8_t* buffer, uint16_t buffer_len);


} //!MQTTSN

#endif //!_mqttsn_publish__h
