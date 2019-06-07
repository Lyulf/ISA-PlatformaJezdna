#include "PathAI.h"
#include "Utils.h"
#include "Variables.h"

PathAI::PathAI()
  : compass(Compass::getInstance()),
    serial(SerialPort::getInstance()),
    sound_sensor(SoundSensor::getInstance()),
    front_obstruction_filter(0),
    side_obstruction_filter(0) { }

void PathAI::handleBluetoothSerial(String bt_command) {
  //bt_command to będzie komenda zczytana z bluetootha i na jej podstawie okreslamy nasz nowy kierunek jazdy
  //double driving_tendency_angle
  //double target_angle;

  //zmieniamy nasza komende na int'a, przyda nam sie to, jesli inputem był rzeczywiscie numer
  //right_angles[i] -= right_angles[i] > 180 ? 360 : 0;
//  int command_int = bt_command.toInt(); 
  auto serial_buffer = serial->getBuffer();
  if(bt_command == String("north")) {
    Serial1.flush();
    serial->clearBuffer();
    serial->sendMsg("Going NORTH\n");
    driving_tendency_angle = compass->getRightAngle(Direction::NORTH);
  }
  else if(bt_command == String("east")) {
    Serial1.flush();
    serial->clearBuffer();
    serial->sendMsg("Going EAST\n");
    driving_tendency_angle = compass->getRightAngle(Direction::EAST);
  }
  else if(bt_command == String("south")) {
    Serial1.flush();
    serial->clearBuffer();
    serial->sendMsg("Going SOUTH\n");
    driving_tendency_angle = compass->getRightAngle(Direction::SOUTH);
  }
  else if(bt_command == String("west")) {
    Serial1.flush();
    serial->clearBuffer();
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

void PathAI::turn(int dir) {
    engine->turn(TURNING_POWER * dir, -TURNING_POWER * dir);

    auto current_angle = compass->getCurrentAngle();
    
    if(fabs(angleDifference(current_angle, target_angle)) < MAX_ANGLE_DIFFERENCE){

      engine->straight(0);
      driving_mode = 0;

      serial->sendMsg("\n actual: %f", current_angle);

      serial->sendMsg("     expected: %f", target_angle);
      serial->sendMsg("\n Turning protocol concluded, resuming journey");
  
      delay(1000);
    }
}

void PathAI::driveStraight() {

    auto dist = sound_sensor->getFrontDistance();
    
    engine->straight(FORWARD_POWER);
    
    if (dist < OBSTACLE_DISTANCE) {
      //filtr
      front_obstruction_filter++;
    }
    else {
      front_obstruction_filter = 0;
    }

    if (front_obstruction_filter > 3) {
      engine->straight(0);
      
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

void PathAI::correctTargetAngle(double& angle) {
  double tmp = 0;
  for(int i = 0; i < Direction::ALL; i++) {
    tmp = angleDifference(angle, compass->getRightAngle(i));
    if(fabs(tmp) <= 45) {
      angle = compass->getRightAngle(i);
      return;
    }
  }
}

int PathAI::getDrivingMode() {
  return driving_mode;
}

PathAI* PathAI::getInstance() {
  static PathAI instance;
  return &instance;
}
