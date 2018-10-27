#include <stdint.h>

#ifndef _mqttsn_packet__h
#define _mqttsn_packet__h

namespace MQTTSN
{

enum MsgTypes : uint8_t
{
	ADVERTISE, SEARCHGW, GWINFO, RESERVED1, 
	CONNECT, CONNACK,
	WILLTOPICREQ, WILLTOPIC, WILLMSGREQ, WILLMSG, 
	REGISTER, REGACK,
	PUBLISH, PUBACK, PUBCOMP, PUBREC, PUBREL, RESERVED2,
	SUBSCRIBE, SUBACK, UNSUBSCRIBE, UNSUBACK, 
	PINGREQ, PINGRESP,
	DISCONNECT, RESERVED3, 
	WILLTOPICUPD, WILLTOPICRESP, WILLMSGUPD, WILLMSGRESP,
};

enum TopicTypes : uint8_t
{
	TOPIC_TYPE_NAME,        // Normal topic type
	TOPIC_TYPE_PREDEFINED,  // Pre-defined ID
	TOPIC_TYPE_SHORT,       // Short topic name
};

enum QosLevels : uint8_t
{
	QoSLevel0,             // Fire and Forget
	QoSLevel1,             // Acknowledged deliver
	QoSLevel2,             // Assured Delivery
	QoSLevelNoConnection,  // No Connection required
};

enum ReturnCodes : uint8_t
{
	Accepted,
	Congested,
	InvalidTopicID,
	NotSupported,
	// Reserved
};

typedef union
{
	uint8_t all;
	struct
	{
		uint8_t topic_id_type: 2;  // See TopicTypes enum
		uint8_t clean_session: 1;  // Clear the existing session on connect
		uint8_t will: 1;           // Client wishes to set a Will during connect
		uint8_t retain: 1;
		uint8_t qos: 2;            // QoS level 0-2 (0b00, 0b01, 0b10)
		uint8_t dup: 1;            // Set if this message is being retransmitted
	} bits;
} Flags;


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
write_uint16(uint8_t** buffer, uint16_t value);

/**
 * Write a char array to the buffer
 */
void
write_string(uint8_t** buffer, const char* str, uint16_t str_len);

} //!MQTTSN

#endif //!_mqttsn_packet__h
