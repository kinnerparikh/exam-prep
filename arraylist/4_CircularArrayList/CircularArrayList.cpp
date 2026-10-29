/*
 * CircularArrayList.cpp
 *
 * PROBLEM A4 -- your implementation goes here. Read CircularArrayList.h
 * carefully first: the exact rules for `front` and expand() are tested.
 *
 * Tip: write a private helper that converts a logical index into a physical
 * array index -- then elementAt, replaceAt, last, toString and expand all
 * become one-liners around it.
 *
 * Run the tests:  make al4
 */

#include "CircularArrayList.h"

#include <sstream>
#include <stdexcept>
#include <string>

/* ======================= Given to you ======================= */

CircularArrayList::CircularArrayList() {
    data     = nullptr;
    numItems = 0;
    cap      = 0;
    front    = 0;
}

int CircularArrayList::size() const {
    return numItems;
}

int CircularArrayList::capacity() const {
    return cap;
}

int CircularArrayList::frontIndex() const {
    return front;
}

/* ======================= Your turn ========================== */

CircularArrayList::~CircularArrayList() {
    // TODO
}

bool CircularArrayList::isEmpty() const {
    throw std::logic_error("TODO: implement isEmpty");
}

int CircularArrayList::first() const {
    throw std::logic_error("TODO: implement first");
}

int CircularArrayList::last() const {
    throw std::logic_error("TODO: implement last");
}

int CircularArrayList::elementAt(int index) const {
    throw std::logic_error("TODO: implement elementAt");
}

void CircularArrayList::replaceAt(int value, int index) {
    throw std::logic_error("TODO: implement replaceAt");
}

void CircularArrayList::pushAtFront(int value) {
    throw std::logic_error("TODO: implement pushAtFront");
}

void CircularArrayList::pushAtBack(int value) {
    throw std::logic_error("TODO: implement pushAtBack");
}

void CircularArrayList::popFromFront() {
    throw std::logic_error("TODO: implement popFromFront");
}

void CircularArrayList::popFromBack() {
    throw std::logic_error("TODO: implement popFromBack");
}

std::string CircularArrayList::toString() const {
    // Must print in LOGICAL order (starting from front), not array order.
    throw std::logic_error("TODO: implement toString");
}

void CircularArrayList::expand() {
    // The old contents may wrap around the end of the array. "Unwrap" them
    // into new[0 .. numItems-1], then set front = 0.
    throw std::logic_error("TODO: implement expand");
}
