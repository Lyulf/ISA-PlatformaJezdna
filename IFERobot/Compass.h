#ifndef ISA_COMPASS_H
#define ISA_COMPASS_H

#include "QMC5883.h"
#include "SerialPort.h"
#include "Queue.h"
#include "Variables.h"

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

    void update();
    void oneTimeUpdate();
    static Compass* getInstance();

private:
    SerialPort* serial;
    Queue<double, NUMBER_OF_INITIAL_SAMPLES> angle_buffer;
    double right_angles[Direction::ALL];
};

#endif
