/*
 * NumList.h
 *
 * A small, ALREADY WORKING dynamic array of ints. It is the starting point
 * for problems A2 and A3, where you write one extra member function each --
 * just like an exam question that says "here is the class definition, write
 * member function X".
 *
 * You do not need to touch NumList.cpp. You MAY add private helper
 * declarations at the bottom of this class if you want them.
 *
 * Representation (same as IntArrayList):
 *   data[0 .. numItems-1] are the elements, `cap` is the array's length.
 *   An empty NumList may have data == nullptr and cap == 0.
 */

#ifndef NUM_LIST_H
#define NUM_LIST_H

#include <initializer_list>
#include <string>
#include <vector>

class NumList {
public:
    NumList();
    NumList(std::initializer_list<int> values); /* NumList list{1, 2, 3}; */
    explicit NumList(const std::vector<int> &values);
    NumList(const NumList &other);
    NumList &operator=(const NumList &other);
    ~NumList();

    int              size() const;
    int              capacity() const;
    int              elementAt(int index) const; /* throws std::range_error */
    void             pushAtBack(int value);
    std::string      toString() const;           /* "[1, 2, 3]" */
    std::vector<int> toVector() const;

    /* ================= PRACTICE PROBLEMS ================= */

    /* A2 -- implement in arraylist/2_removeAll/removeAll.cpp */
    int removeAll(int value);

    /* A3 -- implement in arraylist/3_mergeFrom/mergeFrom.cpp */
    void mergeFrom(const NumList &other);

private:
    int *data;
    int  numItems;
    int  cap;

    void expand();
};

#endif
