#include "SpeedSensor.h"
#include <Wire.h>

void initEncoder() {
  pinMode(ENCODER_LEFT, INPUT);
  pinMode(ENCODER_RIGHT, INPUT);
}
