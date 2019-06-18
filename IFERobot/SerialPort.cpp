#include "SerialPort.h"

SerialPort::SerialPort()
  : serial_buffer(""), buffer(), buffer_pos(0) {
    Serial1.begin(9600); // HC06
  }

void SerialPort::readFromBluetooth() {
  //jeśli istnieje w serialu znak to wchodzi w loop
    //czyta pojedynczy znak w formie inta
    int bt_read_int;
    bt_read_int = Serial1.read();
    char c = (char) bt_read_int;
    sendMsg("\n%c, %s, %d", c, buffer, buffer_pos);
    if(isspace(bt_read_int)) {
      if(c == '\n') {
        finishReading();
      }
      return;
    }
    buffer[buffer_pos++] = tolower(bt_read_int);
    if(buffer_pos == SERIAL_BUFFER_SIZE) {
      finishReading();
    }
}

void SerialPort::finishReading() {
      serial_buffer = buffer;
      buffer_pos = 0;
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

String SerialPort::getBuffer() {
  return serial_buffer;
}

void SerialPort::clearBuffer() {
  serial_buffer = String("");
}

SerialPort* SerialPort::getInstance() {
  static SerialPort instance;
  return &instance;
}
