#ifndef ISA_SERIAL_PORT_H
#define ISA_SERIAL_PORT_H

#include <Arduino.h>
#include <string>

//BT variables
extern String serial_buffer;
extern char buffer[];

void readFromBluetooth();
void requestChar(char c);

#endif
