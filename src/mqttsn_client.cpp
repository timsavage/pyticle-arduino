#include "mqttsn_client.h"
#include "mqttsn_packet.h"
#include "mqttsn_discovery.h"
#include "mqttsn_session.h"
#include "mqttsn_subscription.h"


MQTTSN::Client::Client(UDP* sock, int port)
: m_sock(sock),
  m_port(port),
  m_msg_id(0)
{
}

void MQTTSN::Client::begin(const char* client_id, IPAddress search_ip, uint32_t duration)
{
    m_client_id = client_id;
    m_search_ip = search_ip;
    m_duration = duration;
    
    // Initialise UDP socket
    m_sock->begin(m_port);

    // Send gateway search
    memset(m_buffer, 0, MQTTSN_BUFFER_SIZE);
    send_message(MQTTSN::pack_searchgw(m_buffer, MQTTSN_BUFFER_SIZE, 1), m_search_ip);
}

void MQTTSN::Client::loop()
{
    uint16_t buffer_len;
    uint8_t* msg_ptr;
    uint16_t msg_len;
    uint8_t msg_type;

    while (m_sock->parsePacket()) {
        memset(m_buffer, 0, MQTTSN_BUFFER_SIZE);

        // Read data into buffer
        if ((buffer_len = m_sock->read(m_buffer, MQTTSN_BUFFER_SIZE)) == 0) {
            continue;
        }

        // Parse length and message type.
        msg_ptr = m_buffer;
        if ((msg_len = MQTTSN::read_length(&msg_ptr, buffer_len)) == 0) {
            continue;
        }
        msg_type = MQTTSN::read_uint8(&msg_ptr);
        msg_len--;  // Remove one byte

        #if defined(MQTTSN_DEBUG)
        DEBUG_LOG.printf(
            "Recieved: %s from %s: %d\n",
            MQTTSN::message_name(msg_type),
            m_sock->remoteIP().toString().c_str(),
            m_sock->remotePort()
        );
        #endif

        switch(msg_type) {
        case MQTTSN::ADVERTISE:
            break;

        case MQTTSN::GWINFO:
            gwinfo(msg_ptr, msg_len);
            break;

        case MQTTSN::CONNACK:
            connack(msg_ptr, msg_len);
            break;

        case MQTTSN::PINGRESP:
            pingresp(msg_ptr, msg_len);
            break;
        
        case MQTTSN::SUBACK:
            break;
        
        case MQTTSN::PUBLISH:
            break;
        
        default:
            break;

        }
    }
}

void MQTTSN::Client::send_message(uint16_t msg_len)
{
    // send_message(msg_len, m_gateway_ip);
    send_message(msg_len, m_search_ip);
}

void MQTTSN::Client::send_message(uint16_t msg_len, IPAddress addr)
{
    m_sock->beginPacket(addr, m_port); 
        m_sock->write(m_buffer, msg_len);
    m_sock->endPacket();
}

void MQTTSN::Client::gwinfo(uint8_t* msg_ptr, uint16_t msg_len)
{
    m_gateway_ip = m_sock->remoteIP();

    #if defined(MQTTSN_DEBUG)
    Serial.print("Found gateway: "); Serial.println(m_gateway_ip);
    #endif

    // Initiate connection
    memset(m_buffer, 0, MQTTSN_BUFFER_SIZE);
    send_message(MQTTSN::pack_connect(m_buffer, MQTTSN_BUFFER_SIZE, 0, 1, m_duration, m_client_id));
}

void MQTTSN::Client::connack(uint8_t* msg_ptr, uint16_t msg_len)
{
    m_next_ping = millis() + m_duration;

    // Subscribe
    memset(m_buffer, 0, MQTTSN_BUFFER_SIZE);
    send_message(MQTTSN::pack_subscribe(m_buffer, MQTTSN_BUFFER_SIZE, 0, QoSLevel0, m_msg_id++, "environmental"));
}

void MQTTSN::Client::pingresp(uint8_t* msg_ptr, uint16_t msg_len)
{

}
