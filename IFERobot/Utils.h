#ifndef ISA_UTILS_H
#define ISA_UTILS_H

#include "PushPopContainer.h"

double angleDifference(double current, double target);

template<typename T>
void forcePush(PushPopContainer<T>& container, T value) {
    if(container.full()) {
        container.pop();
    }
    container.push(value);
}

#endif