// =============================================================================
// LinkedStack.h
// Linked-list-based stack (template, header-only).
// Each element is a heap-allocated Node; head_ points to the TOP node, so
// push and pop only touch the head of the list -> O(1).
//
//   head_ -> [ top ] -> [ ... ] -> [ bottom ] -> nullptr
//
// maxSize_ == 0 means unlimited (limited only by available memory).
// A positive maxSize_ lets us demonstrate overflow on a linked stack too.
//
// Author: Aksh Rastogi (02518015625)
// =============================================================================
#ifndef LINKED_STACK_H
#define LINKED_STACK_H

#include <iostream>
#include <new>
#include <sstream>
#include <string>
#include "StackADT.h"

template <typename T>
class LinkedStack : public StackADT<T> {
public:
    explicit LinkedStack(int maxSize = 0)
        : head_(nullptr), count_(0), maxSize_(maxSize < 0 ? 0 : maxSize) {}

    // Free every node, one by one.
    ~LinkedStack() {
        while (head_ != nullptr) {
            Node* old = head_;
            head_ = head_->next;
            delete old;
        }
    }

    // Copying is disabled: a shallow copy would share (and double-delete) the nodes.
    LinkedStack(const LinkedStack&) = delete;
    LinkedStack& operator=(const LinkedStack&) = delete;

    // Create a new node and link it in front of the current head.
    void push(const T& value) override {
        if (isFull()) {
            throw StackOverflowError("Stack overflow: linked stack reached its limit of "
                                     + std::to_string(maxSize_) + " elements");
        }
        Node* node = nullptr;
        try {
            node = new Node{value, head_};
        } catch (const std::bad_alloc&) {
            throw StackOverflowError("Stack overflow: out of memory");
        }
        head_ = node;
        ++count_;
    }

    // Unlink the head node, free it, and return its value.
    T pop() override {
        if (isEmpty()) {
            throw StackUnderflowError("Stack underflow: cannot pop from an empty stack");
        }
        Node* old = head_;
        T value = old->data;
        head_ = old->next;
        delete old;
        --count_;
        return value;
    }

    T peek() const override {
        if (isEmpty()) {
            throw StackUnderflowError("Stack underflow: cannot peek into an empty stack");
        }
        return head_->data;
    }

    bool isEmpty() const override { return head_ == nullptr; }
    bool isFull() const override { return maxSize_ > 0 && count_ >= maxSize_; }
    int size() const override { return count_; }

    // Print bottom -> top, e.g.  Bottom -> [ 1 2 3 ] <- Top
    void display() const override {
        if (isEmpty()) {
            std::cout << "Stack is empty.\n";
            return;
        }
        std::cout << "Bottom -> [ " << toString() << " ] <- Top\n";
    }

    // Space-separated contents, bottom -> top.
    // The list runs top -> bottom, so each element is PREPENDED to the result.
    std::string toString() const override {
        std::string result;
        for (Node* cur = head_; cur != nullptr; cur = cur->next) {
            std::ostringstream item;
            item << cur->data;
            result = (cur == head_) ? item.str() : item.str() + " " + result;
        }
        return result;
    }

private:
    struct Node {
        T     data;
        Node* next;
    };

    Node* head_;     // top of the stack
    int   count_;    // number of nodes
    int   maxSize_;  // 0 = unlimited
};

#endif // LINKED_STACK_H
