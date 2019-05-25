#include "SerialPort.h"

char buffer[64];
String serial_buffer = String("");

void readFromBluetooth() {
  //jeśli istnieje w serialu znak to wchodzi w loop
    //czyta pojedynczy znak w formie inta
    int bt_read_int;
    bt_read_int = Serial1.read();
    serial_buffer += (char)bt_read_int;
    serial_buffer.toLowerCase();
    serial_buffer.trim();
}

void requestChar(char c) {
  while(1) {
    if(Serial1.available()){
      if (Serial1.read() == c) {
        break;
      }
    }
   }
}