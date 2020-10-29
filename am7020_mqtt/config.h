#ifndef _CONFIG_H
#define _CONFIG_H

#define TINY_GSM_MODEM_SIM7020

/* Define the serial console for debug prints, if needed */
// #define TINY_GSM_DEBUG SerialMon
/* uncomment to dump all AT commands */
#define DEBUG_DUMP_AT_COMMAND

#define UPLOAD_INTERVAL         60000

#define SerialMon               Serial
#define MONITOR_BAUDRATE        115200


/* AM7020 module setup: Serial port, baudrate, and reset pin */
#if (defined ARDUINO_AVR_UNO) || (defined ARDUINO_AVR_NANO)
    /* Arduino Uno */
    #include <SoftwareSerial.h>
    SoftwareSerial SoftSerial(8, 9);    //RX:8 TX:9    
    #define SerialAT            SoftSerial
    #define AM7020_BAUDRATE     38400
    #define AM7020_RESET        7

#elif defined ARDUINO_AVR_MEGA2560
    /* Arduino Mega2560 */
    #define SerialAT            Serial1
    #define AM7020_BAUDRATE     115200
    #define AM7020_RESET        7

#elif (defined ARDUINO_SAMD_MKRZERO)|| (defined ARDUINO_SAMD_MKRWIFI1010)
    /* Arduino MKR Series */
    #define SerialAT            Serial1
    #define AM7020_BAUDRATE     115200
    #define AM7020_RESET        A5
   
#elif defined ARDUINO_ESP32_DEV
    /* ESP32 Boards */
    #define SerialAT            Serial2
    #define AM7020_BAUDRATE     115200
    #define AM7020_RESET        5

#else
    /* add your own boards and uncomment it */
    /*
    #define SerialAT            Serial1
    #define AM7020_BAUDRATE     9600
    #define AM7020_RESET        7
    */

#endif

/* set GSM PIN */
#define GSM_PIN             ""

// for taiwan mobile
#define APN                 "twm.nbiot"
#define BAND                28

// for CHT
//#define APN               "internet.iot"
//#define BAND              8

// MQTT Setting
#define MQTT_BROKER         "broker.hivemq.com"
#define MQTT_PORT           1883
#define MQTT_USERNAME       ""
#define MQTT_PASSWORD       ""
#define MQTT_TOPIC          "temp/humidity"

#endif /* _CONFIG_H */
