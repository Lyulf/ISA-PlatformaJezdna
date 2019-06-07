#ifndef ISA_ENGINE_H
#define ISA_ENGINE_H

/*
 * Silniki, sterowanie PWM
 * 
 */
 
#define A_ENABLE		2	//PWM
#define A_PHASE			46  //0 - przod, 1 - tyl DIGITAL
	
#define B_ENABLE		3	//PWM
#define B_PHASE			48	//0 - przod, 1 - tyl DIGITAL
#define MODE			44	//sterowanie 0 - ciężkie, 1 - uproszczone DIGITAL
							// Pin MODE sterownika jest na stałe podpięty do +5V (HIGH)

/*
 * Silnik, sterowanie PWM
 * L298N (https://botland.com.pl/pl/sterowniki-silnikow-moduly/3164-l298n-dwukanalowy-sterownik-silnikow-modul-12v2a.html)
 */

#define LEFT_PWM		3
#define LEFT_IN1		47	// in1 (l298n)
#define LEFT_IN2		46	// in2 (l298n)

#define RIGHT_PWM		2
#define RIGHT_IN1		45	// in3 (l298n)
#define RIGHT_IN2		44	// in4 (l298n)

#include "Compass.h"
#include "SerialPort.h"
#include "SoundSensor.h"

enum class EngineSelector
{
	Left,
	Right
};

extern int driving_mode;

class Engine {
	Engine();
public:
	void setPowerLevel(EngineSelector side, int level);

	void driveStraight();
	void engineTurn(int left_axis_power, int right_axis_power);
	void engineGoStraight(int both_axis_power);

	void turn(int dir);
	void correctTargetAngle(double& angle);

	static Engine* getInstance();

private:
	SerialPort* serial;
	Compass* compass;
	SoundSensor* sound_sensor;
	double target_angle;
	int front_obstruction_filter;
	int side_obstruction_filter;
};

#endif