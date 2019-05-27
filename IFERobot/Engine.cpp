#include "Engine.h"
#include "Utils.h"
#include "Variables.h"

Engine::Engine()
  : compass(Compass::getInstance()),
    serial(SerialPort::getInstance()),
    sound_sensor(SoundSensor::getInstance()),
    front_obstruction_filter(0),
    side_obstruction_filter(0) {
  pinMode(LEFT_PWM, OUTPUT);
  pinMode(LEFT_IN1, OUTPUT);
  pinMode(LEFT_IN2, OUTPUT);
  
  pinMode(RIGHT_PWM, OUTPUT);
  pinMode(RIGHT_IN1, OUTPUT);
  pinMode(RIGHT_IN2, OUTPUT);
}

void Engine::correctTargetAngle(double& angle) {
  double tmp = 0;
  for(int i = 0; i < Direction::ALL; i++) {
    tmp = angleDifference(angle, compass->getRightAngle(i));
    if(fabs(tmp) <= 45) {
      angle = compass->getRightAngle(i);
      return;
    }
  }
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

void Engine::engineTurn(int left_axis_power, int right_axis_power) { //funkcja do latwiejszego skrecania autkiem
    setPowerLevel(EngineSelector::Left, left_axis_power);
    setPowerLevel(EngineSelector::Right, right_axis_power);
}

void Engine::engineGoStraight(int both_axis_power) { //funkcja do nadawania mocy obu osiom autka na raz
    setPowerLevel(EngineSelector::Left, both_axis_power);
    setPowerLevel(EngineSelector::Right, both_axis_power);
}

void Engine::turn(int dir) {
    engineTurn(TURNING_POWER * dir, -TURNING_POWER * dir);

    auto current_angle = compass->getCurrentAngle();
    
    if(fabs(angleDifference(current_angle, target_angle)) < MAX_ANGLE_DIFFERENCE){

      engineGoStraight(0);
      driving_mode = 0;

      serial->sendMsg("\n actual: %f", current_angle);

      serial->sendMsg("     expected: %f", target_angle);
      serial->sendMsg("\n Turning protocol concluded, resuming journey");
  
      delay(1000);
    }
}

void Engine::driveStraight() {

    auto dist = sound_sensor->getFrontDistance();
    
    engineGoStraight(FORWARD_POWER);
    
    if (dist < OBSTACLE_DISTANCE) {
      //filtr
      front_obstruction_filter++;
    }
    else {
      front_obstruction_filter = 0;
    }

    if (front_obstruction_filter > 3) {
      engineGoStraight(0);
      
      serial->sendMsg("\n I see an obstacle before me");
      delay(1000); 
      
      auto current_angle = compass->getCurrentAngle();

      if(sound_sensor->getRightDistance() > sound_sensor->getLeftDistance()){
        target_angle = current_angle + 90;
        target_angle -= target_angle > 180 ? 360 : 0;
        driving_mode = 1;
      }
      else {
        target_angle = current_angle - 90;
        target_angle += target_angle < -180 ? 360 : 0;
        driving_mode = -1;
      }
      
      serial->sendMsg("\n trg before: %f", target_angle);
      correctTargetAngle(target_angle);
      serial->sendMsg("     trg after: %f", target_angle);
      
      front_obstruction_filter = 0;
    }
}

Engine* Engine::getInstance() {
  static Engine instance;
  return &instance;
}