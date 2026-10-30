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

    // setting data to nullptr is required when length <= 0, 
    // since no allocation will initialize data. we can just
    // do it every time since it will get overwritten when/if
    // allocation happens
    data = nullptr;
    cap = length;
    numItems = length;

    // no allocation needed if length is <1
    if (length > 0) {
        data = new int[length];
        for (int i = 0; i < length; i++) {
            data[i] = arr[i];
        }
    }
}

IntArrayList::IntArrayList(const IntArrayList &other) {
    // we want to deep copy the array list
    // so the cap and the numItems should be the same

    data = nullptr;
    cap = other.cap;
    numItems = other.numItems;

    // if the other list is empty, then we dont need to allocate
    if (cap > 0) {
        data = new int[cap];
        for (int i = 0; i < numItems; i++) {
            data[i] = other.data[i];
        }
    }
}

IntArrayList &IntArrayList::operator=(const IntArrayList &other) {
    // self assignment check, checking if "this" being the address of the
    // current object is the same as the other's address, hence &other
    if (this == &other) {
        return *this;
    }

    // int *newList declares a pointer; int newList[N] declares an array.
    // new int[N] allocates an array and returns a pointer to its first element.
    int *newList = nullptr;

    // no need to do unneccessary work if incoming is empty
    if (other.cap > 0) {
        newList = new int[other.cap];
        for (int i = 0; i < other.numItems; i++) {
            newList[i] = other.data[i];
        }
    }

    // dealloc/free current data array and set relevant parameters
    delete [] data;
    data = newList;
    numItems = other.numItems;
    cap = other.cap;
    return *this;
}

IntArrayList::~IntArrayList() {
    // classic destructor 
    delete [] data;
}

bool IntArrayList::isEmpty() const {
    // you don't need to check every element in the array
    return size() == 0;
}

int IntArrayList::first() const {
    if (isEmpty()) {
        throw std::runtime_error("empty list");
    }
    return data[0];
}

int IntArrayList::last() const {
    if (isEmpty()) {
        throw std::runtime_error("empty list");
    }

    // could also do data[numItems - 1]
    return data[size() - 1];
}

int IntArrayList::elementAt(int index) const {
    if (index < 0 || index >= size()) {
        throw std::range_error("out of range");
    }
    return data[index];
}

void IntArrayList::replaceAt(int value, int index) {
    // upper bound is included in this because size is 
    // 1 indexed meanwhile array locations are 0 indexed
    if (index < 0 || index >= size()) {
        throw std::range_error("out of range");
    }
    data[index] = value;
}

int IntArrayList::indexOf(int value) const {
    // we just want to find the first element in the array of 
    // the value
    for (int i = 0; i < numItems; i++) {
        if (data[i] == value) {
            return i;
        }
    }
    return -1;
}

void IntArrayList::pushAtBack(int value) {
    // if we hit the limit, we should increase the allocated size of
    // the array
    if (numItems == cap) {
        expand();
    }

    // doing numItems++ here is the same as doing:
    // data[numItems] = value;
    // numItems += 1; OR numItems++; OR numItems = numItems + 1;
    data[numItems++] = value;
}

void IntArrayList::pushAtFront(int value) {
    // we should just reuse this logic
    insertAt(value, 0);
}

void IntArrayList::insertAt(int value, int index) {
    // upper bound inclusive on this one
    if (index < 0 || index > size()) {
        throw std::range_error("out of range");
    }

    if (numItems == cap) {
        expand();
    }

    // start at the back of the array and shift everything
    // one to the right
    for (int i = numItems; i > index; i--) {
        data[i] = data[i - 1];
    }
    data[index] = value;
    numItems++;
}

void IntArrayList::popFromFront() {
    if (isEmpty()) {
        throw std::runtime_error("empty");
    }
    // reuse the logic
    removeAt(0);
}

void IntArrayList::popFromBack() {
    if (isEmpty()) {
        throw std::runtime_error("empty");
    }
    // you dont need to actually set the value of the last element
    // to anything. based on our implementation of the array list,
    // you will never be able to access this item, and size of the 
    // array on heap is still "cap", so this will be freed at some 
    // point
    numItems--;
}

void IntArrayList::removeAt(int index) {
    if (index < 0 || index >= size()) {
        throw std::range_error("out of bounds");
    }
    
    // start at the requested index and left shift the rest of the 
    // array
    for (int i = index; i < numItems - 1; i++) {
        data[i] = data[i + 1];
    }
    numItems--;
}

void IntArrayList::clear() {
    // clear is not the same as delete
    numItems = 0;
}

void IntArrayList::shrinkToFit() {
    // 1. create a new list
    // 2. allocate a new list to size
    // 3. copy everything into the new list
    // 4. dealloc the old list (data)
    // 5. set data pointer to the new list

    int *newList = nullptr;
    if (size() > 0) {
        newList = new int[size()];
        for (int i = 0; i < size(); i++) {
            newList[i] = data[i];
        }
    }

    delete [] data;
    data = newList;
    cap = size();
}

void IntArrayList::expand() {
    // new capacity = 2 * cap + 2. Allocate, copy, delete [] the old array.

    // 1. calculate the new cap and initialize a new list to that size
    // 2. copy elements from data into the new list
    // 3. dealloc the old list (data)
    // 4. set data pointer to the new list

    cap = 2 * cap + 2;
    int *newList = new int[cap];

    for (int i = 0; i < numItems; i++) {
        newList[i] = data[i];
    }

    delete [] data;
    data = newList;
}
