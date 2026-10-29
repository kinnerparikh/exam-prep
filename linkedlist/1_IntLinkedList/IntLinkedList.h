/*
 * IntLinkedList.h
 *
 * PROBLEM L1 -- Implement a DOUBLY-linked list of ints with front and back
 * pointers.
 *
 *        front                                   back
 *          |                                       |
 *          v                                       v
 *   null <- [ 1 ] <-> [ 2 ] <-> [ 3 ] <-> [ 4 ] -> null
 *
 * Implement every function in IntLinkedList.cpp. Do not change the public
 * interface, the Node struct, or the data members; you MAY add private
 * helpers. Do NOT use std::list / std::vector or any other STL container.
 *
 * Create nodes with the Node constructor, e.g.
 *       Node *n = new Node(value, prevPtr, nextPtr);
 * and free them with `delete n;`. (Node counts how many nodes are alive so
 * the tests can detect leaks -- every node you `new` must eventually be
 * `delete`d.)
 *
 * ---------------------------------------------------------------------------
 * REQUIRED BEHAVIOR (the tests check all of this)
 * ---------------------------------------------------------------------------
 *  Invariants -- must be true after EVERY public function:
 *    - Empty list: front == nullptr, back == nullptr, numItems == 0.
 *    - front->prev == nullptr and back->next == nullptr.
 *    - For every node n with a next: n->next->prev == n.
 *    - numItems == the number of nodes.
 *
 *  Constructors / Big Three
 *    - IntLinkedList(arr, length): list holding arr[0..length-1] in order.
 *    - Copy constructor and operator= make DEEP copies (brand-new nodes).
 *      operator= must handle self-assignment and must not leak old nodes.
 *    - The destructor deletes every node.
 *
 *  Errors
 *    - elementAt / replaceAt / removeAt with index outside [0, size)
 *          -> throw std::range_error
 *    - insertAt with index outside [0, size]   (size itself is allowed)
 *          -> throw std::range_error
 *    - first / last / popFromFront / popFromBack on an empty list
 *          -> throw std::runtime_error
 *
 *  Other
 *    - indexOf(value): index of the FIRST occurrence, or -1.
 *    - insertInOrder(value): assuming the list is sorted ascending, insert
 *      value immediately BEFORE the first element that is >= value (or at
 *      the back if there is none), so the list stays sorted.
 *    - toString()        -> "[1, 2, 3]"   walking front -> back via next
 *      toReverseString() -> "[3, 2, 1]"   walking back -> front via prev
 *      (toReverseString is how the tests check your prev pointers!)
 *
 *  Big-O targets (know WHY for the exam):
 *    pushAtFront/pushAtBack/popFromFront/popFromBack/first/last: O(1)
 *    elementAt/insertAt/removeAt/indexOf/insertInOrder: O(n)
 *
 *  Challenge: write elementAt (or the destructor) RECURSIVELY using a private
 *  helper that takes a Node *.
 */

#ifndef INT_LINKED_LIST_H
#define INT_LINKED_LIST_H

#include <string>

class IntLinkedList {
public:
    IntLinkedList();
    IntLinkedList(const int *arr, int length);
    IntLinkedList(const IntLinkedList &other);
    IntLinkedList &operator=(const IntLinkedList &other);
    ~IntLinkedList();

    int  size() const;
    bool isEmpty() const;

    int  first() const;
    int  last() const;
    int  elementAt(int index) const;
    void replaceAt(int value, int index);
    int  indexOf(int value) const;

    void pushAtFront(int value);
    void pushAtBack(int value);
    void insertAt(int value, int index);
    void insertInOrder(int value);

    void popFromFront();
    void popFromBack();
    void removeAt(int index);
    void clear();

    std::string toString() const;
    std::string toReverseString() const;

    /* For the tests: how many Nodes currently exist (in ALL lists). */
    static int liveNodeCount() { return liveNodes(); }

private:
    static int &liveNodes() {
        static int count = 0;
        return count;
    }

    struct Node {
        int   data;
        Node *prev;
        Node *next;

        Node(int value, Node *prevNode, Node *nextNode)
            : data(value), prev(prevNode), next(nextNode) {
            liveNodes()++;
        }
        ~Node() {
            liveNodes()--;
            prev = next = nullptr; /* so using a node after delete fails fast */
        }
    };

    Node *front;
    Node *back;
    int   numItems;

    /* Add private helper declarations here. */

    /* SOLUTION helpers (see IntLinkedList.cpp). */
    Node *nodeAt(int index) const;          /* requires 0 <= index < size */
    void  insertBefore(Node *curr, int value);
};

#endif
