/*
 * IntArrayList.cpp
 *
 * PROBLEM A1 -- SOLUTION (see IntArrayList.h for the full specification).
 *
 * Mental model for this whole file:
 *
 *        index:   0    1    2    3    4    5
 *        data:  [ 7 | 3 | 9 |  ? |  ? |  ? ]     numItems = 3, cap = 6
 *                 \_________/ \_____________/
 *                  the list    spare capacity (garbage -- never read it)
 *
 *   - `data` is ONE contiguous heap array of `cap` ints.
 *   - Only data[0 .. numItems-1] are real elements.
 *   - Any function that adds an element must first make sure there is room
 *     (numItems < cap); if not, it calls expand().
 *
 * Every function follows the same recipe:
 *   1. Check for errors FIRST (before changing anything), so a function
 *      that throws leaves the list unchanged.
 *   2. Make room / shift elements if needed.
 *   3. Update numItems.
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

/* ======================= Solution =========================== */

/*
 * Array constructor: copy the first `length` ints of `arr`.
 * The spec says size == capacity == length, so allocate EXACTLY `length`
 * slots. With length 0 we allocate nothing (data stays nullptr), which
 * matches the default constructor.
 */
IntArrayList::IntArrayList(const int *arr, int length) {
    data     = nullptr;
    numItems = length;
    cap      = length;
    if (length > 0) {
        data = new int[length];
        for (int i = 0; i < length; i++) {
            data[i] = arr[i];
        }
    }
}

/*
 * Copy constructor: make a DEEP copy.
 *
 * The compiler-generated copy constructor would do `data = other.data`,
 * so both lists would share ONE array. Then:
 *   - changing one list would change the other, and
 *   - both destructors would delete [] the same array (double delete -> crash).
 * So we give the new list its own array and copy the elements over.
 *
 * We don't need to free anything first: this object is brand new, so it
 * doesn't own an array yet.
 */
IntArrayList::IntArrayList(const IntArrayList &other) {
    data     = nullptr;
    numItems = other.numItems;
    cap      = other.cap;
    if (cap > 0) {
        data = new int[cap];
        for (int i = 0; i < numItems; i++) {   /* only the REAL elements */
            data[i] = other.data[i];
        }
    }
}

/*
 * Assignment operator:  a = b;   (a already exists and owns an array)
 *
 * Steps:
 *   1. Self-assignment check. Without it, `a = a` would delete a's array
 *      and then try to copy from the array it just deleted.
 *   2. Build the new copy in a NEW array first.
 *   3. Only then delete [] the old array and switch over.
 *   4. return *this so chains like  a = b = c;  work.
 *
 * Why allocate before deleting (step 2 before step 3)? If `new` threw
 * (out of memory), the list would still be intact. Deleting first and then
 * allocating also passes the tests -- it's just less safe.
 */
IntArrayList &IntArrayList::operator=(const IntArrayList &other) {
    if (this == &other) {
        return *this;
    }

    int *newData = nullptr;
    if (other.cap > 0) {
        newData = new int[other.cap];
        for (int i = 0; i < other.numItems; i++) {
            newData[i] = other.data[i];
        }
    }

    delete [] data;          /* free OUR old array (no leak) */
    data     = newData;
    numItems = other.numItems;
    cap      = other.cap;
    return *this;
}

/*
 * Destructor: free the one heap array we own.
 * `delete [] nullptr` is legal and does nothing, so an empty list is fine.
 * Use delete [] (with brackets) because the array was made with new [].
 */
IntArrayList::~IntArrayList() {
    delete [] data;
}

bool IntArrayList::isEmpty() const {
    return numItems == 0;
}

int IntArrayList::first() const {
    if (isEmpty()) {
        throw std::runtime_error("first: list is empty");
    }
    return data[0];
}

int IntArrayList::last() const {
    if (isEmpty()) {
        throw std::runtime_error("last: list is empty");
    }
    return data[numItems - 1];               /* last REAL element, not cap - 1 */
}

/*
 * Valid indices are 0 .. numItems-1. Note we compare against numItems,
 * NOT cap: slots between numItems and cap exist in memory but are not
 * part of the list.
 */
int IntArrayList::elementAt(int index) const {
    if (index < 0 || index >= numItems) {
        throw std::range_error("elementAt: index out of range");
    }
    return data[index];                      /* O(1): direct array access */
}

void IntArrayList::replaceAt(int value, int index) {
    if (index < 0 || index >= numItems) {
        throw std::range_error("replaceAt: index out of range");
    }
    data[index] = value;
}

/* Linear search from the front, so we find the FIRST occurrence. O(n). */
int IntArrayList::indexOf(int value) const {
    for (int i = 0; i < numItems; i++) {
        if (data[i] == value) {
            return i;
        }
    }
    return -1;
}

/*
 * pushAtBack: the cheap case for an ArrayList. The first free slot is
 * data[numItems], so nothing has to shift.
 *
 * Amortized O(1): most calls just write one slot. Occasionally expand()
 * costs O(n), but because capacity roughly DOUBLES each time, those
 * expensive calls are rare enough that the average cost per push is O(1).
 */
void IntArrayList::pushAtBack(int value) {
    if (numItems == cap) {
        expand();
    }
    data[numItems] = value;
    numItems++;
}

/*
 * pushAtFront is just "insert at index 0" -- every element has to move one
 * slot right to make room, so it is O(n). insertAt does exactly that.
 */
void IntArrayList::pushAtFront(int value) {
    insertAt(value, 0);
}

/*
 * insertAt: put `value` at `index`, shifting data[index .. numItems-1]
 * one slot to the RIGHT.
 *
 * Note index == numItems IS allowed (it means "append").
 *
 * The shift must go from the BACK toward the front:
 *
 *   insert 5 at index 1 in [1, 2, 3]:
 *     [1 | 2 | 3 | ?]   data[3] = data[2]  -> [1 | 2 | 3 | 3]
 *                       data[2] = data[1]  -> [1 | 2 | 2 | 3]
 *                       data[1] = 5        -> [1 | 5 | 2 | 3]
 *
 * If you shifted front-to-back instead, data[2] = data[1] would overwrite
 * the 3 before you had copied it -- you'd end up with [1, 5, 2, 2].
 */
void IntArrayList::insertAt(int value, int index) {
    if (index < 0 || index > numItems) {       /* > not >= : size is allowed */
        throw std::range_error("insertAt: index out of range");
    }
    if (numItems == cap) {
        expand();
    }
    for (int i = numItems; i > index; i--) {
        data[i] = data[i - 1];
    }
    data[index] = value;
    numItems++;
}

/*
 * popFromFront: everything shifts left one slot, so O(n).
 * We check for "empty" here ourselves because the spec wants a
 * runtime_error (removeAt would throw a range_error instead).
 */
void IntArrayList::popFromFront() {
    if (isEmpty()) {
        throw std::runtime_error("popFromFront: list is empty");
    }
    removeAt(0);
}

/*
 * popFromBack: O(1). We don't need to erase anything -- shrinking numItems
 * means the old last slot is no longer part of the list (it becomes
 * spare capacity and will simply be overwritten later).
 */
void IntArrayList::popFromBack() {
    if (isEmpty()) {
        throw std::runtime_error("popFromBack: list is empty");
    }
    numItems--;
}

/*
 * removeAt: close the gap by shifting data[index+1 .. numItems-1] one slot
 * to the LEFT. This time we go front-to-back (the mirror image of
 * insertAt), so each element is copied before it is overwritten:
 *
 *   remove index 1 from [1, 2, 3, 4]:
 *     data[1] = data[2] -> [1 | 3 | 3 | 4]
 *     data[2] = data[3] -> [1 | 3 | 4 | 4]
 *     numItems-- (3)    ->  list is [1, 3, 4]
 */
void IntArrayList::removeAt(int index) {
    if (index < 0 || index >= numItems) {
        throw std::range_error("removeAt: index out of range");
    }
    for (int i = index; i < numItems - 1; i++) {
        data[i] = data[i + 1];
    }
    numItems--;
}

/*
 * clear: the list is empty, but we KEEP the array (the spec says capacity
 * is unchanged). Setting numItems to 0 is enough -- old values become
 * spare capacity. O(1).
 */
void IntArrayList::clear() {
    numItems = 0;
}

/*
 * shrinkToFit: reallocate so that cap == numItems (no wasted slots).
 * Same "allocate new, copy, delete old" pattern as expand(), just smaller.
 * Special case: an empty list keeps NO array (data = nullptr, cap = 0).
 */
void IntArrayList::shrinkToFit() {
    int *newData = nullptr;
    if (numItems > 0) {
        newData = new int[numItems];
        for (int i = 0; i < numItems; i++) {
            newData[i] = data[i];
        }
    }
    delete [] data;
    data = newData;
    cap  = numItems;
}

/*
 * expand: grow the array to 2 * cap + 2 slots.
 *
 * Why "2 * cap + 2" and not "cap + 1"?
 *   - Multiplying makes expansions rare: n pushes cause only about log2(n)
 *     expansions, and the total copying is O(n) -> amortized O(1) per push.
 *   - Adding a constant would expand on (almost) every push: total copying
 *     1 + 2 + ... + n = O(n^2).
 *   - The "+ 2" handles cap == 0 (2 * 0 would still be 0).
 *
 * You cannot resize a C++ array in place, so the pattern is always:
 *   allocate bigger array -> copy elements -> delete [] old -> point at new.
 */
void IntArrayList::expand() {
    int  newCap  = 2 * cap + 2;
    int *newData = new int[newCap];
    for (int i = 0; i < numItems; i++) {
        newData[i] = data[i];
    }
    delete [] data;   /* forgetting this line = memory leak on every expand */
    data = newData;
    cap  = newCap;
}
