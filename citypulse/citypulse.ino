const int redPin = 7;
const int greenPin = 8;
void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  if (Serial.available() > 0) {
    // Read the incoming byte
    char incomingByte = Serial.read();
    // If the received byte is '1', turn on the green LED and turn off the red LED
    if (incomingByte == '1') {
      digitalWrite(greenPin, HIGH);
      delay(400);
      digitalWrite(redPin, LOW);
      Serial.println("Green LED on, Red LED off");
    }
    // If the received byte is '0', turn off the green LED and turn on the red LED
    else if (incomingByte == '0') {
      digitalWrite(greenPin, LOW);
         delay(400);
      digitalWrite(redPin, HIGH);
      Serial.println("Green LED off, Red LED on");
    }
  }
}
