/*
 * IntArrayList.h
 *
 * PROBLEM A1 -- Implement an ArrayList (dynamic array) of ints.
 *
 * Implement every function in IntArrayList.cpp. Do NOT change the public
 * interface or the three data members below; you MAY add private helpers.
 * Do NOT use std::vector or any other STL container.
 *
 * ---------------------------------------------------------------------------
 * REQUIRED BEHAVIOR (the tests check all of this)
 * ---------------------------------------------------------------------------
 *  Representation
 *    - data points to a heap array of `cap` ints; the list's elements live in
 *      data[0 .. numItems-1].
 *    - A brand-new empty list has data == nullptr, numItems == 0, cap == 0.
 *
 *  Growth
 *    - Whenever an insertion happens and the array is full (numItems == cap),
 *      call expand() first. expand() grows the capacity to  2 * cap + 2
 *      (so 0 -> 2 -> 6 -> 14 -> 30 -> ...), copies the elements over, and
 *      frees the old array. Capacity never shrinks except in shrinkToFit().
 *
 *  Constructors / Big Three
 *    - IntArrayList(arr, length): copy of the first `length` ints of arr;
 *      size == capacity == length (length 0 behaves like the default ctor).
 *    - Copy constructor and operator= make DEEP copies. operator= must be
 *      safe for self-assignment (a = a) and must not leak the old array.
 *    - The destructor frees all heap memory.
 *
 *  Errors
 *    - elementAt / replaceAt / removeAt with index outside [0, size)
 *          -> throw std::range_error
 *    - insertAt with index outside [0, size]   (size itself is allowed)
 *          -> throw std::range_error
 *    - first / last / popFromFront / popFromBack on an empty list
 *          -> throw std::runtime_error
 *    - When a function throws, the list must be left unchanged.
 *
 *  Other
 *    - indexOf(value) returns the index of the FIRST occurrence, or -1.
 *    - clear() makes the list empty but keeps the current capacity.
 *    - shrinkToFit() reallocates so that capacity == size (if size is 0,
 *      free the array, set data to nullptr and capacity to 0).
 *    - toString() returns e.g. "[1, 2, 3]"; an empty list is "[]".
 *
 *  Big-O targets (good exam questions -- know WHY):
 *    pushAtBack amortized O(1), pushAtFront O(n), insertAt O(n),
 *    elementAt O(1), removeAt O(n), popFromBack O(1).
 */

#ifndef INT_ARRAY_LIST_H
#define INT_ARRAY_LIST_H

#include <string>

class IntArrayList {
public:
    IntArrayList();
    IntArrayList(const int *arr, int length);
    IntArrayList(const IntArrayList &other);
    IntArrayList &operator=(const IntArrayList &other);
    ~IntArrayList();

    int  size() const;
    bool isEmpty() const;
    int  capacity() const;

    int  first() const;
    int  last() const;
    int  elementAt(int index) const;
    void replaceAt(int value, int index);
    int  indexOf(int value) const;

    void pushAtBack(int value);
    void pushAtFront(int value);
    void insertAt(int value, int index);

    void popFromFront();
    void popFromBack();
    void removeAt(int index);
    void clear();

    void        shrinkToFit();
    std::string toString() const;

private:
    int *data;
    int  numItems;
    int  cap;

    void expand();
};

#endif
