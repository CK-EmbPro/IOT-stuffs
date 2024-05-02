#include <SPI.h>

const int slaveSelectPin = 10; // Define the slave select pin

void setup() {
  Serial.begin(9600);
  pinMode(slaveSelectPin, OUTPUT);
  SPI.begin();
}

void loop() {
  digitalWrite(slaveSelectPin, LOW); // Select the slave
  SPI.transfer('H'); // Send a character to the slave
  digitalWrite(slaveSelectPin, HIGH); // Deselect the slave
  delay(1000);
}
