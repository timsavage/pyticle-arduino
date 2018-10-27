#include <string.h>

#include "mqttsn_packet.h"
#include "mqttsn_will.h"

namespace MQTTSN
{

    /**
     * Pack the Will Topic message
     */
    uint16_t 
    pack_will_topic(uint8_t* buffer, uint16_t buffer_len, 
                    uint8_t qos, uint8_t retain, const char* will_topic)
    {
        uint16_t result = 0;
        uint8_t* ptr = buffer;
        Flags flags;
        uint16_t will_topic_len = strlen(will_topic);

        if (!(result = write_length(&ptr, buffer_len, 2 + will_topic_len))) {
            goto exit;
        }
        write_uint8(&ptr, WILLTOPIC);
        flags.all = 0;
        flags.bits.qos = qos;
        flags.bits.retain = retain;
        write_uint8(&ptr, flags.all);
        write_string(&ptr, will_topic, will_topic_len);

    exit:
        return result;
    }

    /**
     * Pack the Will Topic Update message
     */
    uint16_t 
    pack_will_topic_update(uint8_t* buffer, uint16_t buffer_len, 
                           uint8_t qos, uint8_t retain, const char* will_topic)
    {
        uint16_t result = 0;
        uint8_t* ptr = buffer;
        Flags flags;
        uint16_t will_topic_len = strlen(will_topic);
        
        if (!(result = write_length(&ptr, buffer_len, 2 + will_topic_len))) {
            goto exit;
        }
        write_uint8(&ptr, WILLTOPICUPD);
        flags.all = 0;
        flags.bits.qos = qos;
        flags.bits.retain = retain;
        write_uint8(&ptr, flags.all);
        write_string(&ptr, will_topic, will_topic_len);

    exit:
        return result;
    }

    /**
     * Pack the Will Message message
     */
    uint16_t 
    pack_will_message(uint8_t* buffer, uint16_t buffer_len, 
                      const char* will_msg)
    {
        uint16_t result = 0;
        uint8_t* ptr = buffer;
        Flags flags;
        uint16_t will_msg_len = strlen(will_msg);

        if (!(result = write_length(&ptr, buffer_len, 1 + will_msg_len))) {
            goto exit;
        }
        write_uint8(&ptr, WILLMSG);
        write_string(&ptr, will_msg, will_msg_len);

    exit:
        return result;
    }

    /**
     * Pack the Will Message Update message
     */
    uint16_t 
    pack_will_message_update(uint8_t* buffer, uint16_t buffer_len, 
                             const char* will_msg)
    {
        uint16_t result = 0;
        uint8_t* ptr = buffer;
        Flags flags;
        uint16_t will_msg_len = strlen(will_msg);

        if (!(result = write_length(&ptr, buffer_len, 1 + will_msg_len))) {
            goto exit;
        }
        write_uint8(&ptr, WILLMSGUPD);
        write_string(&ptr, will_msg, will_msg_len);

    exit:
        return result;
    }

    /**
     * Unpack the Will Topic/Message Response message
     */
    uint8_t
    unpack_will_response(uint8_t* return_code,
                         uint8_t* buffer, uint16_t buffer_len)
    {
        uint8_t result = 0;    
        uint8_t* ptr = buffer;

        *return_code = read_uint8(&ptr);

        return result;
    }

}
