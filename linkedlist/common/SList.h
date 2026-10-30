/*
 * SList.h
 *
 * A small, ALREADY WORKING singly-linked list of ints (front pointer only).
 * It is the starting point for problems L2, L3 and L4, where you write one
 * member function each -- like an exam question that says "here is the class
 * definition, write member function X".
 *
 *    front
 *      |
 *      v
 *    [ 1 ] -> [ 2 ] -> [ 3 ] -> nullptr          numItems == 3
 *
 * You do not need to touch SList.cpp. You MAY add private helper
 * declarations at the bottom of this class (e.g. for a recursive solution).
 *
 * Invariants your functions must preserve:
 *   - front == nullptr exactly when the list is empty
 *   - the last node's next is nullptr
 *   - numItems == the number of nodes
 *   - every node you remove is deleted; every node that stays is not
 *
 * Create nodes with  new Node(value, nextPtr)  and free them with  delete.
 */

#ifndef SLIST_H
#define SLIST_H

#include <initializer_list>
#include <string>
#include <vector>

class SList {
public:
    SList();
    SList(std::initializer_list<int> values); /* SList list{1, 2, 3}; */
    explicit SList(const std::vector<int> &values);
    ~SList();

    /* Copying is disabled -- not needed for these problems. */
    SList(const SList &other) = delete;
    SList &operator=(const SList &other) = delete;

    int  size() const;
    bool isEmpty() const;
    void pushAtFront(int value);
    void pushAtBack(int value); /* O(n): walks to the end */

    /* Both of these check the list's structure as they walk it, and throw
       std::logic_error if the number of nodes doesn't match numItems (for
       example a missing nullptr at the end, or a cycle). */
    std::string      toString() const; /* "[1, 2, 3]" */
    std::vector<int> toVector() const;

    /* For the tests: the address of every node, front to back. Lets the
       tests check that you RE-LINKED existing nodes instead of making new
       ones or just swapping data. */
    std::vector<const void *> nodeAddresses() const;

    /* For the tests: how many Nodes currently exist (in ALL lists). */
    static int liveNodeCount();

    /* ================= PRACTICE PROBLEMS ================= */

    /* L2 -- implement in linkedlist/2_reverse/reverse.cpp */
    void reverse();

    /* L3 -- implement in linkedlist/3_removeAll/removeAll.cpp */
    int removeAll(int value);

    /* L4 -- implement in linkedlist/4_mergeSorted/mergeSorted.cpp */
    void mergeSorted(SList &other);

private:
    static int &liveNodes();

    struct Node {
        int   data;
        Node *next;

        Node(int value, Node *nextNode) : data(value), next(nextNode) {
            liveNodes()++;
        }
        ~Node() {
            liveNodes()--;
            next = nullptr; /* so using a node after delete fails fast */
        }
    };

    Node *front;
    int   numItems;

    std::vector<const Node *> walk() const;

    /* Add private helper declarations here. */
};

#endif
