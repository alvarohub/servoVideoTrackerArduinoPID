
/* Name: servoVideoTrackerArduinoPID
   Ver 5.8.2015, for Arduino 1.6.4
   Author: Alvaro Cassinelli
   
   Notes: This is to be used with the servoVideoTracker program (OF). 
          Two servo motors control a camera, the camera is connected to the computer and the arduino communicates with the computer on the serial port. 
          Tracking of the face is done by the servoVideoTracker app, which send commands to mantain the face in the center of the video. 
          A PID controller is used to control the speed of the servos. 
*/

#include <Servo.h> // NOTE: Servo cannot be user at the same time with TimerOne...
#include <PID_v1.h>
#include "XYStage.h" // a preinstantiated xyStage can be used from here

void setup(){
   
    Serial.begin(115200);
     
     // Initialize the object (this is basically attaching the pins to the servos - it cannot be done in the constructor because we cannot be sure
     // the servo objects are initialized (in servo.cpp) - check this: http://arduino.stackexchange.com/questions/3658/arduino-servo-wont-move-when-using-classes
    // "A common problem for people who write object oriented code for Arduino: instantiate an object of a class which in turn instantiates an object of another class. "
    // =>  USE INITIALIZERS and not calls to the member object in the constructor of the ("upper") class.
    myXYStage.initServos(); 
    myXYStage.setTrackerMode(STOP);  
}
 

void loop(){
  
 // 1) Check if data is available in the serial port and process it:
 processSerial();
  
  // 2) Update servos (cannot use interrupt with timer1, so I will use an internal time keeping method):
   myXYStage.updateServo();
}

