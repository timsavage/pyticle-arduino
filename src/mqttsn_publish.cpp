#include <string.h>

#include "mqttsn_packet.h"
#include "mqttsn_publish.h"

namespace MQTTSN
{

    /**
     * Pack the Publish message
     */
    uint16_t 
    pack_publish(uint8_t* buffer, uint16_t buffer_len, 
                 uint8_t dup, QosLevel qos, uint8_t retain, uint16_t topic_id, uint16_t msg_id, const void* data, uint16_t data_len)
    {
        uint16_t result = 0;
        uint8_t* ptr = buffer;
        Flags flags;

        if ((result = write_length(&ptr, buffer_len, 4 + data_len)) == 0) {
            goto exit;
        }
        write_uint8(&ptr, PUBLISH);
        flags.all = 0;
        flags.bits.dup = dup;
        flags.bits.qos = (uint8_t)qos;
        flags.bits.retain = retain;
        flags.bits.topic_id_type = TOPIC_TYPE_PREDEFINED;
        write_uint8(&ptr, flags.all);
        write_uint16(&ptr, topic_id);
        write_uint16(&ptr, msg_id);
        memcpy(ptr, data, data_len);

    exit:
        return result;
    }

    /**
     * Unpack the Publish message
     */
    uint8_t 
    unpack_publish(Flags* flags, uint16_t* topic_id, uint16_t* msg_id, void** data, uint16_t* data_len,
                   uint8_t* buffer, uint16_t buffer_len)
    {
        uint8_t result = 0;    
        uint8_t* ptr = buffer;

        flags->all = read_uint8(&ptr);
        *topic_id = read_uint16(&ptr);
        *msg_id = read_uint16(&ptr);
        *data_len = buffer_len - 6;  // Bytes in the messages
        *data = ptr;

        return result;
    }


    /**
     * Pack the Publish Acknowledge message 
     */
    uint16_t
    pack_puback(uint8_t* buffer, uint16_t buffer_len,
                uint16_t topic_id, uint16_t msg_id, ReturnCode return_code)
    {
        uint16_t result = 0;
        uint8_t* ptr = buffer;

        if ((result = write_length(&ptr, buffer_len, 6)) == 0) {
            goto exit;
        }
        write_uint8(&ptr, PUBACK);
        write_uint16(&ptr, topic_id);
        write_uint16(&ptr, msg_id);
        write_uint8(&ptr, return_code);

    exit:
        return result;
    }

    /**
     * Unpack the Publish Acknowledge message 
     */
    uint8_t
    unpack_puback(uint16_t* topic_id, uint8_t* msg_id, ReturnCode* return_code, 
                  uint8_t* buffer, uint16_t buffer_len)
    {
        uint8_t result = 0;    
        uint8_t* ptr = buffer;

        *topic_id = read_uint16(&ptr);
        *msg_id = read_uint16(&ptr);
        *return_code = (ReturnCode)read_uint8(&ptr);

        return result;
    }


    /**
     * Pack Rec/Rel/Comp message
     */
    uint16_t
    pack_pub(uint8_t* buffer, uint16_t buffer_len, 
             uint16_t msg_id)
    {
        uint16_t result = 0;
        uint8_t* ptr = buffer;

        if ((result = write_length(&ptr, buffer_len, 2)) == 0) {
            goto exit;
        }
        write_uint16(&ptr, msg_id);

    exit:
        return result;
    }

    /**
     * Unpack Rec/Rel/Comp message
     */
    uint8_t
    unpack_pub(uint16_t* msg_id,
               uint8_t* buffer, uint16_t buffer_len)
    {
        uint8_t result = 0;    
        uint8_t* ptr = buffer;

        *msg_id = read_uint16(&ptr);

        return result;
    }

} // !MQTTSN
