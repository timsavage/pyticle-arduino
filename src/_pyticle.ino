#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "mqttsn_packet.h"
#include "mqttsn_messages.h"

#define MAX_BUFFER 0x400
#define DELAY_TIME 300000  //ms (300s)

uint8_t buffer[MAX_BUFFER];

WiFiUDP udp;
IPAddress search_ip = {192, 168, 1, 255};
IPAddress gateway_ip;
const uint16_t port = 1883;

uint32_t nextPing = 0;


void dumpBuffer(uint16_t msg_len) 
{
    for (uint16_t idx=0; idx < msg_len; idx++) {
        Serial.printf("%02x ", buffer[idx]);
    }
    Serial.printf(": len=%d\n", msg_len);
}

void initNetwork()
{
    Serial.println("- Init Wifi...");

    WiFi.begin("Penguins!", "M0nk3ysN0tD0nk3ys");

    Serial.print("  Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("  Connected, IP address: "); Serial.println(WiFi.localIP());
    Serial.println("  [OK]");
}

void beginMessage(void)
{
    memset(buffer, 0, MAX_BUFFER);
}

void sendMessage(IPAddress ip, uint16_t msg_len)
{
    if (msg_len == 0) {
        Serial.println("Error sending message.");
    } else {
        udp.beginPacket(ip, port); 
            udp.write(buffer, msg_len);
        udp.endPacket();
    }

    Serial.print("Sent packet: "); dumpBuffer(msg_len);
}

void initMQTTSN()
{
    uint16_t msg_len;

    Serial.println("- Init MQTTSN...");
  
    udp.begin(1883);

    Serial.print("  + Searching...");
    beginMessage();
    sendMessage(search_ip, MQTTSN::pack_searchgw(buffer, MAX_BUFFER, 2));
}


void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println(F("Pyticle Client\nVersion: 0.1"));

    initNetwork();
    initMQTTSN();
}

void mqttsn_loop() 
{
    uint8_t gateway_id;
    uint16_t duration;

    uint8_t* buffer_ptr;
    uint16_t buffer_len;
    uint16_t msg_len;
    uint8_t msg_type;

    while (udp.parsePacket()) {
        memset(buffer, 0, MAX_BUFFER);
        if ((buffer_len = udp.read(buffer, 255)) == 0) {
            continue;
        }
        // Read data
        buffer_ptr = buffer;
        if ((msg_len = MQTTSN::read_length(&buffer_ptr, buffer_len)) == 0) {
            continue;
        }
        msg_type = MQTTSN::read_uint8(&buffer_ptr);
        Serial.print("Recieved: "); Serial.print(MQTTSN::message_name(msg_type)); 
        Serial.printf(" from %s:%d\n", udp.remoteIP().toString().c_str(), udp.remotePort());

        switch(msg_type) {
            case MQTTSN::MQTTSN_ADVERTISE:
                if (!MQTTSN::unpack_advertise(&gateway_id, &duration, buffer_ptr, msg_len)) {
                    Serial.printf("Advertisement from %d: ", gateway_id); Serial.println(udp.remoteIP());
                }
                break;

            case MQTTSN::MQTTSN_GWINFO:
                gateway_ip = udp.remoteIP();
                Serial.print("Found gateway: "); Serial.println(gateway_ip);

                beginMessage();
                sendMessage(search_ip, MQTTSN::pack_connect(buffer, MAX_BUFFER, 0, DELAY_TIME, "Pyticle-Client01"));
                break;

            case MQTTSN::MQTTSN_CONNACK:
                nextPing = millis() + DELAY_TIME;
                break;

            case MQTTSN::MQTTSN_PINGRESP:
                Serial.println("Received ping response...");
                break;

            default:
                Serial.println("Unknown message");
                break;
        }
    }

    if (nextPing && nextPing < millis()) {
        nextPing = millis() + DELAY_TIME;
        Serial.println("Sending ping request...");
        beginMessage();
        sendMessage(search_ip, MQTTSN::pack_pingreq(buffer, MAX_BUFFER, "Pyticle-Client01"));
    }
}

void loop()
{
    mqttsn_loop();
    delay(1000);
}
