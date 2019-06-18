#include "SoundSensor.h"
#include "Utils.h"
#include <Wire.h>

SoundSensor::SoundSensor() { 
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

int SoundSensor::getFrontDistance() const {
  return front_buffer.avg();
}

int SoundSensor::getBackDistance() const {
  return back_buffer.avg();
}

int SoundSensor::getLeftDistance() const {
  return left_buffer.avg();
}

int SoundSensor::getRightDistance() const {
  return right_buffer.avg();
}

void SoundSensor::update() {
  updateFront();
  updateBack();
  updateLeft();
  updateRight();
}

void SoundSensor::updateFront() {
    //zbieranie dystansu z przodu
  int dist = measureSoundSpeed(
               ultrasound_trigger_pin[UltraSoundSensor::Front],
               ultrasound_echo_pin[UltraSoundSensor::Front]);

  forcePush(front_buffer, dist);
}

void SoundSensor::updateBack() {
    //zbieranie dystansu z tylu
  int dist = measureSoundSpeed(
               ultrasound_trigger_pin[UltraSoundSensor::Back],
               ultrasound_echo_pin[UltraSoundSensor::Back]);
  forcePush(back_buffer, dist);
}

void SoundSensor::updateLeft() {
  //zbieranie dystansu z lewej strony
  int dist = measureSoundSpeed(
               ultrasound_trigger_pin[UltraSoundSensor::Left],
               ultrasound_echo_pin[UltraSoundSensor::Left]);
  forcePush(left_buffer, dist);
}

void SoundSensor::updateRight() {
  //zbieranie dystansu z prawej strony
  int dist = measureSoundSpeed(
               ultrasound_trigger_pin[UltraSoundSensor::Right],
               ultrasound_echo_pin[UltraSoundSensor::Right]);
  forcePush(right_buffer, dist);
}

SoundSensor* SoundSensor::getInstance() {
  static SoundSensor instance;
  return &instance;
}