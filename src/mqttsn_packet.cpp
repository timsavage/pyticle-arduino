#include "mqttsn_packet.h"

namespace MQTTSN 
{

static char* message_names[] =
{
		"ADVERTISE", "SEARCHGW", "GWINFO", "RESERVED", "CONNECT", "CONNACK",
		"WILLTOPICREQ", "WILLTOPIC", "WILLMSGREQ", "WILLMSG", "REGISTER", "REGACK",
		"PUBLISH", "PUBACK", "PUBCOMP", "PUBREC", "PUBREL", "RESERVED",
		"SUBSCRIBE", "SUBACK", "UNSUBSCRIBE", "UNSUBACK", "PINGREQ", "PINGRESP",
		"DISCONNECT", "RESERVED", "WILLTOPICUPD", "WILLTOPICRESP", "WILLMSGUPD",
		"WILLMSGRESP"
};

/**
 * Get the name of a message
 */
const char*
message_name(uint8_t msg_type)
{
    return (msg_type <= MQTTSN_WILLMSGRESP) ? message_names[msg_type] : "UNKNOWN";
}

/**
 * Calculate the total message length
 */
uint16_t
message_len(uint16_t length)
{
    return length + ((length > 0xFF) ? 3 : 1);
}

/**
 * Read the length out of the buffer
 * @return 0 if there is an error else the message length
 */
uint16_t
read_length(uint8_t** buffer, uint16_t buffer_len)
{
    uint16_t result = 0;
    if (buffer_len < 1) {
        return 1;
    }

    result = read_uint8(buffer);
    if (result == 1) {
        if (buffer_len < 3) {
            return 1;
        }
        result = read_uint16(buffer);
    }

    return result;
}

/**
 * Write the length into the buffer
 * @return 0 if all good, else error code
 */
uint16_t
write_length(uint8_t** buffer, uint16_t buffer_len, uint16_t msg_len)
{
    uint16_t total_len = message_len(msg_len);

    if (total_len > buffer_len) {
        return 0;
    }

    if (msg_len > 0xFF) {
        write_uint8(buffer, 1);
        write_uint16(buffer, total_len);
    } else {
        write_uint8(buffer, (uint8_t)total_len);
    }

    return total_len;
}

/**
 * Read an unsigned bytes from the buffer
 */
uint8_t 
read_uint8(uint8_t** buffer)
{
    uint8_t value = **buffer;
    (*buffer)++;
    return value;
}

/**
 * Write an unsigned byte to the buffer
 */
void 
write_uint8(uint8_t** buffer, uint8_t value)
{
    **buffer = (uint8_t)value;
    (*buffer)++;
}

/**
 * Read an unsigned short from the buffer
 */
uint16_t 
read_uint16(uint8_t** buffer)
{
    uint8_t* ptr = *buffer;
    uint16_t value = ((uint8_t)(*ptr) << 8) + (uint8_t)(*(ptr+1));
    *buffer += 2;
    return value;
}

/**
 * Write an unsigned short to the buffer
 */
void 
write_uint16(uint8_t** buffer, uint8_t value)
{
    **buffer = (uint8_t)(value >> 8);
    (*buffer)++;
    **buffer = (uint8_t)value;
    (*buffer)++;
}

void
write_string(uint8_t** buffer, const char* str, uint16_t str_len)
{
    memcpy(*buffer, str, str_len);
    *buffer += str_len;
}

} // !MQTTSN
