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
                 uint8_t dup, uint8_t qos, uint8_t retain, uint16_t topic_id, uint16_t msg_id, const uint8_t* data, uint16_t data_len);

    /**
     * Unpack the Publish message
     */
    uint8_t 
    unpack_publish(Flags* flags, uint16_t* topic_id, uint16_t* msg_id, uint8_t** data, uint16_t* data_len,
                   uint8_t* buffer, uint16_t buffer_len);


    /**
     * Pack the Publish Acknowledge message 
     */
    uint16_t
    pack_puback(uint8_t* buffer, uint16_t buffer_len,
                uint16_t topic_id, uint16_t msg_id, uint8_t return_code);

    /**
     * Unpack the Publish Acknowledge message 
     */
    uint8_t
    unpack_puback(uint16_t* topic_id, uint16_t* msg_id, uint8_t* return_code, 
                  uint8_t* buffer, uint16_t buffer_len);


    /**
     * Pack Rec/Rel/Comp message
     */
    uint16_t
    pack_pub(uint8_t* buffer, uint16_t buffer_len, 
             uint16_t msg_id);

    /**
     * Unpack Rec/Rel/Comp message
     */
    uint8_t
    unpack_pub(uint16_t* msg_id,
               uint8_t* buffer, uint16_t buffer_len);

} //!MQTTSN

#endif //!_mqttsn_publish__h
