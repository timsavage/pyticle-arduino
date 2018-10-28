#include <string.h>

#include "mqttsn_packet.h"
#include "mqttsn_subscription.h"

namespace MQTTSN
{

    /**
     * Pack the Register message
     */
    uint16_t 
    pack_register(uint8_t* buffer, uint16_t buffer_len, 
                  uint16_t topic_id, uint16_t msg_id, const char* topic_name)
    {
        uint16_t result = 0;
        uint8_t* ptr = buffer;
        uint16_t topic_name_len = strlen(topic_name);

        if ((result = write_length(&ptr, buffer_len, 4 + topic_name_len)) == 0) {
            goto exit;
        }
        write_uint8(&ptr, SUBSCRIBE);
        write_uint16(&ptr, topic_id);
        write_uint16(&ptr, msg_id);
        write_string(&ptr, topic_name, topic_name_len);

    exit:
        return result;
    }

    /**
     * Unpack the Register Acknowledge message
     */
    uint8_t
    unpack_regack(uint16_t* topic_id, uint8_t* msg_id, uint8_t* return_code, 
                  uint8_t* buffer, uint16_t buffer_len)
    {
        uint8_t result = 0;    
        uint8_t* ptr = buffer;

        *topic_id = read_uint16(&ptr);
        *msg_id = read_uint16(&ptr);
        *return_code = read_uint8(&ptr);

        return result;
    }


    /**
     * Pack the Subscribe message
     */
    uint16_t 
    pack_subscribe(uint8_t* buffer, uint16_t buffer_len, 
                   uint8_t dup, QosLevel qos, uint16_t msg_id, const char* topic_name)
    {
        uint16_t result = 0;
        uint8_t* ptr = buffer;
        Flags flags;
        uint16_t topic_name_len = strlen(topic_name);

        if ((result = write_length(&ptr, buffer_len, 4 + topic_name_len)) == 0) {
            goto exit;
        }
        write_uint8(&ptr, SUBSCRIBE);
        flags.all = 0;
        flags.bits.dup = dup;
        flags.bits.qos = (uint8_t)qos;
        flags.bits.topic_id_type = TOPIC_TYPE_NAME;
        write_uint8(&ptr, flags.all);
        write_uint16(&ptr, msg_id);
        write_string(&ptr, topic_name, topic_name_len);

    exit:
        return result;
    }
                   
    uint16_t 
    pack_subscribe(uint8_t* buffer, uint16_t buffer_len, 
                   uint8_t dup, QosLevel qos, uint16_t msg_id, uint16_t topid_id)
    {
        uint16_t result = 0;
        uint8_t* ptr = buffer;
        Flags flags;

        if ((result = write_length(&ptr, buffer_len, 6)) == 0) {
            goto exit;
        }
        write_uint8(&ptr, SUBSCRIBE);
        flags.all = 0;
        flags.bits.dup = dup;
        flags.bits.qos = (uint8_t)qos;
        flags.bits.topic_id_type = TOPIC_TYPE_PREDEFINED;
        write_uint8(&ptr, flags.all);
        write_uint16(&ptr, msg_id);
        write_uint16(&ptr, topid_id);

    exit:
        return result;
    }

    /**
     * Unpack the Subscribe Acknowledge message
     */
    uint8_t
    unpack_suback(QosLevel* qos, uint16_t* topic_id, uint8_t* msg_id, ReturnCode* return_code, 
                  uint8_t* buffer, uint16_t buffer_len)
    {
        uint8_t result = 0;    
        uint8_t* ptr = buffer;
        Flags flags;

        flags.all = read_uint8(&ptr);
        *qos = (QosLevel)flags.bits.qos;
        *topic_id = read_uint16(&ptr);
        *msg_id = read_uint16(&ptr);
        *return_code = (ReturnCode)read_uint8(&ptr);

        return result;
    }


    /**
     * Pack the UnSubscribe message
     */
    uint16_t
    pack_unsubscribe(uint8_t* buffer, uint16_t buffer_len, 
                     uint16_t msg_id, const char* topic_name)
    {
        uint16_t result = 0;
        uint8_t* ptr = buffer;
        Flags flags;
        uint16_t topic_name_len = strlen(topic_name);

        if (!(result = write_length(&ptr, buffer_len, 4 + topic_name_len))) {
            goto exit;
        }
        write_uint8(&ptr, UNSUBSCRIBE);
        flags.all = 0;
        flags.bits.topic_id_type = TOPIC_TYPE_NAME;
        write_uint8(&ptr, flags.all);
        write_uint16(&ptr, msg_id);
        write_string(&ptr, topic_name, topic_name_len);

    exit:
        return result;
    }

    uint16_t
    pack_unsubscribe(uint8_t* buffer, uint16_t buffer_len, 
                     uint16_t msg_id, uint16_t topic_id)
    {
        uint16_t result = 0;
        uint8_t* ptr = buffer;
        Flags flags;

        if (!(result = write_length(&ptr, buffer_len, 6))) {
            goto exit;
        }
        write_uint8(&ptr, UNSUBSCRIBE);
        flags.all = 0;
        flags.bits.topic_id_type = TOPIC_TYPE_PREDEFINED;
        write_uint8(&ptr, flags.all);
        write_uint16(&ptr, msg_id);
        write_uint16(&ptr, topic_id);

    exit:
        return result;
    }

    /**
     * Unpack the UnSubscribe Acknowledge message
     */
    uint8_t
    unpack_unsuback(uint16_t* msg_id, 
                    uint8_t* buffer, uint16_t buffer_len)
    {
        uint8_t result = 0;    
        uint8_t* ptr = buffer;

        *msg_id = read_uint16(&ptr);

        return result;
    }

} // !MQTTSN
