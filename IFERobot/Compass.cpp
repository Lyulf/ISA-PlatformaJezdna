#include "Compass.h"
#include "SerialPort.h"
#include "Variables.h"
#include <string>
#include <math.h>

QMC5883 qmc;
double right_angles[4];
double current_angle;

double getCurrentAngle() {
  double avg_angle = 0;
  for (int i = 0; i < NUMBER_OF_INITIAL_SAMPLES; i++) {
    qmc.measure();
    int16_t x = qmc.getX();
    int16_t y = qmc.getY();
    avg_angle += (atan2(x, y) * 180 / PI);
    //delay(10);
  }

 return avg_angle / NUMBER_OF_INITIAL_SAMPLES;
}

void calibrateRightAngles() {
  sprintf(buffer, "\n please point me north and type: '+'");
  Serial1.print(buffer);
  requestChar('+');
  right_angles[0] = getCurrentAngle();

  sprintf(buffer, "\n please point me east and type '+'");
  Serial1.print(buffer);
  requestChar('+');
  right_angles[1] = getCurrentAngle();

  sprintf(buffer, "\n please point me south and type '+'");
  Serial1.print(buffer);
  requestChar('+');
  right_angles[2] = getCurrentAngle();

  sprintf(buffer, "\n please point me west and type '+'");
  Serial1.print(buffer);
  requestChar('+');
  right_angles[3] = getCurrentAngle();
  
}

/* Funkcja zwracająca kąt który samochód musi się obrócić na podstawie
 jeśli difference(diff*/
double angleDifference(double current, double target) {
  double diff = target - current;
  if (diff > 180) {
    diff = diff - 360;
  }
  else if (diff < -180) {
    diff = diff + 360;
  }
  return diff;
}