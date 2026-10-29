/*
 * IntLinkedList.cpp
 *
 * PROBLEM L1 -- SOLUTION. See IntLinkedList.h for the full specification
 * and invariants.
 *
 * Picture to keep in your head:
 *
 *        front                                   back
 *          |                                       |
 *          v                                       v
 *   null <- [ 1 ] <-> [ 2 ] <-> [ 3 ] <-> [ 4 ] -> null
 *
 * For every insert/remove, ask the checklist:
 *   - What if the list is empty?
 *   - What if I'm changing the FRONT node? The BACK node? The ONLY node?
 *   - Did I update BOTH next and prev? Did I update front/back? numItems?
 *
 * The pattern used throughout this solution: handle the END cases
 * (front/back) in pushAt.../popFrom..., and let the "middle" cases reuse
 * them. That keeps every tricky nullptr check in one place.
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

/* ======================= Solution =========================== */

/* ---------------------------- helpers ----------------------------- */

/*
 * nodeAt: return a pointer to the node at `index` (caller has already
 * checked 0 <= index < numItems).
 *
 * A linked list has no "data[index]": we must follow pointers one node
 * at a time, so this is O(n). Because the list is DOUBLY linked with a
 * back pointer, we can start from whichever end is closer -- still O(n)
 * in Big-O terms, but at most n/2 steps instead of n.
 *
 * Recursive version (the header's "challenge"), for comparison:
 *
 *     Node *nodeAtRec(Node *curr, int index) const {
 *         if (index == 0) return curr;                // base case
 *         return nodeAtRec(curr->next, index - 1);    // one step closer
 *     }
 *     ... called as nodeAtRec(front, index)
 *
 * Each recursive call uses a stack frame, so it takes O(n) extra space
 * and can overflow the stack on a huge list. The loop uses O(1).
 */
IntLinkedList::Node *IntLinkedList::nodeAt(int index) const {
    if (index < numItems / 2) {
        Node *curr = front;
        for (int i = 0; i < index; i++) {
            curr = curr->next;
        }
        return curr;
    } else {
        Node *curr = back;
        for (int i = numItems - 1; i > index; i--) {
            curr = curr->prev;
        }
        return curr;
    }
}

/*
 * insertBefore: insert `value` immediately before node `curr`.
 * `curr == nullptr` means "before the end", i.e. append.
 * Used by both insertAt and insertInOrder.
 *
 * Middle case, inserting N before C (B is C's prev):
 *
 *   before:   [ B ] <-> [ C ]
 *
 *   1. N = new Node(value, B, C)    N already points at its neighbors:
 *                                       B <- N -> C
 *   2. B->next = N                  [ B ] -> [ N ]
 *   3. C->prev = N                  [ N ] <- [ C ]
 *
 *   after:    [ B ] <-> [ N ] <-> [ C ]
 *
 * Order matters: we read C->prev (to find B) BEFORE overwriting C->prev.
 */
void IntLinkedList::insertBefore(Node *curr, int value) {
    if (curr == nullptr) {          /* past the last node -> new back */
        pushAtBack(value);
        return;
    }
    if (curr == front) {            /* before the first node -> new front */
        pushAtFront(value);
        return;
    }

    /* Middle: curr has a real prev node, so no nullptr checks needed. */
    Node *before = curr->prev;
    Node *n = new Node(value, before, curr);
    before->next = n;
    curr->prev   = n;
    numItems++;
}

/* ---------------------- constructors / Big Three ---------------------- */

/*
 * Start as an empty list, then append each element. pushAtBack is O(1)
 * thanks to the back pointer, so this is O(length) total.
 * (Using pushAtFront would build the list in REVERSE order.)
 */
IntLinkedList::IntLinkedList(const int *arr, int length) {
    front    = nullptr;
    back     = nullptr;
    numItems = 0;
    for (int i = 0; i < length; i++) {
        pushAtBack(arr[i]);
    }
}

/*
 * Copy constructor: DEEP copy -- create a brand-new node for every node in
 * `other`. (Copying just the front/back pointers would make both lists
 * share nodes, and both destructors would delete them -> double delete.)
 *
 * We MUST initialize our members first: this object is brand new, so its
 * pointers hold garbage until we set them, and pushAtBack reads them.
 */
IntLinkedList::IntLinkedList(const IntLinkedList &other) {
    front    = nullptr;
    back     = nullptr;
    numItems = 0;
    for (Node *curr = other.front; curr != nullptr; curr = curr->next) {
        pushAtBack(curr->data);
    }
}

/*
 * operator=:  a = b;   `a` already has nodes of its own.
 *   1. Self-assignment check: without it, `a = a` would clear() a and then
 *      try to copy from the (now empty) list -- a would lose everything.
 *   2. clear() frees our OLD nodes (otherwise they leak).
 *   3. Deep copy other's elements, like the copy constructor.
 *   4. return *this so  a = b = c;  works.
 */
IntLinkedList &IntLinkedList::operator=(const IntLinkedList &other) {
    if (this == &other) {
        return *this;
    }
    clear();
    for (Node *curr = other.front; curr != nullptr; curr = curr->next) {
        pushAtBack(curr->data);
    }
    return *this;
}

/* The destructor and clear() do the same job, so reuse it. */
IntLinkedList::~IntLinkedList() {
    clear();
}

/* --------------------------- accessors --------------------------- */

bool IntLinkedList::isEmpty() const {
    return numItems == 0;          /* (front == nullptr) works too */
}

int IntLinkedList::first() const {
    if (isEmpty()) {
        throw std::runtime_error("first: list is empty");
    }
    return front->data;
}

/* O(1) only because we keep a back pointer. A singly-linked list with
   no back pointer would have to walk the whole list: O(n). */
int IntLinkedList::last() const {
    if (isEmpty()) {
        throw std::runtime_error("last: list is empty");
    }
    return back->data;
}

/* Check the index BEFORE walking, or nodeAt could walk off the end
   and dereference nullptr. */
int IntLinkedList::elementAt(int index) const {
    if (index < 0 || index >= numItems) {
        throw std::range_error("elementAt: index out of range");
    }
    return nodeAt(index)->data;
}

void IntLinkedList::replaceAt(int value, int index) {
    if (index < 0 || index >= numItems) {
        throw std::range_error("replaceAt: index out of range");
    }
    nodeAt(index)->data = value;   /* just change the data; links untouched */
}

/* Walk from the front, counting, so we find the FIRST occurrence. */
int IntLinkedList::indexOf(int value) const {
    int index = 0;
    for (Node *curr = front; curr != nullptr; curr = curr->next) {
        if (curr->data == value) {
            return index;
        }
        index++;
    }
    return -1;
}

/* ---------------------------- inserting ---------------------------- */

/*
 * pushAtFront: O(1) -- no shifting, unlike an ArrayList.
 *
 *   new node n:  null <- [ n ] -> (old front)
 *
 * Two cases:
 *   - list was EMPTY: n is also the back.
 *   - otherwise: the old front must point BACK at n (old front->prev = n).
 *     Forgetting this breaks toReverseString.
 */
void IntLinkedList::pushAtFront(int value) {
    Node *n = new Node(value, nullptr, front);
    if (front == nullptr) {
        back = n;
    } else {
        front->prev = n;
    }
    front = n;
    numItems++;
}

/* Mirror image of pushAtFront: swap front<->back and prev<->next. */
void IntLinkedList::pushAtBack(int value) {
    Node *n = new Node(value, back, nullptr);
    if (back == nullptr) {
        front = n;
    } else {
        back->next = n;
    }
    back = n;
    numItems++;
}

/*
 * insertAt: index == numItems is allowed and means "append".
 * Find the node currently AT `index`, and insert in front of it. For
 * index == numItems there is no such node, so we pass nullptr ("append").
 * O(n) because of the walk; the relinking itself is O(1).
 */
void IntLinkedList::insertAt(int value, int index) {
    if (index < 0 || index > numItems) {        /* > not >= : size is OK */
        throw std::range_error("insertAt: index out of range");
    }
    Node *curr = (index == numItems) ? nullptr : nodeAt(index);
    insertBefore(curr, value);
}

/*
 * insertInOrder: find the first node whose data is >= value and insert in
 * front of it. If every element is < value, the walk ends at nullptr and
 * we append. Using < (not <=) puts the new value BEFORE any equal values,
 * exactly as the spec says.
 *
 *   [1, 3, 5].insertInOrder(4): 1 < 4 -> 3 < 4 -> 5 >= 4, stop at 5
 *                               -> insert before 5 -> [1, 3, 4, 5]
 *
 * We walk with a pointer (not an index), so the list is walked once.
 */
void IntLinkedList::insertInOrder(int value) {
    Node *curr = front;
    while (curr != nullptr && curr->data < value) {
        curr = curr->next;
    }
    insertBefore(curr, value);
}

/* ---------------------------- removing ---------------------------- */

/*
 * popFromFront: O(1).
 *   1. Remember the old front (we need it to delete it).
 *   2. Move front to the next node.
 *   3. If the list is now empty, back must become nullptr too (otherwise
 *      back would dangle, pointing at the node we're deleting).
 *      Otherwise the new front's prev must become nullptr.
 *   4. delete the old node LAST -- after we're done reading its next.
 */
void IntLinkedList::popFromFront() {
    if (isEmpty()) {
        throw std::runtime_error("popFromFront: list is empty");
    }
    Node *oldFront = front;
    front = front->next;
    if (front == nullptr) {
        back = nullptr;
    } else {
        front->prev = nullptr;
    }
    delete oldFront;
    numItems--;
}

/*
 * popFromBack: O(1) ONLY because the list is doubly linked: back->prev
 * gives us the new back immediately. In a singly-linked list we'd have to
 * walk from the front to find the second-to-last node: O(n).
 */
void IntLinkedList::popFromBack() {
    if (isEmpty()) {
        throw std::runtime_error("popFromBack: list is empty");
    }
    Node *oldBack = back;
    back = back->prev;
    if (back == nullptr) {
        front = nullptr;
    } else {
        back->next = nullptr;
    }
    delete oldBack;
    numItems--;
}

/*
 * removeAt: the end cases reuse popFromFront/popFromBack (which already
 * handle front/back/nullptr correctly). A middle node always has a real
 * prev AND a real next, so we can relink without nullptr checks:
 *
 *   before:   [ P ] <-> [ C ] <-> [ N ]       remove C (curr)
 *
 *   P->next = N   (curr->prev->next = curr->next)
 *   N->prev = P   (curr->next->prev = curr->prev)
 *
 *   after:    [ P ] <-> [ N ]                 then delete C
 */
void IntLinkedList::removeAt(int index) {
    if (index < 0 || index >= numItems) {
        throw std::range_error("removeAt: index out of range");
    }
    if (index == 0) {
        popFromFront();
        return;
    }
    if (index == numItems - 1) {
        popFromBack();
        return;
    }

    Node *curr = nodeAt(index);
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    delete curr;
    numItems--;
}

/*
 * clear: delete every node.
 *
 * The classic bug is:
 *       delete curr;
 *       curr = curr->next;     // WRONG: reading a node we just deleted!
 * So save `next` FIRST, then delete, then move on.
 *
 * Iterative on purpose: a recursive destructor would use one stack frame
 * per node and could overflow on a very long list.
 */
void IntLinkedList::clear() {
    Node *curr = front;
    while (curr != nullptr) {
        Node *next = curr->next;   /* 1. save the rest of the list */
        delete curr;               /* 2. free this node            */
        curr = next;               /* 3. move on                   */
    }
    front    = nullptr;            /* restore the empty-list invariant */
    back     = nullptr;
    numItems = 0;
}

/*
 * Same as toString, but start at `back` and follow `prev`. If any prev
 * pointer is wrong, this string will be wrong -- that's how the tests
 * catch "forgot to update prev" bugs.
 */
std::string IntLinkedList::toReverseString() const {
    std::stringstream ss;
    ss << "[";
    for (Node *curr = back; curr != nullptr; curr = curr->prev) {
        if (curr != back) ss << ", ";
        ss << curr->data;
    }
    ss << "]";
    return ss.str();
}
