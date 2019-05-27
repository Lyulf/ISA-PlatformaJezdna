#ifndef ISA_SERIAL_PORT_H
#define ISA_SERIAL_PORT_H

#include <Arduino.h>
#include <cstdio>
#include <utility>

class SerialPort {
    SerialPort();
public:
    void readFromBluetooth();
    void requestChar(const char c);
    String getSerialBuffer();

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
    String serial_buffer;
    char buffer[64];
};

#endif
