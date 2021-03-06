#include "PathAI.h"
#include "Utils.h"
#include "Variables.h"
#include <stdio.h>

using DM = DrivingMode;

PathAI::PathAI()
  : compass(Compass::getInstance()),
    serial(SerialPort::getInstance()),
    sound_sensor(SoundSensor::getInstance()),
    directions(),
    driving_mode(DM::DrivingStraight),
    wait_ms(0) { }

void PathAI::handleBluetoothSerial(String bt_command) {
  //bt_command to będzie komenda zczytana z bluetootha i na jej podstawie okreslamy nasz nowy kierunek jazdy
  //double driving_tendency_angle
  //double target_angle;

  //zmieniamy nasza komende na int'a, przyda nam sie to, jesli inputem był rzeczywiscie numer
  //right_angles[i] -= right_angles[i] > 180 ? 360 : 0;
//  int command_int = bt_command.toInt(); 
  // auto serial_buffer = serial->getBuffer();
  bt_command.toLowerCase();
  if(bt_command == String("north")) {
    // Serial1.flush();
    // serial->clearBuffer();
    serial->sendMsg("Going NORTH\n");
    driving_tendency_angle = compass->getRightAngle(Direction::NORTH);
  }
  else if(bt_command == String("east")) {
    // Serial1.flush();
    // serial->clearBuffer();
    serial->sendMsg("Going EAST\n");
    driving_tendency_angle = compass->getRightAngle(Direction::EAST);
  }
  else if(bt_command == String("south")) {
    // Serial1.flush();
    // serial->clearBuffer();
    serial->sendMsg("Going SOUTH\n");
    driving_tendency_angle = compass->getRightAngle(Direction::SOUTH);
  }
  else if(bt_command == String("west")) {
    // Serial1.flush();
    // serial->clearBuffer();
    serial->sendMsg("Going WEST\n");
    driving_tendency_angle = compass->getRightAngle(Direction::WEST);
  }
  /*
  //teraz sprawdzamy jeśli komenda z bluetootha nie była z zadnym kierunków geograficznych tylko numerem
  else if(command_int > 0 && command_int <= 90) {
    //driving_tendency_angle = kat z ring_angles[i]; wiekszy o 1 index
  }
  else if(command_int > 90 && command_int <= 180) {
    //driving_tendency_angle = kat z ring_angles[i]; wiekszy o 2 indexy
  }
  else if(command_int >= -180 && command_int <= 90) {
    //driving_tendency_angle = kat z ring_angles[i]; mniejszy o 2 indexy
  }
  else if(command_int >= 0 && command_int < 90) {
    //driving_tendency_angle = kat z ring_angles[i]; mniejszy o 1 indexy
  }*/
}

void PathAI::drive() {
  static auto last_time = millis();
  auto current_time = millis();
  auto time_diff = std::max(current_time, last_time) - std::min(current_time, last_time);
  last_time = current_time;

  if(time_diff < wait_ms) {
    wait_ms -= time_diff;
  } else {
    wait_ms = 0;
  }

  if(wait_ms) {
    return;
  }

  int dist;
  if (driving_mode == DM::DrivingStraight) {
      // if(directions.getSize()) {
      //   if(directions.peek() == RIGHT_DIR) {
      //     dist = sound_sensor->getLeftDistance();
      //   } else {
      //     dist = sound_sensor->getRightDistance();
      //   }
      //   if (dist > OBSTACLE_DISTANCE_SIDE) {
        //   //filtr
        //   side_obstruction_filter++;
        // }
        // else {
        //   side_obstruction_filter = 0;
        // }
        // if (side_obstruction_filter > 3) {
          // serial->sendMsg("\nside_dist=%f", dist);
          // stop();
          // if(directions.peek() == RIGHT_DIR) {
          //   serial->sendMsg("\n I see a path to my left");
          // } else {
          //   serial->sendMsg("\n I see a path to my right");
          // }
          // delay(1000); 

    
          // side_obstruction_filter = 0;
          // return;
        // }
      // }
      dist = sound_sensor->getFrontDistance();
      if(dist < OBSTACLE_DISTANCE_FRONT) {
        stop();
        serial->sendMsg("\nfront_dist=%f", dist);
        serial->sendMsg("\n I see an obstacle before me");
        driving_mode = DrivingMode::LookingForPath;
        wait_ms = 1000;
      } else {
        driveStraight();
      }
  } else if(driving_mode == DrivingMode::LookingForPath) {
    auto current_angle = compass->getCurrentAngle();
    if(!directions.empty()) {
      auto dir = directions.peek();
        if(dir == RIGHT_DIR) {
          dist = sound_sensor->getLeftDistance();
        } else {
          dist = sound_sensor->getRightDistance();
        }

        if(dist > OBSTACLE_DISTANCE_SIDE) {
          serial->sendMsg("\ndist = %d", dist);
          if(dir == LEFT_DIR) {
            serial->sendMsg("\nturning right");
            target_angle = current_angle + 90;
            target_angle -= target_angle > 180 ? 360 : 0;
            driving_mode = DM::TurningRight;
          }
          else {
            serial->sendMsg("\nturning left");
            target_angle = current_angle - 90;
            target_angle += target_angle < -180 ? 360 : 0;
            // target_angle = fmod(current_angle - 90, 180);
            driving_mode = DM::TurningLeft;
          }
          directions.pop();
          serial->sendMsg("\n trg before: %f", target_angle);
          correctTargetAngle(target_angle);
          serial->sendMsg("     trg after: %f", target_angle);
          return;
        }
    }

    // dist = sound_sensor->getFrontDistance();
    // if (dist < OBSTACLE_DISTANCE_FRONT) {
      //   //filtr
      //   front_obstruction_filter++;
    // }
  // else {
  //   front_obstruction_filter = 0;
  // }

  // if (front_obstruction_filter > 3) {
    // delay(1000); 

    auto left_dist = sound_sensor->getLeftDistance();
    auto right_dist = sound_sensor->getRightDistance();
    serial->sendMsg("\nleft = %d right = %d", left_dist, right_dist);
    if(right_dist > left_dist) {
      serial->sendMsg("\nturning right");
      target_angle = current_angle + 90;
      target_angle -= target_angle > 180 ? 360 : 0;
      directions.push(RIGHT_DIR);
      driving_mode = DM::TurningRight;
    }
    else {
      serial->sendMsg("\nturning left");
      // target_angle = fmod(current_angle - 90, 180);
      target_angle = current_angle - 90;
      target_angle += target_angle < -180 ? 360 : 0;
      directions.push(LEFT_DIR);
      driving_mode = DM::TurningLeft;
    }
    
    serial->sendMsg("\n trg before: %f", target_angle);
    correctTargetAngle(target_angle);
    serial->sendMsg("     trg after: %f", target_angle);
    
    // front_obstruction_filter = 0;
  // }

  } else if(driving_mode == DM::TurningLeft || driving_mode == DM::TurningRight) {
    auto current_angle = compass->getCurrentAngle();
    if(fabs(angleDifference(current_angle, target_angle)) < MAX_ANGLE_DIFFERENCE){
      stop();
      driving_mode = DM::DrivingStraight;

      serial->sendMsg("\n actual: %f     expected: %f", current_angle, target_angle);
      serial->sendMsg("\n Turning protocol concluded, resuming journey");
      wait_ms = 1000;
    } else {
      int dir = driving_mode == DM::TurningLeft ? LEFT_DIR : RIGHT_DIR;
      turn(dir);
    }
  } else {
    stop();
  }
}

void PathAI::turn(int dir) {
  engine->turn(TURNING_POWER * dir, -TURNING_POWER * dir);
}

void PathAI::driveStraight() {
  engine->straight(FORWARD_POWER);
}

void PathAI::stop() {
  engine->straight(0);
}

void PathAI::correctTargetAngle(double& angle) {
  double best_diff = 666; //dummy value
  int best_angle = 0;
  double tmp = 0;
  for(int i = 0; i < Direction::ALL; i++) {
    tmp = fabs(angleDifference(angle, compass->getRightAngle(i)));
    if(tmp < best_diff) {
      best_angle = i;
      best_diff = tmp;
    }
  }
  angle = compass->getRightAngle(best_angle);
}

int PathAI::getDrivingMode() {
  return driving_mode;
}

PathAI* PathAI::getInstance() {
  static PathAI instance;
  return &instance;
}
