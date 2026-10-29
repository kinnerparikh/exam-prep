/*
 * IntLinkedList.cpp
 *
 * PROBLEM L1 -- your implementation goes here. See IntLinkedList.h for the
 * full specification and invariants.
 *
 * For every insert/remove, draw the boxes and arrows first and ask:
 *   - What if the list is empty?
 *   - What if I'm changing the FRONT node? The BACK node? The ONLY node?
 *   - Did I update BOTH next and prev? Did I update front/back? numItems?
 *
 * Run the tests:  make ll1
 */

#include "IntLinkedList.h"

#include <sstream>
#include <stdexcept>
#include <string>

/* ======================= Given to you ======================= */

IntLinkedList::IntLinkedList() {
    front    = nullptr;
    back     = nullptr;
    numItems = 0;
}

int IntLinkedList::size() const {
    return numItems;
}

std::string IntLinkedList::toString() const {
    std::stringstream ss;
    ss << "[";
    for (Node *curr = front; curr != nullptr; curr = curr->next) {
        if (curr != front) ss << ", ";
        ss << curr->data;
    }
    ss << "]";
    return ss.str();
}

/* ======================= Your turn ========================== */

IntLinkedList::IntLinkedList(const int *arr, int length) {
    // Hint: start from an empty list and reuse pushAtBack.
    throw std::logic_error("TODO: implement IntLinkedList(arr, length)");
}

IntLinkedList::IntLinkedList(const IntLinkedList &other) {
    throw std::logic_error("TODO: implement the copy constructor");
}

IntLinkedList &IntLinkedList::operator=(const IntLinkedList &other) {
    throw std::logic_error("TODO: implement operator=");
}

IntLinkedList::~IntLinkedList() {
    // TODO: delete every node. (Careful: don't use a node after deleting it!)
}

bool IntLinkedList::isEmpty() const {
    throw std::logic_error("TODO: implement isEmpty");
}

int IntLinkedList::first() const {
    throw std::logic_error("TODO: implement first");
}

int IntLinkedList::last() const {
    throw std::logic_error("TODO: implement last");
}

int IntLinkedList::elementAt(int index) const {
    throw std::logic_error("TODO: implement elementAt");
}

void IntLinkedList::replaceAt(int value, int index) {
    throw std::logic_error("TODO: implement replaceAt");
}

int IntLinkedList::indexOf(int value) const {
    throw std::logic_error("TODO: implement indexOf");
}

void IntLinkedList::pushAtFront(int value) {
    throw std::logic_error("TODO: implement pushAtFront");
}

void IntLinkedList::pushAtBack(int value) {
    throw std::logic_error("TODO: implement pushAtBack");
}

void IntLinkedList::insertAt(int value, int index) {
    throw std::logic_error("TODO: implement insertAt");
}

void IntLinkedList::insertInOrder(int value) {
    throw std::logic_error("TODO: implement insertInOrder");
}

void IntLinkedList::popFromFront() {
    throw std::logic_error("TODO: implement popFromFront");
}

void IntLinkedList::popFromBack() {
    throw std::logic_error("TODO: implement popFromBack");
}

void IntLinkedList::removeAt(int index) {
    throw std::logic_error("TODO: implement removeAt");
}

void IntLinkedList::clear() {
    throw std::logic_error("TODO: implement clear");
}

std::string IntLinkedList::toReverseString() const {
    // Walk from `back` using prev pointers -- do NOT just reverse toString().
    throw std::logic_error("TODO: implement toReverseString");
}
