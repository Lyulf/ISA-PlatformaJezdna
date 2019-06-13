#ifndef ISA_STACK_H
#define ISA_STACK_H

class Stack {
public:
    Stack();

    void push(int value);
    void pop();
    int peek();
    int getSize();

private:
    int data[256];
    int size;
};

#endif