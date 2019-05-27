#include "PathFinding.h"
#include "SpeedSensor.h"
#include "SoundSensor.h"
#include "Compass.h"
#include "SerialPort.h"
#include "Engine.h"
#include "Variables.h"
#include <math.h>
#include <string>

void setup(void) {
  // Inicjalizacja
  initUltraSoundSensor();
  initEngine();
  engineGoStraight(0);
  SpeedSensor speedSensor = SpeedSensor::getInstance();
  Wire.begin();
  qmc.init();

  Serial1.begin(9600); // HC06
  Serial1.print("Hello Father\n");

  delay(2000);

  //finds initial angle based on average of measurements

  Serial1.print("I have awakened\n");
  
  //calculating intitial angle based on average of multiple samples
  double initial_angle = getCurrentAngle();

  sprintf(buffer, "\n initial_angle = %f", initial_angle);
  Serial1.print(buffer);

  //find 4 right angles

  //hardcoded values
  right_angles[0] = NORTH_DIR;
  right_angles[1] = EAST_DIR;
  right_angles[2] = SOUTH_DIR;
  right_angles[3] = WEST_DIR;

  calibrateRightAngles();

  //right_angles[0] = initial_angle;
  for (int i = 0; i < 4; i++) {
    //right_angles[i] = initial_angle + 90 * i;
    //right_angles[i] -= right_angles[i] > 180 ? 360 : 0;
    
    sprintf(buffer, "\n right_angle[%d] = %f", i, right_angles[i]);
    Serial1.print(buffer);
  }
}


void loop(void) { 
  if(Serial1.available()) {
    readFromBluetooth();
    handleBluetoothSerial(serial_buffer);
  }

  else if(!Serial.available()) {
    serial_buffer = String("");
  }
  
  if (driving_mode == 0) {
    driveStraight();
  }

  else if(driving_mode == 1 || driving_mode == -1) {
    turn(driving_mode);
  }
}