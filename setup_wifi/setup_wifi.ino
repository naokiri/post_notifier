//
// Update ESP-WROOM-02 (ESP8266) baud rate and flash to RAM
//


#include <SoftwareSerial.h>

SoftwareSerial wifiSerial(2, 4); // RX, TX


// Default 115200 for first time. Update if configured.
#define WIFI_MODULE_BAUD_RATE 9600


void setupWifiModule() {
    Serial.println("Setup ESP-WROOM-02");    
    wifiSerial.print("AT+UART_DEF=9600,8,1,0,0\r\n");
  
    // wifiSerial.print("AT+CWMODE_DEF=1");
    // delay(1000);
    // if(wifiSerial.available()) {
    //     Serial.write(wifiSerial.read());
    // }
    // wifiSerial.print("AT+CWJAP_DEF=ssid,pwd");
    // delay(1000);
    // if(wifiSerial.available()) {
    //     Serial.write(wifiSerial.read());
    // }
    // wifiSerial.println("AT+UART_DEF=9600,8,1,0,0");
}

void setup() {
    Serial.begin(9600);
    while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
    }

    wifiSerial.begin(WIFI_MODULE_BAUD_RATE);
    setupWifiModule();
    wifiSerial.end();
    delay(500); // Seems safe around 100
    wifiSerial.begin(9600);
}

void loop() {
    if(wifiSerial.available()) {
        Serial.write(wifiSerial.read());
    }
    if (Serial.available()){
        wifiSerial.write(Serial.read());
    }
}