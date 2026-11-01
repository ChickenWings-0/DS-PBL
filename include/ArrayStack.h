// =============================================================================
// ArrayStack.h
// Array-based stack (template, header-only).
// Elements live in a dynamically allocated array of fixed capacity.
// top_ is the index of the top element; top_ == -1 means the stack is empty.
//
//   index:   0     1     2    ...   top_         capacity_-1
//          [ a ] [ b ] [ c ]  ...  [ top ] [   ] [   ]
//
// All operations are O(1) except display()/toString(), which are O(n).
//
// Author: Aksh Rastogi (02518015625)
// =============================================================================
#ifndef ARRAY_STACK_H
#define ARRAY_STACK_H

#include <iostream>
#include <sstream>
#include <string>
#include "StackADT.h"

template <typename T>
class ArrayStack : public StackADT<T> {
public:
    // Allocates the array once. A capacity below 1 is raised to 1.
    explicit ArrayStack(int capacity = 100)
        : data_(nullptr), capacity_(capacity < 1 ? 1 : capacity), top_(-1) {
        data_ = new T[capacity_];
    }

    // Frees the array.
    ~ArrayStack() {
        delete[] data_;
    }

    // Copying is disabled: two objects must never share (and double-delete) one array.
    ArrayStack(const ArrayStack&) = delete;
    ArrayStack& operator=(const ArrayStack&) = delete;

    // Insert on top. Overflow if the array is already full.
    void push(const T& value) override {
        if (isFull()) {
            throw StackOverflowError("Stack overflow: array stack is full (capacity "
                                     + std::to_string(capacity_) + ")");
        }
        data_[++top_] = value;
    }

    // Remove and return the top element. Underflow if empty.
    T pop() override {
        if (isEmpty()) {
            throw StackUnderflowError("Stack underflow: cannot pop from an empty stack");
        }
        return data_[top_--];
    }

    // Return the top element without removing it. Underflow if empty.
    T peek() const override {
        if (isEmpty()) {
            throw StackUnderflowError("Stack underflow: cannot peek into an empty stack");
        }
        return data_[top_];
    }

    bool isEmpty() const override { return top_ == -1; }
    bool isFull() const override { return top_ == capacity_ - 1; }
    int size() const override { return top_ + 1; }
    int capacity() const { return capacity_; }

    // Print bottom -> top, e.g.  Bottom -> [ 1 2 3 ] <- Top
    void display() const override {
        if (isEmpty()) {
            std::cout << "Stack is empty.\n";
            return;
        }
        std::cout << "Bottom -> [ " << toString() << " ] <- Top\n";
    }

    // Space-separated contents, bottom -> top (used by the trace table).
    std::string toString() const override {
        std::ostringstream out;
        for (int i = 0; i <= top_; ++i) {
            if (i > 0) out << ' ';
            out << data_[i];
        }
        return out.str();
    }

private:
    T*  data_;      // the array (new T[capacity_])
    int capacity_;  // maximum number of elements
    int top_;       // index of the top element, -1 when empty
};

#endif // ARRAY_STACK_H
