#include "DynamixelMotor.h"
#include "SoftwareSerial.h"
/*
script for the AX-12A motor, adapted from the examples in the Ardyno library
This Script moves the motor with four commands, as outlined below
  setSpeed [speed]
    Enter this command with a speed in rpm
    ex: setSpeed 10
    The above example sets the speed to 10 rotations per min
    the speed value has to be below 114 rpm
  rotate [location]
    Moves the motor to the specified location in degrees
    ex: rotate 10
    The above example rotates the motor to 10 degrees
    The location passed to the motor must be below 296 degrees
  rotateA [location]
    Moves the motor to the specified location in degrees, with acceleration control
    ex: rotateA 10
    The above example rotates the motor to 10 degrees
    The location passed to the motor must be below 296 degrees
  constantTurn [value]
    rotates the motor at a specified speed (% of maximum motor torque)
    ex: constantTurn 10
    The above example rotates the motor at 10% of the max torque available
    To stop the motor from rotating use the following command:
      constantTurn 0
    Note: if a value is NOT passed with this command, the speed is set to 10%
  constantTurnA [value]
    rotates the motor at a specified speed (% of maximum motor torque), with acceleration control
    ex: constantTurn 10
    The above example rotates the motor at 10% of the max torque available
    To stop the motor from rotating use the following command:
      constantTurn 0
    Note: if a value is NOT passed with this command, the speed is set to 10%
 getPosition
    returns the current location of the motor
 getStatus
    returns the status of the motor
Note: if going between joint and wheel mode, the motor has to turn abruptly
*/
// id of the motor
const uint8_t id=1;
// speed, between 0 and 1023
int16_t speed = 30/0.111;
// communication baudrate
// Note, the motor's bit rate must be set to 9600 for this script to work
// the motor's bit rate is initially 100000, see Ardyno example for changing the bitrate
const unsigned long dynamixel_baudrate=9600;
const unsigned long serial_baudrate=9600;


//Defines pins for the serial connection
#define SOFT_RX_PIN 3
#define SOFT_TX_PIN 4
int serialByte = 0;
String serialString = "";
SoftwareDynamixelInterface interface(SOFT_RX_PIN, SOFT_TX_PIN);
DynamixelMotor motor(interface, id);

void setup()
{ 
  interface.begin(dynamixel_baudrate);
  Serial.begin(serial_baudrate);
  delay(100);
  motor.enableTorque();  
}


void loop() 

    
{
  int16_t index = 0;
  float value = 0;
  int16_t speedValue = 0;
  int16_t positionValue = 0;
  uint8_t motorStatus= 0;
  bool flag = false;
  String subString1 = "";
  String subString2 = "";
 
  
  if(Serial.available() > 0 ){
    serialString = Serial.readString();
    Serial.print(serialString);
  
    //Parses the value from the serial port
    if(serialString.indexOf(' ') >= 0) { //test for character in string
            index = serialString.indexOf(' ');   
              subString1 = serialString.substring(0, index);
              subString2 = serialString.substring(index+1);  
      }
      else
      {
              subString1 = serialString;
              subString1.trim();
      }
      
  //-----Getting the status of the motor 
      if (subString1 == "getStatus"){
        flag = true;
        Serial.print("Getting the status of the motor \n");
        motorStatus = motor.init();
        Serial.print(String(motorStatus)+ "\n");
      }
     
  //-----Setting the speed of the motor for jointMode   
      if (subString1 == "setSpeed"){
        flag = true;
        Serial.print("Setting the motor speed\n");
        //If no value is being passed with the setSpeed command an error displayed
        if (subString2 == "")
        {
          Serial.print("Speed Value not entered properly, the value should be a number between 0 and 114 (rpm)\n");
        }
        else
        {
          value = subString2.toInt();
          speedValue = value/0.111;
          //Displays an error if the value is out of range
          if (speedValue > 1023)
            Serial.print("Speed value is out of range--must be smaller than 114 rpm \n");
          else
          {
            motor.jointMode();
            speed = speedValue;
            motor.speed(speed);
          }
        }
      }
  
  //-------Rotating the motor to the specified location with acceleration control-----
      if (subString1 == "rotateA"){
        flag = true;
        Serial.print("I will rotate the motor\n");
        
        if (subString2 == ""){
          Serial.print("Goal Angle not entered properly, the value should be a number below 300(degrees)\n");
        }
        else
        {
          value = subString2.toInt();
          positionValue = value/0.29;
        
          if (positionValue > 1023)
            Serial.print("Goal value is out of range--must be smaller than 300 degrees\n");
          
          else
          {
            motor.jointMode();
            motor.speed(speed);
            Serial.print("Nearest Position: " + String(positionValue*0.29) + "\n");
            unsigned long currentMillis = millis();
            unsigned long previousMillis = 0;
            long interval = 50; //Interval in milliseconds
            float Cposition = motor.currentPosition();
            int16_t originalPosition = motor.currentPosition();
            float multiplier = 1;
            int16_t moveTo = 0;
            if (Cposition < positionValue){
              while (Cposition < positionValue){
                currentMillis = millis();
                if ((currentMillis - previousMillis) > interval){
                  previousMillis = currentMillis;
                  if ((positionValue-Cposition)>((positionValue-originalPosition)/2)){
                    moveTo = Cposition + multiplier;
                    if (moveTo > positionValue)
                      moveTo = positionValue;
                    motor.goalPosition(moveTo);
                    Cposition = Cposition + multiplier;
                    multiplier = multiplier + 1;
                    
                  }
                  else if ((positionValue-Cposition)<=((positionValue-originalPosition)/2)){
                    if ((multiplier - 1) > 0)
                      multiplier = multiplier - 1;
                    moveTo = Cposition + multiplier;
                    if (moveTo > positionValue)
                      moveTo = positionValue;
                    motor.goalPosition(moveTo);
                    Cposition = Cposition + multiplier;  
                  }
                }
              }
            }
            else if (Cposition > positionValue)
            {
               motor.goalPosition(positionValue);
               while (Cposition > positionValue){
                currentMillis = millis();
                if ((currentMillis - previousMillis) > interval){
                  previousMillis = currentMillis;
                  if ((positionValue-Cposition)<((positionValue-originalPosition)/2)){
                    moveTo = Cposition - multiplier;
                    if (moveTo < positionValue)
                      moveTo = positionValue;
                    motor.goalPosition(moveTo);
                    Cposition = Cposition - multiplier;
                    multiplier = multiplier + 1;
                  }
                  else if ((positionValue-Cposition) >((positionValue-originalPosition)/2)){
                    moveTo = Cposition - multiplier;
                    if (moveTo < positionValue)
                      moveTo = positionValue;
                    motor.goalPosition(moveTo);
                    if ((multiplier - 1) > 0)
                      multiplier = multiplier - 1;
                    Cposition = Cposition - multiplier;  
                  }
                }
              }
              
            }  
            
            
            delay(2000);
            serialString = String(motor.currentPosition()*0.29);
            Serial.print("Current Position: " + serialString + "\n\n");
          }
        }
      }
      
      
  //---------------Rotating the motor to the specified location
      if (subString1 == "rotate"){
        flag = true;
        Serial.print("I will rotate the motor\n");
        if (subString2 == ""){
          Serial.print("Goal Angle not entered properly, the value should be a number below 300(degrees)\n");
        }
        else
        {
          value = subString2.toInt();
          positionValue = value/0.29;
          if (positionValue > 1023)
            Serial.print("Goal value is out of range--must be smaller than 300 degrees\n");
          else
          {
            motor.jointMode();
            motor.speed(speed);
            Serial.print("Nearest Position: " + String(positionValue*0.29) + "\n");
            motor.goalPosition(positionValue);
            delay(2000);
            serialString = String(motor.currentPosition()*0.29);
            Serial.print("Current Position: " + serialString + "\n\n");
          }
        }
      }
  
  //-----Setting the motor to turn constantly with acceleration control-----
      int16_t position1 = 0;
      int16_t position2 = 0;
      float setingSpeed = 0.1/0.1;
      float measSpeed = 0;
      float curSpeed = 0;
      
      if (subString1 == "constantTurnA"){
        flag = true;
  
        //Determinging the initial speed of the motor 
        position1 = motor.currentPosition();
        delayMicroseconds(100000);
        position2 = motor.currentPosition();
        curSpeed = 2*(position2 - position1)*600/360;
        
        //Putting the motor into wheel mode
        Serial.print("I will constantly rotate the motor\n");
        motor.wheelMode();
  
        
        if (subString2 == "")
          motor.speed(100);
        else
        {
          value = subString2.toInt();
          speedValue = value/0.1;
          if (speedValue > 1000)
            Serial.print("The speed value is out of range \n");
          else
          {
            if (speedValue > curSpeed/0.1){
              if (curSpeed/0.1 > speed)
                setingSpeed = curSpeed;
              else
                setingSpeed = speed;
              speed = speedValue;
              motor.speed(setingSpeed);
             
              //Getting the speed
              while ((measSpeed/0.1) < speed && setingSpeed < 1000){
                delayMicroseconds(5000000);
                position1 = motor.currentPosition();
                delayMicroseconds(100000);
                position2 = motor.currentPosition();
                measSpeed = 2*(position2 - position1)*600/360;
                if ((measSpeed/0.1) < speed){
                  setingSpeed = setingSpeed + 10;
                  motor.speed(setingSpeed);
                }
              }
              speed = setingSpeed;
            }
            else if (speedValue < curSpeed/0.1){
              if (curSpeed/0.1 > speed)
                setingSpeed = curSpeed;
              else
                setingSpeed = speed;
              speed = speedValue;
              measSpeed = 100;
              while ((measSpeed/0.1) > speed && setingSpeed < 1000){
                delayMicroseconds(5000000);
                position1 = motor.currentPosition();
                delayMicroseconds(100000);
                position2 = motor.currentPosition();
                measSpeed = 2*(position2 - position1)*600/360;
                if ((measSpeed/0.1) > speed){
                  setingSpeed = setingSpeed - 10;
                  motor.speed(setingSpeed);
                }  
              }
              if (speedValue == 0){
                setingSpeed = 0;
                motor.speed(setingSpeed);
              }
              speed = setingSpeed;
            }
          }
           Serial.print("Speed set \n");
        }
      }
  
  //-------Setting the motor to turn constantly----------
      if (subString1 == "constantTurn"){
        flag = true;
        Serial.print("I will constantly rotate the motor\n\n");
        motor.wheelMode();
        if (subString2 == "")
          motor.speed(100);
        else
        {
          value = subString2.toInt();
          speedValue = value/0.0977;
          if (speedValue > 1000)
            Serial.print("The speed value is out of range");
          else
          {
            speed = speedValue;
            motor.speed(speed);
          }
        }
        
      }
  //---------Reading the position from the motor-----------------------
      if (subString1 == "getPosition"){
        flag = true;
        serialString = String(motor.currentPosition()*0.29);
        Serial.print("Current Position: " + serialString + "\n");
      }
  
      //Prints an error if the command is not recognized
      if (flag == false)
      {
        Serial.print("command not recognized\n");
      }   
    }
  
}
