#include "ESP8266.h"
#include "ssid.h"
#include <SoftwareSerial.h>

#define BAUD_RATE 9600
// Default 115200, but might be stable if you setup slower uart_def.
#define WIFI_MODULE_BAUD_RATE 9600

SoftwareSerial wifiSerial(2, 4); // RX, TX
ESP8266 wifi(wifiSerial, WIFI_MODULE_BAUD_RATE);

#define TRIG_PIN 13
#define ECHO_PIN 12

void startESP8266() {
    while (1) {
        Serial.print("(re)starting...");
        if (wifi.restart()) {
            break;
        } else {
            Serial.println("not ok...");
            while (1) {
                if (wifi.kick()) {
                    Serial.println("ok");
                    break;
                } else {
                    Serial.println("not ok. Retrying in 5 sec...");
                    delay(5000);
                }
            }
        }
    }

    Serial.println("setup begin");   

    while (1) {
        if (wifi.setOprToStation()) {
            break;
        } else {
            Serial.println("Retrying set to station in 5 sec...");
            delay(5000);
        }
    }

    while (1) {
        if (wifi.joinAP(SSID, PASSWORD)) {
            break;
        } else {
            Serial.println("Retrying connection in 5 sec...");
            delay(5000);
        }
    }
}

/**
 * Send http request to notify host.
 * (GET request for ease for now.)
 */ 
void notify() {
    wifi.createTCP(NOTIFY_HOST, 80);
    char *flash = "GET /api/control?alert=001000 HTTP/1.1\r\nUser-Agent: "
                  "arduino\r\nHost: " NOTIFY_HOST "\r\nAccept: */*\r\n\r\n";

    wifi.send(flash, strlen(flash));

    uint8_t buffer[512] = {0};
    uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);
    if (len > 0) {
        Serial.println("Received:[");
        for (uint32_t i = 0; i < len; i++) {
            Serial.print((char)buffer[i]);
        }
        Serial.println("\r\n]");
    }

    wifi.releaseTCP();
}

void setup() {
    Serial.begin(BAUD_RATE);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for Native USB only
    }
    startESP8266();
    Serial.println("startup done");

    // notify();

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

/**
 * Take ~20 (12 + reflect) microsecond and return distance in meter.
 *
 * Assumes fixed sonic speed of 345m/s
 */
float getDistance(int trigPin, int echoPin) {
    unsigned long duration;
    float distance;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Wait max 0.1sec
    duration = pulseIn(echoPin, HIGH, 100000);
    distance = (duration / 2) * 0.000345;
    return distance;
}

void loop() {
    // Debug purpose 
    // if (wifiSerial.available()) {
    //     Serial.write(wifiSerial.read());
    // }
    // if (Serial.available()) {
    //     wifiSerial.write(Serial.read());
    // }

    float distance = getDistance(TRIG_PIN, ECHO_PIN);
    if (distance < 0.1) {
        notify();
    }
    Serial.print(distance);
    Serial.println(" m");
    delay(60000);
}