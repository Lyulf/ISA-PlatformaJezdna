#include "PathAI.h"

PathAI::PathAI()
  : compass(Compass::getInstance()), serial(SerialPort::getInstance()) { }

void PathAI::handleBluetoothSerial(String bt_command) {
  //bt_command to będzie komenda zczytana z bluetootha i na jej podstawie okreslamy nasz nowy kierunek jazdy
  //double driving_tendency_angle
  //double target_angle;

  //zmieniamy nasza komende na int'a, przyda nam sie to, jesli inputem był rzeczywiscie numer
  //right_angles[i] -= right_angles[i] > 180 ? 360 : 0;
//  int command_int = bt_command.toInt(); 

  if(bt_command == String("north")) {
    Serial1.flush();
    driving_tendency_angle = compass->getRightAngle(Direction::NORTH);
  }
  else if(bt_command == String("east")) {
    Serial1.flush();
    driving_tendency_angle = compass->getRightAngle(Direction::EAST);
  }
  else if(bt_command == String("south")) {
    Serial1.flush();
    driving_tendency_angle = compass->getRightAngle(Direction::SOUTH);
  }
  else if(bt_command == String("west")) {
    Serial1.flush();
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

PathAI* PathAI::getInstance() {
  static PathAI instance;
  return &instance;
}