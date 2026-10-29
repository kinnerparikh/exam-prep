/*
 * CircularArrayList.cpp
 *
 * PROBLEM A4 -- SOLUTION. Read CircularArrayList.h for the exact rules for
 * `front` and expand().
 *
 * THE BIG IDEA
 * ------------
 * Think of the array as a ring: the slot after cap-1 is slot 0.
 * `front` is the physical slot holding the first element, and the list
 * occupies numItems consecutive slots around the ring starting there.
 *
 *     logical index i   lives at   physical slot (front + i) % cap
 *
 * Example, cap = 6, front = 4, numItems = 4:
 *
 *     slot:     0    1    2    3    4    5
 *     data:  [ 30 | 40 |  ? |  ? | 10 | 20 ]
 *                                  ^front
 *     logical 0 -> (4+0)%6 = 4 -> 10
 *     logical 1 -> (4+1)%6 = 5 -> 20
 *     logical 2 -> (4+2)%6 = 0 -> 30    <- wrapped around!
 *     logical 3 -> (4+3)%6 = 1 -> 40
 *
 * Adding/removing at either end only moves `front` or changes numItems --
 * NO shifting -- so all four push/pop operations are O(1) (amortized for
 * push, because of the occasional expand).
 *
 * THE ONE TRAP: moving `front` LEFT
 * ---------------------------------
 * In C++, % can return a negative number: (-1) % 6 == -1, not 5.
 * So  (front - 1) % cap  is WRONG when front == 0. Add cap first:
 *
 *     front = (front - 1 + cap) % cap;     front = 0 -> (0 - 1 + 6) % 6 = 5
 *
 * Since 0 <= front < cap, (front - 1 + cap) is never negative.
 *
 * Also never compute  x % cap  when cap == 0 (division by zero -> crash).
 * Every function below either expands first (push) or checks for an empty
 * list first (everything else), so cap > 0 whenever we use %.
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

/* ======================= Solution =========================== */

/*
 * The helper every other function leans on. Callers guarantee cap > 0
 * and 0 <= logicalIndex < numItems.
 */
int CircularArrayList::physicalIndex(int logicalIndex) const {
    return (front + logicalIndex) % cap;
}

/* One heap array -> one delete []. (delete [] nullptr is a safe no-op.) */
CircularArrayList::~CircularArrayList() {
    delete [] data;
}

bool CircularArrayList::isEmpty() const {
    return numItems == 0;
}

/* The first element is simply at slot `front` (logical index 0). */
int CircularArrayList::first() const {
    if (isEmpty()) {
        throw std::runtime_error("first: list is empty");
    }
    return data[front];
}

/*
 * The last element is logical index numItems - 1. It is NOT necessarily
 * data[numItems - 1] and NOT necessarily data[cap - 1] -- it may have
 * wrapped around. Let the helper do the math.
 */
int CircularArrayList::last() const {
    if (isEmpty()) {
        throw std::runtime_error("last: list is empty");
    }
    return data[physicalIndex(numItems - 1)];
}

/* Bounds are checked against the LOGICAL size, then translated. O(1). */
int CircularArrayList::elementAt(int index) const {
    if (index < 0 || index >= numItems) {
        throw std::range_error("elementAt: index out of range");
    }
    return data[physicalIndex(index)];
}

void CircularArrayList::replaceAt(int value, int index) {
    if (index < 0 || index >= numItems) {
        throw std::range_error("replaceAt: index out of range");
    }
    data[physicalIndex(index)] = value;
}

/*
 * pushAtFront: O(1) amortized -- this is the whole point of the circular
 * design. Instead of shifting everything right, step `front` one slot
 * LEFT (wrapping 0 -> cap-1) and write the value there.
 *
 *   cap 4, front 0:  [ A | B | ? | ? ]     pushAtFront(Z)
 *                      ^
 *   front = (0 - 1 + 4) % 4 = 3
 *                    [ A | B | ? | Z ]     list is now [Z, A, B]
 *                                    ^front
 *
 * Expand FIRST: expand() resets front to 0, so we must do it before
 * computing the new front, and it also guarantees cap > 0 for the %.
 */
void CircularArrayList::pushAtFront(int value) {
    if (numItems == cap) {
        expand();
    }
    front = (front - 1 + cap) % cap;   /* "+ cap" avoids a negative result */
    data[front] = value;
    numItems++;
}

/*
 * pushAtBack: the first free slot after the list is logical index
 * numItems, i.e. physical (front + numItems) % cap. `front` doesn't move.
 */
void CircularArrayList::pushAtBack(int value) {
    if (numItems == cap) {
        expand();
    }
    data[physicalIndex(numItems)] = value;
    numItems++;
}

/*
 * popFromFront: O(1). Step `front` one slot RIGHT (wrapping cap-1 -> 0).
 * The old front slot becomes free space; no need to erase it.
 */
void CircularArrayList::popFromFront() {
    if (isEmpty()) {
        throw std::runtime_error("popFromFront: list is empty");
    }
    front = (front + 1) % cap;
    numItems--;
}

/*
 * popFromBack: O(1). The last element is defined by front + numItems,
 * so just shrinking numItems drops it. `front` does not move.
 */
void CircularArrayList::popFromBack() {
    if (isEmpty()) {
        throw std::runtime_error("popFromBack: list is empty");
    }
    numItems--;
}

/*
 * Print in LOGICAL order: walk i = 0 .. numItems-1 and translate each
 * one. (Looping over data[0 .. cap-1] would print array order, which
 * is wrong once the list wraps around, and would print garbage slots.)
 */
std::string CircularArrayList::toString() const {
    std::stringstream ss;
    ss << "[";
    for (int i = 0; i < numItems; i++) {
        if (i > 0) ss << ", ";
        ss << data[physicalIndex(i)];
    }
    ss << "]";
    return ss.str();
}

/*
 * expand: allocate 2*cap+2 slots and "unwrap" the list into
 * new[0 .. numItems-1], then reset front to 0.
 *
 *   old (cap 4, front 2):  [ C | D | A | B ]     list = [A, B, C, D]
 *                                    ^front
 *   new (cap 10, front 0): [ A | B | C | D | ? | ? | ? | ? | ? | ? ]
 *
 * Why not just copy data[0..cap-1] straight across? Because the list
 * would then still "wrap" at the OLD cap, but the % math now uses the NEW
 * cap -- C and D would appear to be at the wrong logical positions.
 * Copying in logical order + front = 0 makes the new array consistent.
 *
 * When cap == 0 (first push), numItems == 0, so the loop body never runs
 * and physicalIndex (which would do % 0) is never called.
 */
void CircularArrayList::expand() {
    int  newCap  = 2 * cap + 2;
    int *newData = new int[newCap];
    for (int i = 0; i < numItems; i++) {
        newData[i] = data[physicalIndex(i)];   /* uses OLD front & cap */
    }
    delete [] data;
    data  = newData;
    cap   = newCap;
    front = 0;          /* must come AFTER the copy loop (it uses front) */
}
