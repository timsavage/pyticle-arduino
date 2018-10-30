#include "mqttsn_client.h"
#include "mqttsn_packet.h"
#include "mqttsn_discovery.h"
#include "mqttsn_session.h"
#include "mqttsn_subscription.h"

#if defined(MQTTSN_DEBUG)
    #define DEBUG_print(format) MQTTSN_DEBUG_LOG.print(format)
    #define DEBUG_printf(format, args...) MQTTSN_DEBUG_LOG.printf(format, args)
    #define DEBUG_println(format) MQTTSN_DEBUG_LOG.println(format)
#else
    #define DEBUG_print(format)
    #define DEBUG_printf(format, args...)
    #define DEBUG_println(format)
#endif

namespace MQTTSN {

Client::Client(UDP* sock, int port)
: m_sock(sock),
  m_port(port),
  m_msg_id(0),
  m_connected(false),
  ping_in_flight(false)
{
}

void Client::begin(const char* client_id, IPAddress search_ip, uint32_t duration_s)
{
    m_client_id = client_id;
    m_search_ip = search_ip;
    m_duration = duration_s * 1000;

    // Ensure last atom is a broadcast address
    m_search_ip[3] = 255;

    // Initialise UDP socket
    m_sock->begin(m_port);
}

void Client::loop()
{
    Timeout timeout(1000);
    handle_message(timeout);

    if (m_last_received.expired()) {
        if (!ping_in_flight) {
            ping_in_flight = true;

            memset(m_buffer, 0, MQTTSN_BUFFER_SIZE);
            send_message(pack_pingreq(m_buffer, MQTTSN_BUFFER_SIZE, m_client_id), m_search_ip);
        }
    }
}

MsgType Client::handle_message(Timeout& timeout)
{
    uint16_t buffer_len;
    uint8_t* msg_ptr;
    uint16_t msg_len;
    MsgType msg_type;

    while (m_sock->parsePacket()) {
        // Read data into buffer
        clear_buffer();
        if ((buffer_len = m_sock->read(m_buffer, MQTTSN_BUFFER_SIZE)) == 0) {
            continue;
        }

        // Parse length and message type.
        msg_ptr = m_buffer;
        if ((msg_len = read_length(&msg_ptr, buffer_len)) == 0) {
            continue;
        }
        msg_type = (MsgType)read_uint8(&msg_ptr);
        msg_len--;  // Remove one byte

        DEBUG_printf(
            "Recieved: %s from %s: %d\n",
            message_name(msg_type),
            m_sock->remoteIP().toString().c_str(),
            m_sock->remotePort()
        );

        switch(msg_type) {
        case CONNACK:
            connack(msg_ptr, msg_len);
            break;
        case REGACK:
            regack(msg_ptr, msg_len);
            break;
        case SUBACK:
            suback(msg_ptr, msg_len);
            break;
        case UNSUBACK:
            unsuback(msg_ptr, msg_len);
            break;
        case ADVERTISE:
            break;
        case GWINFO:
            gwinfo(msg_ptr, msg_len);
            break;
        case PINGRESP:
            pingresp(msg_ptr, msg_len);
            break;
        case PUBLISH:
            publish(msg_ptr, msg_len);
            break;
        }
    }

    m_last_received.expire_in(this->m_duration);

    return msg_type;
}

Result Client::wait_for(MsgType msg_type, Timeout& timeout)
{
    do {
        if (timeout.expired()) {
            break;
        }
    } while (handle_message(timeout) != msg_type);

    return Success;
}

void Client::clear_buffer()
{
    memset(m_buffer, 0, MQTTSN_BUFFER_SIZE);
}
    
void Client::send_message(uint16_t msg_len)
{
    // send_message(msg_len, m_gateway_ip);
    send_message(msg_len, m_search_ip);
}

void Client::send_message(uint16_t msg_len, IPAddress addr)
{
    m_sock->beginPacket(addr, m_port); 
        m_sock->write(m_buffer, msg_len);
    m_sock->endPacket();
}

bool Client::search()
{
    Timeout timeout(1000);

    // Send gateway search
    clear_buffer();
    send_message(pack_searchgw(m_buffer, MQTTSN_BUFFER_SIZE), m_search_ip);

    if (wait_for(GWINFO, timeout) == Success) {
        connect();
    }
}

bool Client::connect()
{
    Timeout connect_timer(5000);
    ReturnCode rc;

    // Initiate connection
    clear_buffer();
    send_message(pack_connect(m_buffer, MQTTSN_BUFFER_SIZE, 0, 1, m_duration, m_client_id));

    if (wait_for(CONNACK, connect_timer) == Success) {
        unpack_connack(&rc, m_buffer + 2, MQTTSN_BUFFER_SIZE);
    }

    if (rc == Accepted) {
        m_connected = true;
        return true;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////////
// Message handlers

uint8_t Client::gwinfo(uint8_t* msg_ptr, uint16_t msg_len)
{
    m_gateway_ip = m_sock->remoteIP();
    DEBUG_print("Found gateway: "); DEBUG_println(m_gateway_ip);
    return Success;
}

uint8_t Client::connack(uint8_t* msg_ptr, uint16_t msg_len)
{
    return Success;
}

uint8_t Client::regack(uint8_t* msg_ptr, uint16_t msg_len)
{

    return Success;
}

uint8_t Client::publish(uint8_t* msg_ptr, uint16_t msg_len)
{

    return Success;
}

uint8_t Client::puback(uint8_t* msg_ptr, uint16_t msg_len)
{

    return Success;
}

uint8_t Client::suback(uint8_t* msg_ptr, uint16_t msg_len)
{

    return Success;
}

uint8_t Client::unsuback(uint8_t* msg_ptr, uint16_t msg_len)
{

    return Success;
}

uint8_t Client::pingresp(uint8_t* msg_ptr, uint16_t msg_len)
{
    ping_in_flight = false;
    DEBUG_print("Ping received: "); DEBUG_println(m_gateway_ip);
    return Success;
}

};
