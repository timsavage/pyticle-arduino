#include <stdint.h>

#ifndef _mqttsn_subscription__h
#define _mqttsn_subscription__h

namespace MQTTSN 
{

    /**
     * Register message
     */
    uint16_t 
    pack_register(uint8_t* buffer, uint16_t buffer_len, 
                  uint16_t topic_id, uint16_t msg_id, const char* topic_name);

    uint8_t 
    unpack_register(uint16_t* topic_id, uint16_t* msg_id, char** topic_name, uint16_t* topic_len,
                    uint8_t* buffer, uint16_t buffer_len);


    /**
     * Register Acknowledge message
     */
    uint8_t
    unpack_regack(QosLevel* qos, uint16_t* topic_id, uint8_t* msg_id, ReturnCode* return_code, 
                  uint8_t* buffer, uint16_t buffer_len);


    /**
     * Subscribe message
     */
    uint16_t 
    pack_subscribe(uint8_t* buffer, uint16_t buffer_len, 
                   uint8_t dup, QosLevel qos, uint16_t msg_id, const char* topic_name);

    uint16_t 
    pack_subscribe(uint8_t* buffer, uint16_t buffer_len, 
                   uint8_t dup, QosLevel qos, uint16_t msg_id, uint16_t topic_id);

    uint8_t
    unpack_suback(QosLevel* qos, uint16_t* topic_id, uint8_t* msg_id, ReturnCode* return_code, 
                  uint8_t* buffer, uint16_t buffer_len);


    /**
     * UnSubscribe message
     */
    uint16_t
    pack_unsubscribe(uint8_t* buffer, uint16_t buffer_len, 
                     uint16_t msg_id, const char* topic_name);

    uint16_t
    pack_unsubscribe(uint8_t* buffer, uint16_t buffer_len, 
                     uint16_t msg_id, uint16_t topic_id);

    /**
     * UnSubscribe Acknowledge message
     */
    uint8_t
    unpack_unsuback(uint16_t* msg_id, 
                    uint8_t* buffer, uint16_t buffer_len);

} //!MQTTSN

#endif //!_mqttsn_subscription__h
