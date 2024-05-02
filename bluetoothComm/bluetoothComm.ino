#include<SoftwareSerial.h>
SoftwareSerial configBt(10, 11);

void setup() {
  Serial.begin(9600);
  configBt.begin(9600);
}

void loop() {
  if (configBt.available()) {
    Serial.print(configBt.readString());
  } 

  if (Serial.available()) {
    configBt.write(Serial.read());
  }
}
