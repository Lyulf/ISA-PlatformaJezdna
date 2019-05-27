#include "Engine.h"
#include "Compass.h"
#include "SerialPort.h"
#include "Variables.h"
#include "SoundSensor.h"
#include "Utils.h"

int driving_mode = 0;
double current_angle;
double target_angle;
extern Compass* compass;

void initEngine() {
  pinMode(LEFT_PWM, OUTPUT);
  pinMode(LEFT_IN1, OUTPUT);
  pinMode(LEFT_IN2, OUTPUT);
  
  pinMode(RIGHT_PWM, OUTPUT);
  pinMode(RIGHT_IN1, OUTPUT);
  pinMode(RIGHT_IN2, OUTPUT);
}

void correctTargetAngle(double& angle) {
  double tmp = 0;
  for(int i = 0; i < Direction::ALL; i++) {
    tmp = angleDifference(angle, compass->getRightAngle(i));
    if(fabs(tmp) <= 45) {
      angle = compass->getRightAngle(i);
      return;
    }
  }
}

void setPowerLevel(EngineSelector side, int level)
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

void engineTurn(int left_axis_power, int right_axis_power) { //funkcja do latwiejszego skrecania autkiem
    setPowerLevel(EngineSelector::Left, left_axis_power);
    setPowerLevel(EngineSelector::Right, right_axis_power);
}

void engineGoStraight(int both_axis_power) { //funkcja do nadawania mocy obu osiom autka na raz
    setPowerLevel(EngineSelector::Left, both_axis_power);
    setPowerLevel(EngineSelector::Right, both_axis_power);
}

void turn(int dir) {
    engineTurn(TURNING_POWER * dir, -TURNING_POWER * dir);

    current_angle = compass->getCurrentAngle();
    
    if(fabs(angleDifference(current_angle, target_angle)) < MAX_ANGLE_DIFFERENCE){

      engineGoStraight(0);
      driving_mode = 0;

      sprintf(buffer, "\n actual: %f", current_angle);
      Serial1.print(buffer);

      sprintf(buffer, "     expected: %f", target_angle);
      Serial1.print(buffer);
  
      Serial1.print("\n Turning protocol concluded, resuming journey");
  
      delay(1000);
    }
}

void driveStraight() {

    dist = getFrontDistance();
    
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
      
      Serial1.print("\n I see an obstacle before me");
      delay(1000); 
      
      current_angle = compass->getCurrentAngle();

      if(getRightDistance() > getLeftDistance()){
        target_angle = current_angle + 90;
        target_angle -= target_angle > 180 ? 360 : 0;
        driving_mode = 1;
      }
      else {
        target_angle = current_angle - 90;
        target_angle += target_angle < -180 ? 360 : 0;
        driving_mode = -1;
      }
      
      sprintf(buffer, "\n trg before: %f", target_angle);
      Serial1.print(buffer);
      correctTargetAngle(target_angle);
      sprintf(buffer, "     trg after: %f", target_angle);
      Serial1.print(buffer);
      
      front_obstruction_filter = 0;
    }
}