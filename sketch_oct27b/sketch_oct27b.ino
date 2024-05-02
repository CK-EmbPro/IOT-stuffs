#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX
void setup() {
Serial.begin(9600);
mySerial.begin(9600);
}
void loop() {
if (mySerial.available()) {
String receivedData = mySerial.readStringUntil('\n');
mySerial.println("Hello jodos");

Serial.print("Received data from Arduino1: ");
Serial.println(receivedData);
}
}
