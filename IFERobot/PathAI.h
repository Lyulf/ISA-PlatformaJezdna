#ifndef ISA_PATH_AI_H
#define ISA_PATH_AI_H

#include "Compass.h"
#include "SerialPort.h"
#include "Engine.h"
#include "SoundSensor.h"
#include <Arduino.h>
#include "Stack.h"
#include <cstdint>
#include <Fuzzy.h>

struct DrivingMode {
    enum Mode {
        Stopping,
        DrivingStraight,
        TurningLeft,
        TurningRight,
        LookingForPath
    };
};

class PathAI {
    PathAI();
public:
    void handleBluetoothSerial(String bt_command);
    void drive();
    int getDrivingMode();

    static PathAI* getInstance();

private:
	void driveStraight();
	void turn(int dir);
    void stop();
	void correctTargetAngle(double& angle);

    SerialPort* serial;
    Compass* compass;
    Engine* engine;
	SoundSensor* sound_sensor;
    Stack directions;
    double driving_tendency_angle;
	double target_angle;
    DrivingMode::Mode driving_mode;
    unsigned int wait_ms;
    Fuzzy* fuzzy;
};

#endif
