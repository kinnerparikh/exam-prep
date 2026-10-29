/*
 * CircularArrayList_tests.cpp -- tests for PROBLEM A4.
 *
 * Run all:   make al4
 */

#include "CircularArrayList.h"
#include "test_framework.h"

#include <cstdlib>
#include <deque>
#include <stdexcept>
#include <string>
#include <vector>

using IntVec = std::vector<int>;
using testing::liveArrayCount;
using testing::totalArrayAllocations;

static IntVec contents(const CircularArrayList &list) {
    IntVec out;
    for (int i = 0; i < list.size(); i++) out.push_back(list.elementAt(i));
    return out;
}

/* A reference model of exactly how `front` and `capacity` must behave. */
struct Model {
    std::deque<int> items;
    int cap = 0;
    int front = 0;

    void expandIfFull() {
        if (static_cast<int>(items.size()) == cap) {
            cap = 2 * cap + 2;
            front = 0;
        }
    }
    void pushFront(int v) {
        expandIfFull();
        front = (front - 1 + cap) % cap;
        items.push_front(v);
    }
    void pushBack(int v) {
        expandIfFull();
        items.push_back(v);
    }
    void popFront() {
        front = (front + 1) % cap;
        items.pop_front();
    }
    void popBack() { items.pop_back(); }
    IntVec vec() const { return IntVec(items.begin(), items.end()); }
};

/* ======================= basics ======================= */

TEST(new_list_is_empty) {
    CircularArrayList list;
    ASSERT_EQ(0, list.size());
    ASSERT_TRUE(list.isEmpty());
    ASSERT_EQ(0, list.capacity());
    ASSERT_EQ(0, list.frontIndex());
    ASSERT_EQ("[]", list.toString());
}

TEST(new_list_allocates_nothing) {
    long before = totalArrayAllocations();
    CircularArrayList list;
    ASSERT_EQ(before, totalArrayAllocations());
}

TEST(pushAtBack_keeps_order) {
    CircularArrayList list;
    list.pushAtBack(1);
    list.pushAtBack(2);
    list.pushAtBack(3);
    ASSERT_EQ(IntVec({1, 2, 3}), contents(list));
    ASSERT_FALSE(list.isEmpty());
}

TEST(pushAtFront_reverses_order) {
    CircularArrayList list;
    list.pushAtFront(1);
    list.pushAtFront(2);
    list.pushAtFront(3);
    ASSERT_EQ(IntVec({3, 2, 1}), contents(list));
}

TEST(mixed_pushes) {
    CircularArrayList list;
    list.pushAtBack(3);
    list.pushAtFront(2);
    list.pushAtBack(4);
    list.pushAtFront(1);
    list.pushAtBack(5);
    ASSERT_EQ(IntVec({1, 2, 3, 4, 5}), contents(list));
}

TEST(capacity_grows_as_2n_plus_2) {
    CircularArrayList list;
    IntVec expectedCaps = {2, 2, 6, 6, 6, 6, 14, 14, 14, 14, 14, 14, 14, 14, 30};
    for (size_t i = 0; i < expectedCaps.size(); i++) {
        if (i % 2 == 0) list.pushAtFront(static_cast<int>(i));
        else            list.pushAtBack(static_cast<int>(i));
        ASSERT_EQ_MSG(expectedCaps[i], list.capacity(),
                      "after " + std::to_string(i + 1) + " pushes");
    }
}

/* ======================= wrap-around ======================= */

TEST(pushAtFront_into_empty_wraps_to_last_slot) {
    CircularArrayList list;
    list.pushAtFront(7); /* expand to cap 2, then front moves left from 0 -> 1 */
    ASSERT_EQ(2, list.capacity());
    ASSERT_EQ(1, list.frontIndex());
    ASSERT_EQ(IntVec({7}), contents(list));
}

TEST(pushAtFront_wraps_front_to_end_of_array) {
    CircularArrayList list;
    list.pushAtBack(1);  /* cap 2, front 0: data = [1, ?] */
    list.pushAtFront(0); /* front wraps to 1:  data = [1, 0] */
    ASSERT_EQ(1, list.frontIndex());
    ASSERT_EQ(IntVec({0, 1}), contents(list));
    ASSERT_EQ("[0, 1]", list.toString());
}

TEST(pushAtFront_does_not_shift_existing_elements) {
    CircularArrayList list;
    for (int i = 0; i < 6; i++) list.pushAtBack(i); /* cap 6, front 0 */
    list.popFromBack();                             /* room for one more */
    list.pushAtFront(-1);
    ASSERT_EQ_MSG(5, list.frontIndex(),
                  "pushAtFront should move front left (wrapping), not shift");
    ASSERT_EQ(IntVec({-1, 0, 1, 2, 3, 4}), contents(list));
}

TEST(expand_unwraps_and_resets_front) {
    CircularArrayList list;
    list.pushAtBack(2);  /* data = [2, ?], front 0 */
    list.pushAtFront(1); /* data = [2, 1], front 1 -- full and wrapped */
    list.pushAtBack(3);  /* expand: new data = [1, 2, ...], front 0 */
    ASSERT_EQ(6, list.capacity());
    ASSERT_EQ(0, list.frontIndex());
    ASSERT_EQ(IntVec({1, 2, 3}), contents(list));
}

TEST(expand_from_pushAtFront_when_wrapped) {
    CircularArrayList list;
    for (int i = 5; i >= 0; i--) list.pushAtFront(i); /* fills cap 6 */
    ASSERT_EQ(IntVec({0, 1, 2, 3, 4, 5}), contents(list));
    list.pushAtFront(-1); /* expand to 14, front 0 then wraps to 13 */
    ASSERT_EQ(14, list.capacity());
    ASSERT_EQ(13, list.frontIndex());
    ASSERT_EQ(IntVec({-1, 0, 1, 2, 3, 4, 5}), contents(list));
}

TEST(popFromFront_moves_front_right) {
    CircularArrayList list;
    for (int i = 0; i < 5; i++) list.pushAtBack(i); /* cap 6, front 0 */
    list.popFromFront();
    ASSERT_EQ(1, list.frontIndex());
    list.popFromFront();
    ASSERT_EQ(2, list.frontIndex());
    ASSERT_EQ(IntVec({2, 3, 4}), contents(list));
}

TEST(popFromFront_wraps_front_back_to_zero) {
    CircularArrayList list;
    list.pushAtBack(2);
    list.pushAtFront(1); /* front 1 */
    list.popFromFront(); /* front wraps 1 -> 0 */
    ASSERT_EQ(0, list.frontIndex());
    ASSERT_EQ(IntVec({2}), contents(list));
}

TEST(pushAtBack_wraps_around_end_of_array) {
    CircularArrayList list;
    for (int i = 0; i < 6; i++) list.pushAtBack(i); /* cap 6, full, front 0 */
    list.popFromFront();
    list.popFromFront();                            /* front 2 */
    list.pushAtBack(6);                             /* goes to data[0] */
    list.pushAtBack(7);                             /* goes to data[1] */
    ASSERT_EQ(6, list.capacity());
    ASSERT_EQ(2, list.frontIndex());
    ASSERT_EQ(IntVec({2, 3, 4, 5, 6, 7}), contents(list));
    ASSERT_EQ(7, list.last());
}

TEST(queue_usage_reuses_space_without_growing) {
    CircularArrayList list;
    list.pushAtBack(0);
    list.pushAtBack(1);
    list.pushAtBack(2); /* cap 6 */
    for (int i = 3; i < 1000; i++) {
        list.pushAtBack(i);
        list.popFromFront();
        ASSERT_EQ_MSG(6, list.capacity(),
                      "a queue that never holds > 4 items should never grow");
    }
    ASSERT_EQ(IntVec({997, 998, 999}), contents(list));
}

/* ======================= access ======================= */

TEST(elementAt_with_wrapped_data) {
    CircularArrayList list;
    for (int i = 0; i < 6; i++) list.pushAtBack(i * 10);
    list.popFromFront();
    list.popFromFront();
    list.popFromFront();
    list.pushAtBack(60);
    list.pushAtBack(70); /* logical [30, 40, 50, 60, 70], physically wrapped */
    ASSERT_EQ(30, list.elementAt(0));
    ASSERT_EQ(50, list.elementAt(2));
    ASSERT_EQ(60, list.elementAt(3));
    ASSERT_EQ(70, list.elementAt(4));
}

TEST(elementAt_out_of_range_throws_range_error) {
    CircularArrayList list;
    ASSERT_THROWS(std::range_error, list.elementAt(0));
    list.pushAtBack(1);
    list.pushAtBack(2);
    ASSERT_THROWS(std::range_error, list.elementAt(-1));
    ASSERT_THROWS(std::range_error, list.elementAt(2));
}

TEST(replaceAt_with_wrapped_data) {
    CircularArrayList list;
    list.pushAtBack(2);
    list.pushAtFront(1); /* wrapped */
    list.replaceAt(10, 0);
    list.replaceAt(20, 1);
    ASSERT_EQ(IntVec({10, 20}), contents(list));
    ASSERT_THROWS(std::range_error, list.replaceAt(0, 2));
    ASSERT_THROWS(std::range_error, list.replaceAt(0, -1));
}

TEST(first_and_last) {
    CircularArrayList list;
    list.pushAtBack(5);
    ASSERT_EQ(5, list.first());
    ASSERT_EQ(5, list.last());
    list.pushAtFront(4);
    list.pushAtBack(6);
    ASSERT_EQ(4, list.first());
    ASSERT_EQ(6, list.last());
}

TEST(last_when_back_wrapped_to_index_zero) {
    CircularArrayList list;
    list.pushAtBack(1);
    list.pushAtBack(2);   /* cap 2, front 0 */
    list.popFromFront();  /* front 1, [2] */
    list.pushAtBack(3);   /* lands in data[0] */
    ASSERT_EQ(3, list.last());
    ASSERT_EQ(2, list.first());
}

TEST(first_last_on_empty_throw_runtime_error) {
    CircularArrayList list;
    ASSERT_THROWS(std::runtime_error, list.first());
    ASSERT_THROWS(std::runtime_error, list.last());
}

/* ======================= removal ======================= */

TEST(popFromBack_basic) {
    CircularArrayList list;
    for (int i = 1; i <= 4; i++) list.pushAtBack(i);
    int frontBefore = list.frontIndex();
    list.popFromBack();
    ASSERT_EQ(IntVec({1, 2, 3}), contents(list));
    ASSERT_EQ(frontBefore, list.frontIndex());
}

TEST(pop_on_empty_throws_runtime_error) {
    CircularArrayList list;
    ASSERT_THROWS(std::runtime_error, list.popFromFront());
    ASSERT_THROWS(std::runtime_error, list.popFromBack());
    list.pushAtBack(1);
    list.popFromFront();
    ASSERT_THROWS(std::runtime_error, list.popFromFront());
    ASSERT_THROWS(std::runtime_error, list.popFromBack());
}

TEST(pop_until_empty_then_reuse) {
    CircularArrayList list;
    for (int i = 0; i < 5; i++) list.pushAtFront(i);
    for (int i = 0; i < 5; i++) {
        if (i % 2 == 0) list.popFromFront();
        else            list.popFromBack();
    }
    ASSERT_TRUE(list.isEmpty());
    list.pushAtBack(42);
    list.pushAtFront(41);
    ASSERT_EQ(IntVec({41, 42}), contents(list));
}

/* ======================= toString ======================= */

TEST(toString_uses_logical_order) {
    CircularArrayList list;
    for (int i = 3; i <= 4; i++) list.pushAtBack(i * 10);
    list.pushAtFront(20);
    list.pushAtFront(10); /* wrapped inside capacity 6 */
    ASSERT_EQ("[10, 20, 30, 40]", list.toString());
}

TEST(toString_single) {
    CircularArrayList list;
    list.pushAtFront(-3);
    ASSERT_EQ("[-3]", list.toString());
}

/* ======================= memory ======================= */

TEST(memory_one_array_at_a_time_and_destructor_frees) {
    long before = liveArrayCount();
    {
        CircularArrayList list;
        for (int i = 0; i < 100; i++) {
            if (i % 3 == 0) list.pushAtFront(i);
            else            list.pushAtBack(i);
            ASSERT_EQ_MSG(before + 1, liveArrayCount(),
                          "did expand delete [] the old array?");
        }
    }
    ASSERT_EQ_MSG(before, liveArrayCount(), "destructor must delete [] data");
}

/* ======================= performance ======================= */

TEST(performance_200000_pushAtFront_and_popFromFront) {
    CircularArrayList list;
    for (int i = 0; i < 200000; i++) list.pushAtFront(i);
    ASSERT_EQ(199999, list.first());
    ASSERT_EQ(0, list.last());
    for (int i = 0; i < 199999; i++) list.popFromFront();
    ASSERT_EQ(1, list.size());
    ASSERT_EQ(0, list.first());
}

/* ======================= randomized ======================= */

TEST(stress_random_operations_match_model) {
    std::srand(4);
    CircularArrayList list;
    Model model;
    for (int step = 0; step < 5000; step++) {
        int op = std::rand() % 6;
        int value = std::rand() % 1000;
        bool empty = model.items.empty();
        if (op == 0 || op == 1)    { list.pushAtBack(value);  model.pushBack(value); }
        else if (op == 2)          { list.pushAtFront(value); model.pushFront(value); }
        else if (op == 3 && !empty) { list.popFromFront();    model.popFront(); }
        else if (op == 4 && !empty) { list.popFromBack();     model.popBack(); }
        else if (op == 5 && !empty) {
            int idx = std::rand() % static_cast<int>(model.items.size());
            list.replaceAt(value, idx);
            model.items[idx] = value;
        }
        std::string ctx = "step " + std::to_string(step);
        ASSERT_EQ_MSG(static_cast<int>(model.items.size()), list.size(), ctx);
        ASSERT_EQ_MSG(model.cap, list.capacity(), ctx);
        ASSERT_EQ_MSG(model.front, list.frontIndex(), ctx);
        if (step % 25 == 0) ASSERT_EQ_MSG(model.vec(), contents(list), ctx);
    }
    ASSERT_EQ(model.vec(), contents(list));
}
