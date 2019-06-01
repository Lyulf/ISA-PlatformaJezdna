#ifndef ISA_COMPASS_H
#define ISA_COMPASS_H

#include "QMC5883.h"

extern QMC5883 qmc;
extern double right_angles[];
extern double current_angle;

double getCurrentAngle();
void calibrateRightAngles();
double angleDifference(double current, double target);

#endif