#ifndef ISA_SOUND_SENSOR_H
#define ISA_SOUND_SENSOR_H

/*
 * Czujniki odległości
 * Moduł HC-SR04
 */

#include "Queue.h"
#include "Variables.h"

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

class SoundSensor {
	SoundSensor();
public:
	int measureSoundSpeed(int trigger_pin, int echo_pin);
	int getFrontDistance() const;
	int getBackDistance() const;
	int getLeftDistance() const;
	int getRightDistance() const;

	void update();
	static SoundSensor* getInstance();

private:
	void updateFront();
	void updateBack();
	void updateLeft();
	void updateRight();

	int ultrasound_trigger_pin[UltraSoundSensor::All];
	int ultrasound_echo_pin[UltraSoundSensor::All];
	Queue<int, SOUND_SENSOR_BUFFER_SIZE> front_buffer;
	Queue<int, SOUND_SENSOR_BUFFER_SIZE> back_buffer;
	Queue<int, SOUND_SENSOR_BUFFER_SIZE> left_buffer;
	Queue<int, SOUND_SENSOR_BUFFER_SIZE> right_buffer;
}; 

#endif