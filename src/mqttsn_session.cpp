#include "mqttsn_session.h"
#include "mqttsn_packet.h"

namespace MQTTSN
{

    /**
     * Pack the Connect message
     */
    uint16_t
    pack_connect(uint8_t* buffer, uint16_t buffer_len, 
                 uint8_t will, uint8_t clean_session, uint16_t duration, const char* client_id)
    {
        uint16_t result = 0;    
        uint8_t* ptr = buffer;
        Flags flags;
        uint16_t client_id_len = strlen(client_id);

        if (!(result = write_length(&ptr, buffer_len, 5 + client_id_len))) {
            goto exit;
        }
        write_uint8(&ptr, MQTTSN_CONNECT);
        flags.all = 0;
        flags.bits.will = will;
        flags.bits.clean_session = clean_session;
        write_uint8(&ptr, flags.all);
        write_uint8(&ptr, 1);  // Protocol version
        write_uint16(&ptr, duration);
        write_string(&ptr, client_id, client_id_len);

    exit:
        return result;
    }

    /**
     * Unpack the Connect Acknowledge message
     */
    uint8_t
    unpack_connack(uint8_t* return_code, 
                   uint8_t* buffer, uint16_t buffer_len)
    {
        uint8_t result = 0;    
        uint8_t* ptr = buffer;

        *return_code = read_uint8(&ptr);

        return result;
    }

    /**
     * Pack the Connect message
     */
    uint16_t
    pack_disconnect(uint8_t* buffer, uint16_t buffer_len, 
                    uint16_t duration)
    {
        uint16_t result = 0;    
        uint8_t* ptr = buffer;

        write_uint16(&ptr, duration);

        return result;
    }

    /**
     * Pack the Ping Request message
     */
    uint16_t
    pack_pingreq(uint8_t* buffer, uint16_t buffer_len,  
                 const char* client_id)
    {
        uint16_t result = 0;    
        uint8_t* ptr = buffer;
        uint16_t client_id_len = strlen(client_id);
        
        if ((result = write_length(&ptr, buffer_len, 1 + client_id_len)) == 0) {
            goto exit;
        }
        write_uint8(&ptr, MQTTSN_PINGREQ);
        write_string(&ptr, client_id, client_id_len);

    exit:
        return result;
    }

    /**
     * Unpack the Ping Response message
     */
    uint8_t
    unpack_pingresp(uint8_t* buffer, uint16_t buffer_len)
    {
        // Basically a no-op
        return 0;
    }

} // !MQTTSN
