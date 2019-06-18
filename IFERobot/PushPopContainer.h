#ifndef ISA_PUSH_POP_CONTAINER_H
#define ISA_PUSH_POP_CONTAINER_H

template<typename T>
class PushPopContainer {
public:
    virtual void push(T value) = 0;
    virtual void pop() = 0;
    virtual bool empty() const = 0;
    virtual bool full() const = 0;
};

#endif