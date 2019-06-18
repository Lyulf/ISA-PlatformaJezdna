#ifndef ISA_QUEUE_H
#define ISA_QUEUE_H

#include <array>
#include "PushPopContainer.h"

template<typename T, std::size_t N>
class Queue : public PushPopContainer<T> {
    const std::size_t max_size;
public:
    Queue()
        : max_size(N + 1), data(), head(0), tail(0) { }

    T operator[](std::size_t pos) {
        if(pos >= size()) {
            return T();
        }
        pos += head;
        pos %= max_size;
        return data[pos];
    }

    void push(T value) {
        if(!full()) {
            data[tail++] = value;
            tail %= max_size;
        }
    }

    void pop() {
        if(!empty()) {
            head = ++head % max_size;
        }

    }

    T peek() const {
        if(!empty()) {
            return data[head];
        } else {
            return T();
        }
    }

    T avg() const {
        auto n = size();
        if(n == 0) {
            return T();
        }
        T sum = T();
        for(std::size_t i = head; i != tail; ++i % max_size) {
            sum += data[i];
        }
        return sum / n;
    }
    
    bool empty() const {
        return head == tail;
    }

    bool full() const {
        return head == (tail + 1) % max_size;
    }

    std::size_t size() const {
        std::size_t size = tail - head;
        if(head > tail) {
            size += max_size;
        }
        return size;
    }

private:
    std::array<T, N + 1> data;
    std::size_t head, tail;
};

#endif