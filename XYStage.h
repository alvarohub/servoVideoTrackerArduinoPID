#include "Arduino.h"
#include <Servo.h> 

// PID CONTROL
#include <PID_v1.h>

#define pinServoX 10
#define pinServoY 9
#define centerPositionX 90
#define centerPositionY 90

// Input from the PC (0 will correnspod to centerPositionX/Y, MAX to 180 and MIN to 0 when in direct mode)
#define MIN (-90)
#define MAX 90 

#define PID_PERIOD_UPDATE 10 // in ms (note: if we call update ONLY when receiveing a new error, then it is better to set this to 0)

enum TrackMode {STOP=0, PID_TRACK};

class XYStage {
  public:
   
   XYStage();
   
  void initServos();

  void setPIDFactors(float kp, float kd, float ki) {
  Kp_H=kp; Kp_V=kp;
  Kd_H=kd; Kd_V=kd;
  Ki_H=ki; Ki_V=ki;
}

  void getMV(float& MV_H, float& MV_V);
  void setSP(float SP_H, float SP_V);
  void setPV(float PV_H, float PV_V);
  void setDirectPos(int x, int y);

  void setTrackerMode(TrackMode _newmode) {modeTracker=_newmode;}
  TrackMode getTrackerMode() {return(modeTracker);}
  
  void updateServo( );

  //Define Variables we'll be "connecting" to in the PID:
  double Setpoint_H, Input_H, Output_H;
  double Setpoint_V, Input_V, Output_V;
  // NOTE: the Setpoing will be equal to ZERO, while the INPUT will be the measured bias from the center of the screen. The Output will be 
  // the speed (negative or positive) of the servo motion. 
  //Specify tuning parameters:
  double Kp_H, Ki_H, Kd_H;
  double Kp_V, Ki_V, Kd_V; 
  PID* myPID_H, *myPID_V; // NOTE: maybe it would be better to use polar coordinates?
  
  private:
unsigned long lastTimeUpdate;
  Servo servoVertical, servoHorizontal;
  
  float currentPosX, currentPosY; 
  int oldCurrentPosX, oldCurrentPosY;
  float correctionX, correctionY;
  float proportionalFactor;
   
  TrackMode modeTracker;
};

extern XYStage myXYStage; 
