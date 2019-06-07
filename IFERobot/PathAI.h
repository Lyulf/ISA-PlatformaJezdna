#ifndef ISA_PATH_AI_H
#define ISA_PATH_AI_H

#include "Compass.h"
#include "SerialPort.h"
#include "Engine.h"
#include "SoundSensor.h"
#include <Arduino.h>

class PathAI {
    PathAI();
public:
    void handleBluetoothSerial(String bt_command);

	void driveStraight();
	void turn(int dir);
	void correctTargetAngle(double& angle);
    int getDrivingMode();

    static PathAI* getInstance();

private:
    SerialPort* serial;
    Compass* compass;
    Engine* engine;
	SoundSensor* sound_sensor;
    double driving_tendency_angle;
	double target_angle;
	int front_obstruction_filter;
	int side_obstruction_filter;
    int driving_mode = 0;
};

#endif
