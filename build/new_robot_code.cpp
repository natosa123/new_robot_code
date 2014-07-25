#include <phys253.h>        		//***** from 253 template file
#include <LiquidCrystal.h>  		//***** from 253 template file
#include <servo253.h>       		//***** from 253 template file 


// Tape following Pins
#include "WProgram.h"
#include <HardwareSerial.h>
void setup();
void loop();
int leftTapeQRD = 3;
int rightTapeQRD = 2;

//thresholds for tape following QRDs
int leftThreshold = 470;
int rightThreshold = 100;

//idol detector (qrd) pin
int idolSensorPin = 1;

//threshold for idol sensor
int idolSensorThreshold = 300 ;

// Motor Speed Pins (motor outputs)
int rightWheelPin = 1;
int leftWheelPin = 0;

//Knobs
int knob_one = 6;
int knob_two = 7;

//PID parameters
int kP = 100;
int kD = 20;

//correction errors (negative is too far left, positive is too far right)
int error = 0; 
int lasterror = 0;
int preverrorDuration = 0;
int timeSinceDiffErr = 1;

//turning parameters
float motorAdjustment = 0.5;
int rightMotorOutput = 0;
int leftMotorOutput = 0;
int PIDAdjustment = 0;

// wheel controll
int startMotorSpeed = 375;
int motorSpeed = 150;
int sharpTurnSpeed = 100;
int neutral = 94;
int servoPosition = neutral;

// debugging
int counter = 1;

// Including Outside Files
#include <followTape.h>

void setup() 
{
  //to prevent fuse from blowing
  motor.speed(leftWheelPin, 0)  ;
  motor.speed(rightWheelPin, 0)  ;
  
  //attach driving servo
  RCServo1.attach(RCServo1Output) ;
  RCServo1.write(neutral)  ; 
  
  //set ports to inputs
  portMode(0, INPUT) ;      	 
  portMode(1, INPUT) ;
  
  Serial.begin(9600);
  
    while(1){
    delay(50);
    while( !(stopbutton()) ){
      delay(50);
        LCD.setCursor(0,0);  LCD.clear();  LCD.home() ;
        LCD.print("kP");
        LCD.print(" ");
        LCD.print(kP);
        kP = knob(knob_one)/10; 
        LCD.print(" ");
        LCD.setCursor(0,1);
        LCD.print("kD");
        LCD.print(" ");
        LCD.print(kD);
        kD = knob(knob_two)/10;
        if (startbutton())
        {
          goto loopstart;
        }
    }
    delay(50);
    while( !(stopbutton())) { 
        delay(50);
        LCD.setCursor(0,0);  LCD.clear();  LCD.home() ;
        LCD.print("motorSpeed");
        LCD.print(" ");
        LCD.print(motorSpeed);
        motorSpeed = knob(knob_two);
        LCD.setCursor(0,1);
        LCD.print("Adjust");
        LCD.print(" ");
        LCD.print(knob(knob_one));
        motorAdjustment = knob(knob_one)/512;
        if (startbutton())
        {
          goto loopstart;
        }
    }
    
  }
        loopstart: ;
}

void loop() 
{
   followTape();
   LCD.setCursor(0,0);  LCD.clear();  LCD.home() ;
   LCD.print("L");
   LCD.print(" ");
   LCD.print(analogRead(leftTapeQRD));
   LCD.print(" ");
   LCD.print("R");
   LCD.print(" ");
   LCD.print(analogRead(rightTapeQRD));
   LCD.setCursor(0,1); 
   LCD.print("A");
   LCD.print(" ");
   LCD.print(servoPosition);
   LCD.print(" ");
   LCD.print("E");
   LCD.print(" ");
   LCD.print(error);
}


  


