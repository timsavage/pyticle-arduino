#include <stdint.h>

#ifndef _mqttsn_publish__h
#define _mqttsn_publish__h

namespace MQTTSN 
{

    /**
     * Publish message
     */
    uint16_t 
    pack_publish(uint8_t* buffer, uint16_t buffer_len, 
                 uint8_t dup, QosLevel qos, uint8_t retain, uint16_t topic_id, uint16_t msg_id, const void* data, uint16_t data_len);

    uint8_t 
    unpack_publish(Flags* flags, uint16_t* topic_id, uint16_t* msg_id, void** data, uint16_t* data_len,
                   uint8_t* buffer, uint16_t buffer_len);


    /**
     * Publish Acknowledge message 
     */
    uint16_t
    pack_puback(uint8_t* buffer, uint16_t buffer_len,
                uint16_t topic_id, uint16_t msg_id, ReturnCode return_code);

    uint8_t
    unpack_puback(uint16_t* topic_id, uint16_t* msg_id, ReturnCode* return_code, 
                  uint8_t* buffer, uint16_t buffer_len);


    /**
     * Rec/Rel/Comp message
     */
    uint16_t
    pack_pub(uint8_t* buffer, uint16_t buffer_len, 
             uint16_t msg_id);

    uint8_t
    unpack_pub(uint16_t* msg_id,
               uint8_t* buffer, uint16_t buffer_len);

} //!MQTTSN

#endif //!_mqttsn_publish__h
