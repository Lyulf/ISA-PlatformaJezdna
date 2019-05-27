#include "Compass.h"
#include "SerialPort.h"
#include "Variables.h"
#include <string>
#include <math.h>

Compass::Compass()
  : right_angles() {
  QMC5883::init();
  //hardcoded values
  right_angles[Direction::NORTH] = NORTH_DIR;
  right_angles[Direction::EAST] = EAST_DIR;
  right_angles[Direction::SOUTH] = SOUTH_DIR;
  right_angles[Direction::WEST] = WEST_DIR;

  //find 4 right angles
  calibrateRightAngles();

  //right_angles[0] = initial_angle;
  for (int i = 0; i < 4; i++) {
    //right_angles[i] = initial_angle + 90 * i;
    //right_angles[i] -= right_angles[i] > 180 ? 360 : 0;
    
    sprintf(buffer, "\n right_angle[%d] = %f", i, right_angles[i]);
    Serial1.print(buffer);
  }
};

Compass* Compass::getInstance() {
  static Compass instance;
  return &instance;
}

double Compass::getCurrentAngle() {
  double avg_angle = 0;
  for (int i = 0; i < NUMBER_OF_INITIAL_SAMPLES; i++) {
    measure();
    int16_t x = getX();
    int16_t y = getY();
    avg_angle += (atan2(x, y) * 180 / PI);
    //delay(10);
  }

 return avg_angle / NUMBER_OF_INITIAL_SAMPLES;
}

void Compass::calibrateRightAngles() {
  sprintf(buffer, "\n please point me north and type: '+'");
  Serial1.print(buffer);
  requestChar('+');
  right_angles[Direction::NORTH] = getCurrentAngle();

  sprintf(buffer, "\n please point me east and type '+'");
  Serial1.print(buffer);
  requestChar('+');
  right_angles[Direction::EAST] = getCurrentAngle();

  sprintf(buffer, "\n please point me south and type '+'");
  Serial1.print(buffer);
  requestChar('+');
  right_angles[Direction::SOUTH] = getCurrentAngle();

  sprintf(buffer, "\n please point me west and type '+'");
  Serial1.print(buffer);
  requestChar('+');
  right_angles[Direction::WEST] = getCurrentAngle();
  
}

double Compass::getRightAngle(int direction) {
  if(direction < Direction::ALL) {
    return right_angles[direction];
  } else {
    return 0.0;
  }
}