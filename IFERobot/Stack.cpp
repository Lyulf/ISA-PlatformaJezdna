#include "Stack.h"
#include "SerialPort.h"

Stack::Stack()
    : size(0), data { 0 } { }

void Stack::push(int value) {
    if(full()) {
        auto serial = SerialPort::getInstance();
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

bool Stack::empty() const {
    return size == 0;
}

bool Stack::full() const {
    return size >= 256;
}

int Stack::getSize() {
    return size;
}