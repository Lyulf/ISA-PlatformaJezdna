#ifndef ISA_SERIAL_PORT_H
#define ISA_SERIAL_PORT_H

#include <Arduino.h>
#include <stdio.h>
#include <utility>
#include "Variables.h"

class SerialPort {
    SerialPort();
public:
    void readFromBluetooth();
    void requestChar(const char c);
    String getBuffer();
    void clearBuffer();

    template<class ...Args>
    void sendMsg(const String& format, Args&& ...args) {
        snprintf(buffer, sizeof(buffer), format.c_str(), args...);
        Serial1.print(buffer);
    }

    template<class ...Args>
    void sendRequest(const char c, const String& format, Args&& ...args) {
        sendMsg(format, args...);
        requestChar(c);
    }
    
    template<class ...Args>
    void sendRequest(const String& format, Args&& ...args) {
        sendRequest('+', format, args...);
    }

    static SerialPort* getInstance();

private:
    void finishReading();

    String serial_buffer;
    char buffer[SERIAL_PORT_BUFFER_SIZE];
    std::size_t buffer_pos;
};

#endif
