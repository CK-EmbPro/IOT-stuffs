int LED1 = 13;
int LED2 = 12;
int isObstaclePin = 7;
int isObstacle;

void setup(){
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(isObstaclePin, INPUT);
  Serial.begin(9600);
}
      
void loop(){
  isObstacle = digitalRead(isObstaclePin);
  if(isObstacle == LOW){
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    Serial.println("Obstacle detected!");
  }else{
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    Serial.println("clear, no obstacle");
  }

  
}
