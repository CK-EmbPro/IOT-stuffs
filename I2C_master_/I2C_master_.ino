#include<Wire.h>

void setup() {
  Wire.begin(3);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(9);//address of the arduino 2
  Wire.write("Hello from ck");
  Wire.endTransmission();

  delay(1000);

}

void receiveEvent(){
  while(Wire.available()){
    char receivedChar = Wire.read();
    Serial.print("Received from jodos: ");
    Serial.println(receivedChar);
  }
}
