#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Dump packet data to Serial
#define MQTTSN_DEBUG

#include "mqttsn_client.h"
#include "mqttsn_packet.h"
#include "mqttsn_discovery.h"
#include "mqttsn_session.h"
#include "mqttsn_subscription.h"
#include "mqttsn_publish.h"

#define DELAY_TIME 300000  //ms (300s)

WiFiUDP udp;
MQTTSN::Client client(&udp);


// void dumpBuffer(uint16_t msg_len) 
// {
//     for (uint16_t idx=0; idx < msg_len; idx++) {
//         Serial.printf("%02x ", buffer[idx]);
//     }
//     Serial.printf(": len=%d\n", msg_len);
// }

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

void initMQTTSN()
{
    Serial.println("- Init MQTTSN...");
    client.begin("Pyticle-Client01", IPAddress(192, 168, 1, 255));
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
    client.loop();

    // uint8_t gateway_id;
    // uint16_t duration;

    // uint16_t buffer_len;
    // uint8_t* message_ptr;
    // uint16_t msg_len;
    // uint8_t msg_type;
    // MQTTSN::Flags flags;
    // uint16_t topic_id;
    // uint16_t msg_id;
    // uint8_t* data_ptr;
    // uint16_t data_len;

    // while (udp.parsePacket()) {
    //     memset(buffer, 0, MAX_BUFFER);
    //     if ((buffer_len = udp.read(buffer, 255)) == 0) {
    //         continue;
    //     }
    //     // Read data
    //     message_ptr = buffer;
    //     if ((msg_len = MQTTSN::read_length(&message_ptr, buffer_len)) == 0) {
    //         continue;
    //     }
    //     msg_type = MQTTSN::read_uint8(&message_ptr);
    //     Serial.print("Recieved: "); Serial.print(MQTTSN::message_name(msg_type)); 
    //     Serial.printf(" from %s:%d\n", udp.remoteIP().toString().c_str(), udp.remotePort());

    //     switch(msg_type) {
    //         case MQTTSN::ADVERTISE:
    //             if (!MQTTSN::unpack_advertise(&gateway_id, &duration, message_ptr, msg_len)) {
    //                 Serial.printf("Advertisement from %d: ", gateway_id); Serial.println(udp.remoteIP());
    //             }
    //             break;

    //         case MQTTSN::GWINFO:
    //             gateway_ip = udp.remoteIP();
    //             Serial.print("Found gateway: "); Serial.println(gateway_ip);

    //             beginMessage();
    //             sendMessage(search_ip, MQTTSN::pack_connect(buffer, MAX_BUFFER, 0, 1, 300, "Pyticle-Client01"));
    //             break;

    //         case MQTTSN::CONNACK:
    //             Serial.println("Connected.");
    //             nextPing = millis() + DELAY_TIME;

    //             // Subscribe
    //             beginMessage();
    //             sendMessage(search_ip, MQTTSN::pack_subscribe(buffer, MAX_BUFFER, 0, 0, message_id.next(), "environmental"));
    //             break;

    //         case MQTTSN::PINGRESP:
    //             Serial.println("Received ping response...");
    //             break;

    //         case MQTTSN::SUBACK:
    //             break;

    //         case MQTTSN::PUBLISH:
    //             MQTTSN::unpack_publish(&flags, &topic_id, &msg_id, &data_ptr, &data_len, message_ptr, msg_len);
    //             Serial.print("Publish: topic_id="); Serial.print(topic_id); Serial.print("; msg_id="); Serial.print(msg_id);
    //             Serial.print("; data_ptr="); Serial.print((char*)data_ptr); Serial.print("; data_len="); Serial.println(data_len);
    //             sendMessage(search_ip, MQTTSN::pack_puback(buffer, MAX_BUFFER, topic_id, msg_id, MQTTSN::Accepted));
    //             break;

    //         default:
    //             Serial.println("Unknown message");
    //             break;
    //     }
    // }

    // if (nextPing && nextPing < millis()) {
    //     nextPing = millis() + DELAY_TIME;
    //     Serial.println("Sending ping request...");
    //     beginMessage();
    //     sendMessage(search_ip, MQTTSN::pack_pingreq(buffer, MAX_BUFFER, "Pyticle-Client01"));
    // }
}

void loop()
{
    client.loop();
    delay(1000);
}
