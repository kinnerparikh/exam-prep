/*
 * IntArrayList.cpp
 *
 * PROBLEM A1 -- your implementation goes here. See IntArrayList.h for the
 * full specification.
 *
 * A few functions are done for you as examples. Every other function
 * currently throws a "TODO" logic_error; delete that line and write the
 * real implementation.
 *
 * Run the tests:   make al1            (from the exam-prep folder)
 * One test only:   ./build/al1 insertAt
 */

#include "IntArrayList.h"

#include <sstream>
#include <stdexcept>
#include <string>

/* ======================= Given to you ======================= */

IntArrayList::IntArrayList() {
    data     = nullptr;
    numItems = 0;
    cap      = 0;
}

int IntArrayList::size() const {
    return numItems;
}

int IntArrayList::capacity() const {
    return cap;
}

std::string IntArrayList::toString() const {
    std::stringstream ss;
    ss << "[";
    for (int i = 0; i < numItems; i++) {
        if (i > 0) ss << ", ";
        ss << data[i];
    }
    ss << "]";
    return ss.str();
}

/* ======================= Your turn ========================== */

IntArrayList::IntArrayList(const int *arr, int length) {
    // Hint: allocate exactly `length` slots, then copy.
    data = new int[length]; //allocates length int for the array
    //set numItems and cap to length
    numItems = length;
    cap = length;
    //for each element in current area copy to new array
    for (int i = 0; i < length; i++) {
        data[i] = arr[i];
    }
}

IntArrayList::IntArrayList(const IntArrayList &other) {
    // Hint: a deep copy needs its OWN array. Copying the pointer is not enough.
    throw std::logic_error("TODO: implement the copy constructor");
}

IntArrayList &IntArrayList::operator=(const IntArrayList &other) {
    // Hint: 1) check for self-assignment  2) free old memory  3) deep copy
    //       4) return *this
    throw std::logic_error("TODO: implement operator=");
}

IntArrayList::~IntArrayList() {
    // TODO: free the heap array (destructors must never throw)
}

bool IntArrayList::isEmpty() const {
    throw std::logic_error("TODO: implement isEmpty");
}

int IntArrayList::first() const {
    throw std::logic_error("TODO: implement first");
}

int IntArrayList::last() const {
    throw std::logic_error("TODO: implement last");
}

int IntArrayList::elementAt(int index) const {
    throw std::logic_error("TODO: implement elementAt");
}

void IntArrayList::replaceAt(int value, int index) {
    throw std::logic_error("TODO: implement replaceAt");
}

int IntArrayList::indexOf(int value) const {
    throw std::logic_error("TODO: implement indexOf");
}

void IntArrayList::pushAtBack(int value) {
    throw std::logic_error("TODO: implement pushAtBack");
}

void IntArrayList::pushAtFront(int value) {
    // Hint: which direction do you have to shift, and in what order?
    throw std::logic_error("TODO: implement pushAtFront");
}

void IntArrayList::insertAt(int value, int index) {
    throw std::logic_error("TODO: implement insertAt");
}

void IntArrayList::popFromFront() {
    throw std::logic_error("TODO: implement popFromFront");
}

void IntArrayList::popFromBack() {
    if (numItems > 0) {
        numItems--;
    }
}

void IntArrayList::removeAt(int index) {
    //access a specific numItem in the array list
    //overwrite specific numItem from array list, move elements after 1 over
    //dont change everything before the specific numItem
    for (int i = index; i < numItems - 1; i++) {
        data[i] = data[i + 1];
    }
    //the index of the everything after the specific numItem decreases by 1
    numItems--;

}

void IntArrayList::clear() {
    throw std::logic_error("TODO: implement clear");
}

void IntArrayList::shrinkToFit() {
    throw std::logic_error("TODO: implement shrinkToFit");
}

void IntArrayList::expand() {
    // new capacity = 2 * cap + 2. Allocate, copy, delete [] the old array.
    throw std::logic_error("TODO: implement expand");
}
