#ifndef ISA_COMPASS_H
#define ISA_COMPASS_H

#include "QMC5883.h"

struct Direction {
    enum {
        NORTH = 0,
        EAST,
        SOUTH,
        WEST,
        ALL
    };
};

class Compass : public QMC5883 {
    Compass();
public:
    double getCurrentAngle();
    void calibrateRightAngles();
    double getRightAngle(int direction);

    static Compass* getInstance();
private:
    double right_angles[4];
};

#endif