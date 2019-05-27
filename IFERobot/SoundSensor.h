#ifndef ISA_SOUND_SENSOR_H
#define ISA_SOUND_SENSOR_H

/*
 * Czujniki odległości
 * Moduł HC-SR04
 */

#define US_FRONT				0
#define US_FRONT_TRIGGER_PIN	8
#define US_FRONT_ECHO_PIN		9

#define US_BACK					1
#define US_BACK_TRIGGER_PIN		10
#define US_BACK_ECHO_PIN		11

#define US_LEFT					2
#define US_LEFT_TRIGGER_PIN		6
#define US_LEFT_ECHO_PIN		7

#define US_RIGHT				3
#define US_RIGHT_TRIGGER_PIN	4
#define US_RIGHT_ECHO_PIN		5

struct UltraSoundSensor {
	enum {
		Front = 0,
		Back = 1,
		Left = 2,
		Right = 3,
	
		__first = Front,
		__last = Right,
	
		All,
	};
};
 
extern int ultrasound_trigger_pin[];
extern int ultrasound_echo_pin[];

extern int d[];
extern int sum ;
extern int id;

extern double distance_measured[];
extern int front_obstruction_filter;
extern int side_obstruction_filter;
extern int dist;

void initUltraSoundSensor();
int measureSoundSpeed(int trigger_pin, int echo_pin);
int getFrontDistance();
int getLeftDistance();
int getRightDistance();

#endif