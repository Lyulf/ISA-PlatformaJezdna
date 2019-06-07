#include "SoundSensor.h"
#include <Wire.h>

SoundSensor::SoundSensor()
  : d{0}, sum(0), id(0), distance_measured{0} { 
  ultrasound_trigger_pin[UltraSoundSensor::Front]	= US_FRONT_TRIGGER_PIN;
  ultrasound_trigger_pin[UltraSoundSensor::Back]	= US_BACK_TRIGGER_PIN;
  ultrasound_trigger_pin[UltraSoundSensor::Left]	= US_LEFT_TRIGGER_PIN;
  ultrasound_trigger_pin[UltraSoundSensor::Right]	= US_RIGHT_TRIGGER_PIN;

  ultrasound_echo_pin[UltraSoundSensor::Front]	= US_FRONT_ECHO_PIN;
  ultrasound_echo_pin[UltraSoundSensor::Back]	= US_BACK_ECHO_PIN;
  ultrasound_echo_pin[UltraSoundSensor::Left]	= US_LEFT_ECHO_PIN;
  ultrasound_echo_pin[UltraSoundSensor::Right]	= US_RIGHT_ECHO_PIN;

  for (int i = UltraSoundSensor::__first; i <= UltraSoundSensor::__last; i++)
  {
    pinMode(ultrasound_trigger_pin[i], OUTPUT);
    pinMode(ultrasound_echo_pin[i], INPUT);

    digitalWrite(ultrasound_trigger_pin[i], 0);
  }
}

int SoundSensor::measureSoundSpeed(int trigger_pin, int echo_pin)
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

int SoundSensor::getFrontDistance() {
    //zbieranie dystansu z przodu
  int dist = measureSoundSpeed(
               ultrasound_trigger_pin[UltraSoundSensor::Front],
               ultrasound_echo_pin[UltraSoundSensor::Front]);

  // średnia krocząca z pomiarów dystansu z przodu
  sum -= d[id];
  sum += d[id] = dist;
  id = (id + 1) % 5;
  dist = sum / 5;

  return dist;
}

int SoundSensor::getLeftDistance() {
  int dist = measureSoundSpeed(
               ultrasound_trigger_pin[UltraSoundSensor::Front],
               ultrasound_echo_pin[UltraSoundSensor::Front]);

  return dist;
}

int SoundSensor::getRightDistance() {
  int dist = measureSoundSpeed(
               ultrasound_trigger_pin[UltraSoundSensor::Right],
               ultrasound_echo_pin[UltraSoundSensor::Right]);
               
  return dist;
}

SoundSensor* SoundSensor::getInstance() {
  static SoundSensor instance;
  return &instance;
}