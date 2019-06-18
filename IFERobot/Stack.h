#ifndef ISA_STACK_H
#define ISA_STACK_H

#include "PushPopContainer.h"

class Stack : public PushPopContainer<int> {
public:
    Stack();

    virtual void push(int value);
    virtual void pop();
    int peek();
    virtual bool empty() const;
    virtual bool full() const;
    int getSize();

private:
    int data[256];
    int size;
};

#endif