#include "SerialPort.h"

SerialPort::SerialPort()
  : serial_buffer("") {
    Serial1.begin(9600); // HC06
  }

SerialPort* SerialPort::getInstance() {
  static SerialPort instance;
  return &instance;
}

void SerialPort::readFromBluetooth() {
  //jeśli istnieje w serialu znak to wchodzi w loop
    //czyta pojedynczy znak w formie inta
    int bt_read_int;
    bt_read_int = Serial1.read();
    serial_buffer += (char)bt_read_int;
    serial_buffer.toLowerCase();
    serial_buffer.trim();
}

void SerialPort::requestChar(char c) {
  while(1) {
    if(Serial1.available()){
      if (Serial1.read() == c) {
        break;
      }
    }
   }
}

String SerialPort::getSerialBuffer() {
  return serial_buffer;
}