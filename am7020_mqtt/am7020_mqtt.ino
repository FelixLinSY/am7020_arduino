#include "config.h"
#include <PubSubClient.h>
#include <TinyGsmClient.h>

#ifdef DEBUG_DUMP_AT_COMMAND 
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm        modem(debugger, AM7020_RESET);

#else
TinyGsm       modem(SerialAT, AM7020_RESET);

#endif

TinyGsmClient tcpClient(modem);
PubSubClient  mqttClient(MQTT_BROKER, MQTT_PORT, tcpClient);

void mqttCallback(char *topic, byte *payload, unsigned int len);
void mqttConnect(void);
void nbConnect(void);

void setup()
{
    SerialMon.begin(MONITOR_BAUDRATE);
    SerialAT.begin(AM7020_BAUDRATE);
    
    randomSeed(analogRead(A0));
    
    nbConnect();
    mqttClient.setCallback(mqttCallback);
    mqttClient.setKeepAlive(300);
}

void loop()
{
    static unsigned long timer = 0;
    char                 buff[10];

    if (!mqttClient.connected()) {
        if (!modem.isNetworkConnected()) {
            nbConnect();
        }
        SerialMon.println(F("=== MQTT NOT CONNECTED ==="));
        mqttConnect();
    }

    if (millis() >= timer) {
        timer = millis() + UPLOAD_INTERVAL;
        sprintf(buff, "%ld", random(65535));
        SerialMon.print(F("Publish: "));
        SerialMon.println(buff);
        mqttClient.publish(MQTT_TOPIC, buff);
    }
    
    mqttClient.loop();
}

void mqttCallback(char *topic, byte *payload, unsigned int len)
{
    SerialMon.print(F("Message arrived ["));
    SerialMon.print(topic);
    SerialMon.print(F("]: "));
    SerialMon.write(payload, len);
    SerialMon.println();
}

void mqttConnect(void)
{
    SerialMon.print(F("Connecting to "));
    SerialMon.print(MQTT_BROKER);
    SerialMon.print(F("..."));

    // Connect to MQTT Broker
    String mqttid = ("MQTTID_" + String(random(65536)));
    while (!mqttClient.connect(mqttid.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
        SerialMon.println(F(" fail"));
    }
    SerialMon.println(F(" success"));
    mqttClient.publish(MQTT_TOPIC, "00000");
    mqttClient.subscribe(MQTT_TOPIC);
}

void nbConnect(void)
{
    SerialMon.println(F("Initializing modem..."));
    while (!modem.init() || !modem.nbiotConnect(APN, BAND)) {
        SerialMon.print(F("."));
    };

    SerialMon.print(F("Waiting for network..."));
    while (!modem.waitForNetwork()) {
        SerialMon.print(F("."));
    }
    SerialMon.println(F(" success"));
}
