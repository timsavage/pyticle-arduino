#ifndef _mqttsn_client__h
#define _mqttsn_client__h

#include <Arduino.h>
#include <Udp.h>
#include "mqttsn_packet.h"

#define MQTTSN_DEBUG
#define MQTTSN_DEBUG_LOG Serial


#if !defined(MQTTSN_BUFFER_SIZE)
    #define MQTTSN_BUFFER_SIZE 128
#endif

#if !defined(MQTTSN_MAX_REGISTRATIONS)
    #define MQTTSN_MAX_REGISTRATIONS 4
#endif
#if !defined(MQTTSN_MAX_REGISTRATION_TOPIC_LEN)
    #define MQTTSN_MAX_REGISTRATION_TOPIC_LEN 32
#endif

#if !defined(MQTTSN_MAX_PING_RETRIES)
    #define MQTTSN_MAX_PING_RETRIES 3
#endif

namespace MQTTSN 
{

    class Timeout
    {
        public:
            Timeout(unsigned long ms=0) { m_expiry = ms ? (millis() + ms) : 0; }
        
            // Has the timerout expired?
            bool expired() { 
                if (m_expiry == 0) {
                    return false;
                } 
                return m_expiry < millis(); 
            }

            // Is the timeout active?
            bool active() { return m_expiry != 0; }
            
            // How much time is left?
            unsigned long left_ms()
            {
                unsigned long now = millis();
                if (m_expiry < now) {
                    return 0;
                } else {
                    return m_expiry - now;
                }
            }
            
            // Please expire in...
            void expire_in(unsigned long ms) { m_expiry = millis() + ms; }
            
            // Please expire at...
            void expire_at(unsigned long ms) { m_expiry = ms; }

        private:
            unsigned long m_expiry;
    };


    /**
     * Result structure
     */
    enum Result : uint8_t {
        Success,
        Failure = (uint8_t)-1,
    };


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

    protected:
        /**
         * Handle Message
         */
        MsgType handle_message(Timeout& timeout);

        /**
         * Wait for a particular a message
         */
        Result wait_for(MsgType msg_type, Timeout& timeout);
        
        /**
         * Send message packed into buffer
         */
        void send_message(uint16_t msg_len);
        void send_message(uint16_t msg_len, IPAddress addr);

    public:
        /**
         * Search for gateway
         */
        bool search();

        /**
         * Start connect
         */
        bool connect();
        void sleep();
        void awake();
        void disconnect();

        uint16_t register_topic(const char* topic_name);
        void unregister_topic(uint16_t topic_id);
        void unregister_all_topics();

        uint16_t subscribe(const char* topic_name);
        void unsubscribe(const char* topic_name);

        void publish(const char* topic_name, void* data, uint16_t data_len, enum QosLevel qos=QoSLevel0, bool retained=false);
        void publish(uint16_t topic_id, void* data, uint16_t data_len, enum QosLevel qos=QoSLevel0, bool retained=false);

        /**
         * Process loop entry point.
         */
        void loop();

    protected:
        // Message handlers
        uint8_t gwinfo(uint8_t* msg_ptr, uint16_t msg_len);
        uint8_t connack(uint8_t* msg_ptr, uint16_t msg_len);
        uint8_t regack(uint8_t* msg_ptr, uint16_t msg_len);
        uint8_t publish(uint8_t* msg_ptr, uint16_t msg_len);
        uint8_t puback(uint8_t* msg_ptr, uint16_t msg_len);
        uint8_t suback(uint8_t* msg_ptr, uint16_t msg_len);
        uint8_t unsuback(uint8_t* msg_ptr, uint16_t msg_len);
        uint8_t pingresp(uint8_t* msg_ptr, uint16_t msg_len);

    private:
        uint32_t m_duration;       // Time (in seconds) to next ping

        UDP* m_sock;
        int m_port;
        IPAddress m_search_ip;     // IP Broadcast Address of search
        IPAddress m_gateway_ip;    // IP Address of current gateway.
        bool m_connected;

        uint8_t ping_retry;
        bool ping_in_flight;

        Timeout m_last_received;     // Last time a message was received
        Timeout m_next_ping;         // Last time a message was received

        uint16_t m_msg_id;         // Next message id

        uint8_t m_buffer[MQTTSN_BUFFER_SIZE];
        const char* m_client_id;
    };

} //!MQTTSN

#endif //!_mqttsn_client__h
