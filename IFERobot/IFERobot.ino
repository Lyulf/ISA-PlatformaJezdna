#include "PathAI.h"
#include "SpeedSensor.h"
#include "SoundSensor.h"
#include "Compass.h"
#include "SerialPort.h"
#include "Engine.h"
#include "Variables.h"
#include <math.h>
#include <string>

SoundSensor* sound_sensor;
SpeedSensor* speed_sensor;
Compass* compass;
SerialPort* serial;
PathAI* ai;

void setup(void) {
  // Inicjalizacja
  sound_sensor = SoundSensor::getInstance();
  initEngine();
  engineGoStraight(0);
  speed_sensor = SpeedSensor::getInstance();
  Wire.begin();
  compass = Compass::getInstance();

  serial = SerialPort::getInstance();
  serial->sendMsg("Hello Father\n");

  delay(2000);

  //finds initial angle based on average of measurements

  serial->sendMsg("I have awakened\n");
  
  //calculating intitial angle based on average of multiple samples
  double initial_angle = compass->getCurrentAngle();

  serial->sendMsg("\n initial_angle = %f", initial_angle);
  ai = PathAI::getInstance();
}


void loop(void) { 
  if(Serial1.available()) {
    serial->readFromBluetooth();
    auto serial_buffer = serial->getSerialBuffer();
    ai->handleBluetoothSerial(serial_buffer);
  }

  else if(!Serial.available()) {
    auto serial_buffer = serial->getSerialBuffer();
    serial_buffer = String("");
  }
  
  if (driving_mode == 0) {
    driveStraight();
  }

  else if(driving_mode == 1 || driving_mode == -1) {
    turn(driving_mode);
  }
}