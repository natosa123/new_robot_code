//
//  followTape.h
//  
//
//  Created by Nathaniel osachoff on 23/7/14.
//
//

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
    if ((timeSinceDiffErr > 75) && (error == 5))
    {
      RCServo1.write(servoPosition);
      motor.speed(rightWheelPin, 1023) ;
      motor.speed(leftWheelPin, -1023) ;
      return;
    }
    else if ((timeSinceDiffErr > 75) && (error == -5))
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
