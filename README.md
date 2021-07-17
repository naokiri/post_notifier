Check if anything is in front of this hardware and notify my home server.
Built to notify physical mail for me.

## Target Hardware

- Arduino UNO R3 
- ESP-WROOM-02 wifi module
- HC-SR04 compatible ultrasonic distance sensor

## Current problem

Roughly 50mAh power consumption. Which means it can't surveillance longer than couple of days using decent sized battery.

Arduino UNO's watchdog timer is only for max 8 sec sleep so it doesn't help much for long duration surveillance.

Some small Arduino pro series might help bit lower power consumption.

## Future plan

Add timer module like TPL5110 or like DS3231 and sleep long if chip is available.