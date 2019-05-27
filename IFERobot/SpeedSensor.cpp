#include "SpeedSensor.h"
#include <Wire.h>

SpeedSensor::SpeedSensor() {
  pinMode(ENCODER_LEFT, INPUT);
  pinMode(ENCODER_RIGHT, INPUT);
}

SpeedSensor& SpeedSensor::getInstance() {
  static SpeedSensor instance;
  return instance;
}