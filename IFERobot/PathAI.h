#ifndef ISA_PATH_AI_H
#define ISA_PATH_AI_H

#include "Compass.h"
#include "SerialPort.h"
#include <Arduino.h>

class PathAI {
    PathAI();
public:
    void handleBluetoothSerial(String bt_command);

    static PathAI* getInstance();

private:
    SerialPort* serial;
    Compass* compass;
    double driving_tendency_angle;
};

#endif
