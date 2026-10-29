/*
 * NumList.cpp -- provided implementation. You do not need to edit this.
 */

#include "NumList.h"

#include <sstream>
#include <stdexcept>

NumList::NumList() : data(nullptr), numItems(0), cap(0) {}

NumList::NumList(std::initializer_list<int> values)
    : data(nullptr), numItems(0), cap(0) {
    for (int v : values) pushAtBack(v);
}

NumList::NumList(const std::vector<int> &values)
    : data(nullptr), numItems(0), cap(0) {
    for (int v : values) pushAtBack(v);
}

NumList::NumList(const NumList &other)
    : data(nullptr), numItems(other.numItems), cap(other.cap) {
    if (cap > 0) {
        data = new int[cap];
        for (int i = 0; i < numItems; i++) data[i] = other.data[i];
    }
}

NumList &NumList::operator=(const NumList &other) {
    if (this == &other) return *this;
    int *newData = nullptr;
    if (other.cap > 0) {
        newData = new int[other.cap];
        for (int i = 0; i < other.numItems; i++) newData[i] = other.data[i];
    }
    delete [] data;
    data     = newData;
    numItems = other.numItems;
    cap      = other.cap;
    return *this;
}

NumList::~NumList() {
    delete [] data;
}

int NumList::size() const {
    return numItems;
}

int NumList::capacity() const {
    return cap;
}

int NumList::elementAt(int index) const {
    if (index < 0 || index >= numItems) {
        throw std::range_error("index (" + std::to_string(index) +
                               ") not in range [0.." +
                               std::to_string(numItems) + ")");
    }
    return data[index];
}

void NumList::pushAtBack(int value) {
    if (numItems == cap) expand();
    data[numItems++] = value;
}

std::string NumList::toString() const {
    std::stringstream ss;
    ss << "[";
    for (int i = 0; i < numItems; i++) {
        if (i > 0) ss << ", ";
        ss << data[i];
    }
    ss << "]";
    return ss.str();
}

std::vector<int> NumList::toVector() const {
    return std::vector<int>(data, data + numItems);
}

void NumList::expand() {
    int  newCap  = 2 * cap + 2;
    int *newData = new int[newCap];
    for (int i = 0; i < numItems; i++) newData[i] = data[i];
    delete [] data;
    data = newData;
    cap  = newCap;
}
