#include <Arduino.h>

#ifndef _mqttsn_packet__h
#define _mqttsn_packet__h

namespace MQTTSN
{

enum MsgTypes
{
	MQTTSN_ADVERTISE, MQTTSN_SEARCHGW, MQTTSN_GWINFO, MQTTSN_RESERVED1, 
	MQTTSN_CONNECT, MQTTSN_CONNACK,
	MQTTSN_WILLTOPICREQ, MQTTSN_WILLTOPIC, MQTTSN_WILLMSGREQ, MQTTSN_WILLMSG, 
	MQTTSN_REGISTER, MQTTSN_REGACK,
	MQTTSN_PUBLISH, MQTTSN_PUBACK, MQTTSN_PUBCOMP, MQTTSN_PUBREC, MQTTSN_PUBREL, MQTTSN_RESERVED2,
	MQTTSN_SUBSCRIBE, MQTTSN_SUBACK, MQTTSN_UNSUBSCRIBE, MQTTSN_UNSUBACK, 
	MQTTSN_PINGREQ, MQTTSN_PINGRESP,
	MQTTSN_DISCONNECT, MQTTSN_RESERVED3, 
	MQTTSN_WILLTOPICUPD, MQTTSN_WILLTOPICRESP, MQTTSN_WILLMSGUPD, MQTTSN_WILLMSGRESP,
};

/**
 * Get the name of a message
 */
const char*
message_name(uint8_t msg_type);

/**
 * Calculate the total message length
 */
uint16_t
message_len(uint16_t msg_len);

/**
 * Read the length out of the buffer
 * @return 0 if there is an error else the message length
 */
uint16_t
read_length(uint8_t** buffer, uint16_t buffer_len);

/**
 * Write the length into the buffer
 * @return 0 if all good, else error code
 */
uint16_t
write_length(uint8_t** buffer, uint16_t buffer_len, uint16_t msg_len);

/**
 * Read an unsigned bytes from the buffer
 */
uint8_t 
read_uint8(uint8_t** buffer);

/**
 * Write an unsigned byte to the buffer
 */
void 
write_uint8(uint8_t** buffer, uint8_t value);

/**
 * Read an unsigned short from the buffer
 */
uint16_t 
read_uint16(uint8_t** buffer);

/**
 * Write an unsigned short to the buffer
 */
void 
write_uint16(uint8_t** buffer, uint8_t value);

/**
 * Write a char array to the buffer
 */
void
write_string(uint8_t** buffer, const char* str, uint16_t str_len);

} //!MQTTSN

#endif //!_mqttsn_packet__h
