#include "SoundSensor.h"
#include <Wire.h>

int ultrasound_trigger_pin[] = {
	[UltraSoundSensor::Front]	= US_FRONT_TRIGGER_PIN,
	[UltraSoundSensor::Back]	= US_BACK_TRIGGER_PIN,
	[UltraSoundSensor::Left]	= US_LEFT_TRIGGER_PIN,
	[UltraSoundSensor::Right]	= US_RIGHT_TRIGGER_PIN,
};

 
int ultrasound_echo_pin[] = {
	[UltraSoundSensor::Front]	= US_FRONT_ECHO_PIN,
	[UltraSoundSensor::Back]	= US_BACK_ECHO_PIN,
	[UltraSoundSensor::Left]	= US_LEFT_ECHO_PIN,
	[UltraSoundSensor::Right]	= US_RIGHT_ECHO_PIN,
};

int d[5] = {};
int sum = 0;
int id = 0;

double distance_measured[20];
int front_obstruction_filter = 0;
int side_obstruction_filter = 0;
int dist;

void initUltraSoundSensor() {
  for (int i = (int)UltraSoundSensor::__first; i <= (int)UltraSoundSensor::__last; i++)
  {
    pinMode(ultrasound_trigger_pin[i], OUTPUT);
    pinMode(ultrasound_echo_pin[i], INPUT);

    digitalWrite(ultrasound_trigger_pin[i], 0);
  }
}

int measureSoundSpeed(int trigger_pin, int echo_pin)
{
  digitalWrite(trigger_pin, false);
  delayMicroseconds(2);

  digitalWrite(trigger_pin, true);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, false);

  // zmierz czas przelotu fali dźwiękowej
  int duration = pulseIn(echo_pin, true, 50 * 1000);

  // przelicz czas na odległość (1/2 Vsound(t=20st.C))
  int distance = (int)((float)duration * 0.03438f * 0.5f);
  return distance;
}

int getFrontDistance() {
    //zbieranie dystansu z przodu
  int dist = measureSoundSpeed(
               ultrasound_trigger_pin[(int)UltraSoundSensor::Front],
               ultrasound_echo_pin[(int)UltraSoundSensor::Front]);

  // średnia krocząca z pomiarów dystansu z przodu
  sum -= d[id];
  sum += d[id] = dist;
  id = (id + 1) % 5;
  dist = sum / 5;

  return dist;
}

int getRightDistance() {
  int dist = measureSoundSpeed(
               ultrasound_trigger_pin[(int)UltraSoundSensor::Right],
               ultrasound_echo_pin[(int)UltraSoundSensor::Right]);
               
  return dist;
}

int getLeftDistance() {
  int dist = measureSoundSpeed(
               ultrasound_trigger_pin[(int)UltraSoundSensor::Front],
               ultrasound_echo_pin[(int)UltraSoundSensor::Front]);

  return dist;
}
