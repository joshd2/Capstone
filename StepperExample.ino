//This codes was found on youTube.

//defines pins
const int stepPin = 6;  //PUL -Pulse
const int dirPin = 7; //DIR -Direction
const int enPin = 8;  //ENA -Enable

void setup(){
  //Sets the pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);
}

void loop(){
  //Enables the motor direction to move
  digitalWrite(dirPin,HIGH);
  //Makes 200 Pulses for making one full cycle rotation, without microstepping
  for(int x = 0; x < 200; x++){
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(500); 
  }
  
  //One second delay
  delay(1000);

  //Changes the rotations direction
  digitalWrite(dirPin,LOW);
  // Makes 200 pulses for making one full cycle rotation, without microstepping
  for(int x = 0; x < 200; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(500);
  }
  
  //One second delay
  delay(1000); 
}
