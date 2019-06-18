#include "Compass.h"
#include "Utils.h"
#include "Variables.h"
#include <math.h>
#include <string>

Compass::Compass()
  : serial(SerialPort::getInstance()), angle_buffer() {
  QMC5883::init();
  update();
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
    
    serial->sendMsg("\n right_angle[%d] = %f", i, right_angles[i]);
  }
};

double Compass::getCurrentAngle() {
  return angle_buffer.avg();
}

void Compass::calibrateRightAngles() {
  if(getCurrentAngle() == -135.0) {
    serial->sendMsg("\n WARNING: THE COMPASS SEEMS TO BE BROKEN");
  }
  serial->sendRequest("\n please point me north and type: '+'");
  oneTimeUpdate();
  right_angles[Direction::NORTH] = getCurrentAngle();
  serial->sendMsg("\n north =\t%f", right_angles[Direction::NORTH]);

  serial->sendRequest("\n please point me east and type '+'");
  oneTimeUpdate();
  right_angles[Direction::EAST] = getCurrentAngle();
  serial->sendMsg("\n east =\t%f", right_angles[Direction::EAST]);

  serial->sendRequest("\n please point me south and type '+'");
  oneTimeUpdate();
  right_angles[Direction::SOUTH] = getCurrentAngle();
  serial->sendMsg("\n south =\t%f", right_angles[Direction::SOUTH]);

  serial->sendRequest("\n please point me west and type '+'");
  oneTimeUpdate();
  right_angles[Direction::WEST] = getCurrentAngle();
  serial->sendMsg("\n west =\t%f", right_angles[Direction::WEST]);

  serial->sendRequest("\n please point me north again and type '+'");
}

double Compass::getRightAngle(int direction) {
  if(direction < Direction::ALL) {
    return right_angles[direction];
  } else {
    return 0.0;
  }
}

void Compass::oneTimeUpdate() {
  for(int i = 0; i < NUMBER_OF_INITIAL_SAMPLES; i++) {
    update();
  }
}

void Compass::update() {
  double avg_angle = 0;
  measure();
  int16_t x = getX();
  int16_t y = getY();
  forcePush(angle_buffer, atan2(x, y) * 180 / PI);
}

Compass* Compass::getInstance() {
  static Compass instance;
  return &instance;
}
