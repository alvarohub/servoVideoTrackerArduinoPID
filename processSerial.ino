// COMMANDS:
#define NUMBER_END '#'
#define STAND_BY 'S'
#define RESUME 'T'
#define RESET 'R'

int value;
char incomingByte;
char myStringData[25];
int indexString = -1;
int numberStack[4];
int indexStack = -1;

void processSerial() {
  while (Serial.available() > 0) {
    
    incomingByte = Serial.read();

    // Store string for number:
    if (incomingByte >= '-' && incomingByte <= '9') {
      myStringData[++indexString] = incomingByte;
    }

    // Store number in data array:
    else if (incomingByte == '#') {
      myStringData[indexString + 1] = 0; // must have an end 0 to use atoi... (use parseInt instead?)
      indexString = -1;
      value = atoi(myStringData);
      numberStack[++indexStack] = value;

      // use the data?
      if (indexStack==1) { // if we received TWO numbers:
        if (myXYStage.getTrackerMode()==STOP) { // then just move the servos:
           myXYStage.setDirectPos(numberStack[indexStack - 1], numberStack[indexStack]); // FIRST X, then Y
        } else {
           myXYStage.setPV(numberStack[indexStack - 1], numberStack[indexStack]); // FIRST X, then Y
        }
        //myXYStage.updateServo(); 
        indexStack =-1;//-= 2; // two numbers "consumed" from stack
      }

    }

    else { // COMAND PARSING

      switch (incomingByte) {
        case STAND_BY:
          myXYStage.setTrackerMode(STOP);
          break;
        case RESUME:
          myXYStage.setTrackerMode(PID_TRACK);
          break;
        case RESET:
          //myXYStage.setTrackerMode(STOP);
          myXYStage.setDirectPos(0,0);
          break;
      }

    }

  }
}


