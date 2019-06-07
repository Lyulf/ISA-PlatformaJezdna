#include "Engine.h"

Engine::Engine()
  : compass(Compass::getInstance()),
    serial(SerialPort::getInstance()) {
  pinMode(LEFT_PWM, OUTPUT);
  pinMode(LEFT_IN1, OUTPUT);
  pinMode(LEFT_IN2, OUTPUT);
  
  pinMode(RIGHT_PWM, OUTPUT);
  pinMode(RIGHT_IN1, OUTPUT);
  pinMode(RIGHT_IN2, OUTPUT);
}

void Engine::setPowerLevel(EngineSelector side, int level)
{
  level = constrain(level, -255, 255);

  if (side == EngineSelector::Left) {
    if (level > 0) {
      // do przodu
      digitalWrite(LEFT_IN1, false);
      digitalWrite(LEFT_IN2, true);
      analogWrite(LEFT_PWM, level);
    } else if (level < 0) {
      // do tyłu
      digitalWrite(LEFT_IN1, true);
      digitalWrite(LEFT_IN2, false);
      analogWrite(LEFT_PWM, -level);
    } else {
      // stop (soft)
      digitalWrite(LEFT_IN1, true);
      digitalWrite(LEFT_IN2, true);
      analogWrite(LEFT_PWM, 0);
    }
  }
  
  if (side == EngineSelector::Right) {
    if (level > 0) {
      // do przodu
      digitalWrite(RIGHT_IN1, true);
      digitalWrite(RIGHT_IN2, false);
      analogWrite(RIGHT_PWM, level);
    } else if (level < 0) {
      // do tyłu
      digitalWrite(RIGHT_IN1, false);
      digitalWrite(RIGHT_IN2, true);
      analogWrite(RIGHT_PWM, -level);
    } else {
      // stop (soft)
      digitalWrite(RIGHT_IN1, true);
      digitalWrite(RIGHT_IN2, true);
      analogWrite(RIGHT_PWM, 0);
    }
  } 
}

void Engine::turn(int left_axis_power, int right_axis_power) { //funkcja do latwiejszego skrecania autkiem
    setPowerLevel(EngineSelector::Left, left_axis_power);
    setPowerLevel(EngineSelector::Right, right_axis_power);
}

void Engine::straight(int both_axis_power) { //funkcja do nadawania mocy obu osiom autka na raz
    setPowerLevel(EngineSelector::Left, both_axis_power);
    setPowerLevel(EngineSelector::Right, both_axis_power);
}

Engine* Engine::getInstance() {
  static Engine instance;
  return &instance;
}