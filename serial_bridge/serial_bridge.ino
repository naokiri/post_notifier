// Serial console bridge for testing

#include <SoftwareSerial.h>
SoftwareSerial mySerial(2,4); // RX, TX
void setup()
{
  Serial.begin(9600);
  while (!Serial) {
        ; // wait for serial port to connect. Needed for Native USB only
  }
  mySerial.begin(9600);
  Serial.println("setup done");
}
void loop()
{
  if(mySerial.available())
  Serial.write(mySerial.read());
  if (Serial.available())
  mySerial.write(Serial.read());
}