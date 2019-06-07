#include "Compass.h"
#include "Engine.h"
#include "PathAI.h"
#include "SerialPort.h"
#include "SoundSensor.h"
#include "SpeedSensor.h"
#include "Variables.h"

SerialPort* serial;
Compass* compass;
SoundSensor* sound_sensor;
SpeedSensor* speed_sensor;
Engine* engine;
PathAI* ai;

void setup(void) {
  // Inicjalizacja
  Wire.begin();
  serial = SerialPort::getInstance();
  compass = Compass::getInstance();
  sound_sensor = SoundSensor::getInstance();
  speed_sensor = SpeedSensor::getInstance();
  engine = Engine::getInstance();

  engine->straight(0);

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
    auto serial_buffer = serial->getBuffer();
    ai->handleBluetoothSerial(serial_buffer);
  }

  else if(!Serial.available()) {
    serial->clearBuffer();
  }
  
  auto driving_mode = ai->getDrivingMode();
  if (driving_mode == 0) {
    ai->driveStraight();
  }

  else if(driving_mode == 1 || driving_mode == -1) {
    ai->turn(driving_mode);
  }
}
