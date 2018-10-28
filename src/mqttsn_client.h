#include <Arduino.h>
#include <Udp.h>

#ifndef _mqttsn_client__h
#define _mqttsn_client__h

#ifndef MQTTSN_BUFFER_SIZE
    #define MQTTSN_BUFFER_SIZE 0x400
#endif //!MQTTSN_BUFFER_SIZE

#define MQTTSN_DEBUG
#define DEBUG_LOG Serial

namespace MQTTSN 
{

    class Client
    {
    public:
        /**
         * Construct client
         * 
         * @param sock - UDP socket instance being used
         * @param port - Port being used; defaults to 1883
         */
        Client(UDP* sock, int port=1883);

        /**
         * Begin/Initialise client
         */
        void begin(const char* client_id, IPAddress search_ip, uint32_t duration=300);

        void connect();

        uint16_t register_topic(const char* topic_name);

        void publish();

        /**
         * Process loop entry point.
         */
        void loop();

    protected:
        /**
         * Send message packed into buffer
         */
        void send_message(uint16_t msg_len);
        void send_message(uint16_t msg_len, IPAddress addr);

        // Message handlers
        void gwinfo(uint8_t* msg_ptr, uint16_t msg_len);
        void connack(uint8_t* msg_ptr, uint16_t msg_len);
        void pingresp(uint8_t* msg_ptr, uint16_t msg_len);

    private:
        UDP* m_sock;
        int m_port;
        IPAddress m_search_ip;     // IP Broadcast Address of search
        IPAddress m_gateway_ip;    // IP Address of current gateway.

        uint16_t m_msg_id;         // Next message id
        uint32_t m_duration;       // Time (in seconds) to next ping
        uint32_t m_next_ping;      // Time of next ping (in ms)

        uint8_t m_buffer[MQTTSN_BUFFER_SIZE];
        const char* m_client_id;
    };

} //!MQTTSN

#endif //!_mqttsn_client__h
