//
//  followTape.h
//  
//
//  Created by Nathaniel osachoff on 23/7/14.
//
//

//****TAKE OUT VARIABLES DECLARATIONS AND MOVE TO MAIN FUNCTION SO THEY ARE ONLY DECLARDED ONCE*******
//***START COPYING******


// Tape following Pins
int leftTapeQRD = 3;
int rightTapeQRD = 2;

//thresholds for tape following QRDs
int leftThreshold = 485;
int rightThreshold = 120;

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
//****END COPYING****

#ifndef _followTape_h
#define _followTape_h

inline void followTape()
{
    bool leftSensor = (analogRead(leftTapeQRD) > leftThreshold);
    bool rightSensor = (analogRead(rightTapeQRD) > rightThreshold);
    
    if (leftSensor && rightSensor) // on tape
    {
        error = 0;
        rightMotorOutput = motorSpeed;
        leftMotorOutput = motorSpeed;
    }
    else if (leftSensor) //too far right
    {
        error = 1;
        rightMotorOutput = motorSpeed;
        leftMotorOutput = motorSpeed;
    }
    else if (rightSensor) //too far left
    {
        error = -1;
        rightMotorOutput = motorSpeed;
        leftMotorOutput = motorSpeed;
    }
    else
    {
        if (lasterror < 0) //way too far left
        {
            error = -5;
            rightMotorOutput = motorSpeed*(1-motorAdjustment);
            leftMotorOutput = motorSpeed*(1+motorAdjustment);
        }
        if (lasterror > 0)  // way too far right
        {
            error = 5;
            rightMotorOutput = motorSpeed*(1+motorAdjustment);
            leftMotorOutput = motorSpeed*(1-motorAdjustment);
        }
    }
    if (lasterror != error)
    {
        lasterror = error;
        preverrorDuration = timeSinceDiffErr;
        timeSinceDiffErr = 0;
    }
    timeSinceDiffErr = timeSinceDiffErr + 1;
    if ((timeSinceDiffErr > 50) && (error == 5))
    {
      RCServo1.write(servoPosition);
      motor.speed(rightWheelPin, 1023) ;
      motor.speed(leftWheelPin, -1023) ;
      return;
    }
    else if ((timeSinceDiffErr > 50) && (error == -5))
      {
        RCServo1.write(servoPosition);
        motor.speed(rightWheelPin, -1023) ;
        motor.speed(leftWheelPin, 1023) ;
        return;
      }
    
    PIDAdjustment = (int)((float)kP * (float)error) + (int)((float)kD * (float)(error - lasterror) / (float)(preverrorDuration + timeSinceDiffErr) );
    servoPosition = neutral+PIDAdjustment;
    if (servoPosition>146)
        servoPosition = 146;
    else if (servoPosition<42)
        servoPosition = 42;
    RCServo1.write(servoPosition);
    motor.speed(rightWheelPin, rightMotorOutput) ;
    motor.speed(leftWheelPin, leftMotorOutput) ;
    LCD.setCursor(0,0);  LCD.clear();  LCD.home() ;
    
    
}


#endif
