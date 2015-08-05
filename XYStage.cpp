
#include "XYStage.h"
XYStage myXYStage;  // preinstantiated object

// Default constructor (parameterless)
XYStage::XYStage() {

}

void XYStage::initServos() { // Call this function once in the setup() 
  // NOTE: the attach method CANNOT be called in the constructor... servos may not be properly initialized
  // (see: http://arduino.stackexchange.com/questions/3658/arduino-servo-wont-move-when-using-classes )
  servoHorizontal.attach(pinServoX);
  servoVertical.attach(pinServoY);
  
  // use this variable to avoid reseting the servo position when it did not change (perhaps not a pb anyway...)
  oldCurrentPosX = -1; oldCurrentPosY = -1;
  setDirectPos(0,0);
  
  //PIDs setup:
  // setPIDFactors(0.006, 0, 0); // set factors for both pids at the same time. 
  setPIDFactors(0.0062, 0.00005, 0); // set factors for both pids at the same time. 
  myPID_H=new PID(&Input_H, &Output_H, &Setpoint_H, Kp_H, Ki_H, Kd_H, DIRECT);
  myPID_V=new PID(&Input_V, &Output_V, &Setpoint_V, Kp_V, Ki_V, Kd_V, DIRECT);
  
  // (1) Initial Setpoint:
  setSP(0,0);
  // (2) Initial Process variable:
  setPV(0,0);
  //updateServo();
  
  myPID_H->SetOutputLimits(-20, 20); // this are limits in DEGREES/s
  myPID_H->SetSampleTime(PID_PERIOD_UPDATE); // in ms
  myPID_V->SetOutputLimits(-20, 20); // this are limits in DEGREES/s
  myPID_V->SetSampleTime(PID_PERIOD_UPDATE); // in ms
  
  // Turn the PID ON:
  myPID_H->SetMode(AUTOMATIC);   
  myPID_V->SetMode(AUTOMATIC);  

  lastTimeUpdate=micros();
}
  
// This is to control the position of the servos DIRECTLY when in DIRECT mode ( between -90 and 90 for each axis) 
void XYStage::setDirectPos(int x, int y) { 
  currentPosX = (int)(1.0 * x / (MAX - MIN) * 180 + 90 - 5); // from -5 to 175 (from manual calibration)
  currentPosY = (int)(1.0 * y / (MAX - MIN) * 180 + 90 - 10);
}

void XYStage::setPV(float PV_H, float PV_V) {
  Input_H=PV_H;
  Input_V=PV_V;
}
void XYStage::setSP(float SP_H, float SP_V) {
  Setpoint_H=SP_H;
  Setpoint_V=SP_V;
}

void XYStage::getMV(float& MV_H, float& MV_V) {
 MV_H=Output_H; 
 MV_V=Output_V; 
}

void XYStage::updateServo() {
        if (modeTracker == PID_TRACK) {
          
           // Update pids:
          myPID_H->Compute();
          myPID_V->Compute();
  
//        float dt=micros()-lastTimeUpdate;
//        currentPosX+= 1.0*Output_H*dt/1000000 ; // note: Output is the speed in degrees/sec
//        currentPosY+= 1.0*Output_V*dt/1000000 ;
//        lastTimeUpdate=micros();

         currentPosX+= 0.02*Output_H ; // note: Output is the speed in degrees/sec
         currentPosY+= 0.02*Output_V ;
          
          if (currentPosX>175) currentPosX=175;
          else if (currentPosX<-5) currentPosX=-5;
          if (currentPosY>174) currentPosY=174;
          else if (currentPosY<-6) currentPosY=-6;
          
        }
        
      //  servoHorizontal.write((int)currentPosX);
       // servoVertical.write((int)currentPosY);
      
      // 2) change effective servo positions (if the variable changed):
      if ((oldCurrentPosX != (int)currentPosX) || (oldCurrentPosY != (int)currentPosY)) {
        oldCurrentPosX = currentPosX;
        oldCurrentPosY = currentPosY;
        
        servoHorizontal.write(currentPosX);
        servoVertical.write(currentPosY);
      }
}
