#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "mqttsn_client.h"

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
    client.begin("Pyticle-Client01", WiFi.localIP());
}

void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println(F("Pyticle Client\nVersion: 0.1"));

    initNetwork();
    initMQTTSN();
}

void loop()
{
    client.loop();
    delay(1000);
}
