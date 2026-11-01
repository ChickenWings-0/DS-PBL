// =============================================================================
// StackADT.h
// Abstract Stack ADT interface + exception types for overflow / underflow.
// Both ArrayStack<T> and LinkedStack<T> implement this interface, so the menu
// can use either one through a StackADT<T>* pointer (runtime polymorphism).
//
// Author: Aksh Rastogi (02518015625)
// =============================================================================
#ifndef STACK_ADT_H
#define STACK_ADT_H

#include <stdexcept>
#include <string>

// Thrown when push() is called on a full stack.
class StackOverflowError : public std::runtime_error {
public:
    explicit StackOverflowError(const std::string& msg) : std::runtime_error(msg) {}
};

// Thrown when pop() or peek() is called on an empty stack.
class StackUnderflowError : public std::runtime_error {
public:
    explicit StackUnderflowError(const std::string& msg) : std::runtime_error(msg) {}
};

// Abstract interface: every stack operation is a pure virtual function.
template <typename T>
class StackADT {
public:
    virtual ~StackADT() {}
    virtual void push(const T& value) = 0;      // throws StackOverflowError if full
    virtual T pop() = 0;                        // throws StackUnderflowError if empty
    virtual T peek() const = 0;                 // throws StackUnderflowError if empty
    virtual bool isEmpty() const = 0;
    virtual bool isFull() const = 0;
    virtual int size() const = 0;
    virtual void display() const = 0;           // prints contents bottom -> top to std::cout
    virtual std::string toString() const = 0;   // "a b c" bottom -> top ("" if empty)
};

#endif // STACK_ADT_H
