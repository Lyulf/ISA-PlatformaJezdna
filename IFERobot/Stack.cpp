#include "Stack.h"
#include "SerialPort.h"

Stack::Stack()
    : size(0), data { 0 } { }

void Stack::push(int value) {
    if(size >= 256) {
        auto serial = SerialPort::getInstance();
        serial->sendMsg("KURWA CO");
        return;
    }
    data[size++] = value;
}

void Stack::pop() {
    if(size > 0) {
        size--;
    }
}

int Stack::peek() {
    if(size >= 1) {
        return data[size - 1];
    } else {
        return 0;
    }
}

int Stack::getSize() {
    return size;
}