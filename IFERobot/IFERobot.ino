#include "PathFinding.h"
#include "SpeedSensor.h"
#include "SoundSensor.h"
#include "Compass.h"
#include "SerialPort.h"
#include "Engine.h"
#include "Variables.h"
#include <math.h>
#include <string>

SpeedSensor* speedSensor;
Compass* compass;

void setup(void) {
  // Inicjalizacja
  initUltraSoundSensor();
  initEngine();
  engineGoStraight(0);
  speedSensor = SpeedSensor::getInstance();
  Wire.begin();
  compass = Compass::getInstance();

  Serial1.begin(9600); // HC06
  Serial1.print("Hello Father\n");

  delay(2000);

  //finds initial angle based on average of measurements

  Serial1.print("I have awakened\n");
  
  //calculating intitial angle based on average of multiple samples
  double initial_angle = compass->getCurrentAngle();

  sprintf(buffer, "\n initial_angle = %f", initial_angle);
  Serial1.print(buffer);
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