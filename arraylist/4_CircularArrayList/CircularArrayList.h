/*
 * CircularArrayList.h
 *
 * PROBLEM A4 -- A circular ArrayList (a.k.a. ring buffer / array deque).
 *
 * In a normal ArrayList, pushAtFront and popFromFront are O(n) because every
 * element must shift. A circular ArrayList avoids shifting: the list may
 * START anywhere in the array (at index `front`) and WRAP AROUND past the
 * end back to index 0. That makes all four push/pop operations O(1)
 * (amortized, because of expand).
 *
 *       index:   0    1    2    3    4    5
 *       data:  [ 30 | 40 |  ? |  ? | 10 | 20 ]      cap = 6, numItems = 4
 *                                   ^front = 4
 *       list (logical order) = [10, 20, 30, 40]
 *
 *   The element at logical index i lives at  data[(front + i) % cap].
 *
 * Implement everything in CircularArrayList.cpp. Do not change the public
 * interface or the data members; you MAY add private helpers.
 * Do NOT use std::vector / std::deque or any other STL container.
 *
 * ---------------------------------------------------------------------------
 * REQUIRED BEHAVIOR -- the tests replay your operations against a model, so
 * follow these rules exactly:
 * ---------------------------------------------------------------------------
 *   - New list: data == nullptr, numItems == 0, cap == 0, front == 0.
 *   - Before inserting into a full array (numItems == cap), call expand():
 *       new capacity = 2 * cap + 2; copy the elements IN LOGICAL ORDER into
 *       new[0], new[1], ...; free the old array; set front = 0.
 *   - pushAtFront: (expand if needed) then move front one slot LEFT,
 *       wrapping from 0 to cap - 1, and store the value there.
 *   - pushAtBack:  (expand if needed) then store at (front + numItems) % cap.
 *   - popFromFront: move front one slot RIGHT, wrapping from cap - 1 to 0.
 *   - popFromBack:  just shrink numItems. front does not move.
 *   - Capacity never shrinks.
 *
 *   Errors:
 *     elementAt / replaceAt with index outside [0, size)  -> std::range_error
 *     first / last / popFromFront / popFromBack on empty  -> std::runtime_error
 *
 *   toString() returns the list in LOGICAL order, e.g. "[10, 20, 30, 40]".
 *
 * Watch out: in C++, (-1) % 6 == -1, not 5! How do you step "left" safely?
 */

#ifndef CIRCULAR_ARRAY_LIST_H
#define CIRCULAR_ARRAY_LIST_H

#include <string>

class CircularArrayList {
public:
    CircularArrayList();
    ~CircularArrayList();

    /* Copying is disabled to keep this problem focused on the circular logic. */
    CircularArrayList(const CircularArrayList &other) = delete;
    CircularArrayList &operator=(const CircularArrayList &other) = delete;

    int  size() const;
    bool isEmpty() const;
    int  capacity() const;
    int  frontIndex() const; /* the physical index stored in `front` */

    int  first() const;
    int  last() const;
    int  elementAt(int index) const;
    void replaceAt(int value, int index);

    void pushAtFront(int value);
    void pushAtBack(int value);
    void popFromFront();
    void popFromBack();

    std::string toString() const;

private:
    int *data;
    int  numItems;
    int  cap;
    int  front;

    void expand();
};

#endif
