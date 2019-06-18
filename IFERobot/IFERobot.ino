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
  serial->sendMsg("Hello Father\n");
  serial->sendMsg("I have awakened\n");

  compass = Compass::getInstance();
  sound_sensor = SoundSensor::getInstance();
  speed_sensor = SpeedSensor::getInstance();
  engine = Engine::getInstance();

  engine->straight(0);

  delay(2000);

  //calculating intitial angle based on average of multiple samples
  double initial_angle = compass->getCurrentAngle();

  serial->sendMsg("\n initial_angle = %f", initial_angle);
  serial->sendMsg("\n my journey begins");
  ai = PathAI::getInstance();
}


void loop(void) { 
  // serial->sendMsg("\nangle=%f", compass->getCurrentAngle());
  compass->update();
  sound_sensor->update();

  auto serial_buffer = serial->getBuffer();
  if(serial_buffer) {
    serial->clearBuffer();
    ai->handleBluetoothSerial(serial_buffer);
  }
  
  ai->drive();
}

void SerialEvent() {
  serial->readFromBluetooth();
}
