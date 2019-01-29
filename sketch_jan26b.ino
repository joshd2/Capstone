#include "DynamixelMotor.h"
#include "SoftwareSerial.h"
//script for the AX-12A motor, adapted from the examples in the Ardyno library

// id of the motor
const uint8_t id=1;
// speed, between 0 and 1023
int16_t speed=212;
// communication baudrate
// Note, the motor's bit rate must be set to 57600 for this script to work
// the motor's bit rate is initially 100000, see Ardyno example for changing the bitrate
const unsigned long dynamixel_baudrate=57600;
const unsigned long serial_baudrate=57600;

//Defines pin for the serial connection
#define SOFT_RX_PIN 3
#define SOFT_TX_PIN 4

// software serial without tristate buffer
// see blink_led example, and adapt to your configuration
SoftwareDynamixelInterface interface(SOFT_RX_PIN, SOFT_TX_PIN);
DynamixelMotor motor(interface, id);

void setup()
{ 
  interface.begin(dynamixel_baudrate);
  Serial.begin(serial_baudrate);
  delay(100);
  
  // check if we can communicate with the motor
  // if not, we turn the led on and stop here
  uint8_t status=motor.init();
  if(status!=DYN_STATUS_OK)
  {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    while(1);
  }

  motor.enableTorque();  


  //motor.jointMode();
  //reads the motor's current position and prints it to the serial port
  String info = String(motor.currentPosition()*0.29);
  Serial.print(info);
  Serial.print(" ");

 //Turns the motor about 0.23 degrees
  motor.wheelMode();
  motor.speed(speed);
  delay(60);
  motor.speed(0);
  delay(60);

  //reads the motor's current position and prints it to the serial port
  info = String(motor.currentPosition()*0.29);
  Serial.print(info);
  Serial.print(" ");
}

void loop() 
{
  
 //Turns the motor about 0.23 degrees
  motor.wheelMode();
  motor.speed(speed);
  delay(60);
  motor.speed(0);
  delay(60);

  //reads the motor's current position and prints it to the serial port
  String info = String(motor.currentPosition()*0.29);
  Serial.print(info);
  Serial.print(" ");
 
 
  //Code from the original joint example:
  // go to middle position
  //motor.goalPosition(512);
  //delay(500);

  // move 45° CCW
  //motor.goalPosition(666);
  //delay(500);

  // go to middle position
  //motor.goalPosition(512);
  //delay(500);

  // move 45° CW
  //motor.goalPosition(358);
  //delay(500);
}
