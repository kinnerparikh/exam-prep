/*
 * IntArrayList_tests.cpp -- tests for PROBLEM A1.
 *
 * Run all:     make al1
 * Run some:    ./build/al1 pushAtFront copy
 */

#include "IntArrayList.h"
#include "test_framework.h"

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

using IntVec = std::vector<int>;
using testing::liveArrayCount;
using testing::totalArrayAllocations;

/* Reads the list through size() + elementAt() (independent of toString). */
static IntVec contents(const IntArrayList &list) {
    IntVec out;
    for (int i = 0; i < list.size(); i++) out.push_back(list.elementAt(i));
    return out;
}

static std::string bracket(const IntVec &v) {
    std::string s = "[";
    for (size_t i = 0; i < v.size(); i++) {
        if (i > 0) s += ", ";
        s += std::to_string(v[i]);
    }
    return s + "]";
}

static IntArrayList makeList(const IntVec &v) {
    IntArrayList list;
    for (int x : v) list.pushAtBack(x);
    return list;
}

/* ======================= constructors ======================= */

TEST(default_constructor_creates_empty_list) {
    IntArrayList list;
    ASSERT_EQ(0, list.size());
    ASSERT_TRUE(list.isEmpty());
    ASSERT_EQ(0, list.capacity());
    ASSERT_EQ("[]", list.toString());
}

TEST(default_constructor_allocates_nothing) {
    long before = totalArrayAllocations();
    IntArrayList list;
    ASSERT_EQ(before, totalArrayAllocations());
}

TEST(array_constructor_copies_elements) {
    int arr[] = {3, 1, 4, 1, 5};
    IntArrayList list(arr, 5);
    ASSERT_EQ(5, list.size());
    ASSERT_FALSE(list.isEmpty());
    ASSERT_EQ(5, list.capacity());
    ASSERT_EQ(IntVec({3, 1, 4, 1, 5}), contents(list));
}

TEST(array_constructor_makes_independent_copy) {
    int arr[] = {7, 8, 9};
    IntArrayList list(arr, 3);
    arr[0] = 100;
    ASSERT_EQ(7, list.elementAt(0));
}

TEST(array_constructor_partial_length) {
    int arr[] = {1, 2, 3, 4, 5};
    IntArrayList list(arr, 2);
    ASSERT_EQ(IntVec({1, 2}), contents(list));
    ASSERT_EQ(2, list.capacity());
}

TEST(array_constructor_length_zero) {
    int arr[] = {1};
    IntArrayList list(arr, 0);
    ASSERT_EQ(0, list.size());
    ASSERT_TRUE(list.isEmpty());
    ASSERT_EQ(0, list.capacity());
}

/* ======================= isEmpty / size ======================= */

TEST(isEmpty_changes_as_elements_added_and_removed) {
    IntArrayList list;
    ASSERT_TRUE(list.isEmpty());
    list.pushAtBack(1);
    ASSERT_FALSE(list.isEmpty());
    list.popFromBack();
    ASSERT_TRUE(list.isEmpty());
}

/* ======================= pushAtBack / growth ======================= */

TEST(pushAtBack_single_into_empty) {
    IntArrayList list;
    list.pushAtBack(42);
    ASSERT_EQ(1, list.size());
    ASSERT_EQ(IntVec({42}), contents(list));
}

TEST(pushAtBack_many_preserves_order) {
    IntArrayList list;
    IntVec expected;
    for (int i = 0; i < 1000; i++) {
        list.pushAtBack(i * 3 - 7);
        expected.push_back(i * 3 - 7);
    }
    ASSERT_EQ(1000, list.size());
    ASSERT_EQ(expected, contents(list));
}

TEST(capacity_grows_as_2n_plus_2) {
    IntArrayList list;
    IntVec expectedCaps = {2, 2, 6, 6, 6, 6, 14, 14, 14, 14, 14, 14, 14, 14,
                           30};
    for (size_t i = 0; i < expectedCaps.size(); i++) {
        list.pushAtBack(static_cast<int>(i));
        ASSERT_EQ_MSG(expectedCaps[i], list.capacity(),
                      "after " + std::to_string(i + 1) + " pushAtBack calls");
    }
}

TEST(capacity_always_at_least_size) {
    IntArrayList list;
    for (int i = 0; i < 500; i++) {
        list.pushAtBack(i);
        ASSERT_TRUE(list.capacity() >= list.size());
    }
}

TEST(expand_after_array_constructor) {
    int arr[] = {1, 2, 3};
    IntArrayList list(arr, 3);
    list.pushAtBack(4);
    ASSERT_EQ(8, list.capacity()); /* 2 * 3 + 2 */
    ASSERT_EQ(IntVec({1, 2, 3, 4}), contents(list));
}

/* ======================= pushAtFront ======================= */

TEST(pushAtFront_into_empty) {
    IntArrayList list;
    list.pushAtFront(5);
    ASSERT_EQ(IntVec({5}), contents(list));
}

TEST(pushAtFront_reverses_insertion_order) {
    IntArrayList list;
    list.pushAtFront(1);
    list.pushAtFront(2);
    list.pushAtFront(3);
    ASSERT_EQ(IntVec({3, 2, 1}), contents(list));
}

TEST(pushAtFront_triggers_expand_correctly) {
    IntArrayList list;
    IntVec expected;
    for (int i = 0; i < 20; i++) {
        list.pushAtFront(i);
        expected.insert(expected.begin(), i);
    }
    ASSERT_EQ(expected, contents(list));
    ASSERT_EQ(30, list.capacity());
}

TEST(pushAtFront_and_pushAtBack_mixed) {
    IntArrayList list;
    list.pushAtBack(2);
    list.pushAtFront(1);
    list.pushAtBack(3);
    list.pushAtFront(0);
    ASSERT_EQ(IntVec({0, 1, 2, 3}), contents(list));
}

/* ======================= insertAt ======================= */

TEST(insertAt_zero_into_empty) {
    IntArrayList list;
    list.insertAt(9, 0);
    ASSERT_EQ(IntVec({9}), contents(list));
}

TEST(insertAt_front_middle_back) {
    IntArrayList list = makeList({10, 20, 30});
    list.insertAt(5, 0);       /* front */
    ASSERT_EQ(IntVec({5, 10, 20, 30}), contents(list));
    list.insertAt(15, 2);      /* middle */
    ASSERT_EQ(IntVec({5, 10, 15, 20, 30}), contents(list));
    list.insertAt(35, 5);      /* index == size -> append */
    ASSERT_EQ(IntVec({5, 10, 15, 20, 30, 35}), contents(list));
}

TEST(insertAt_last_valid_index_shifts_last_element) {
    IntArrayList list = makeList({1, 2, 3});
    list.insertAt(99, 2);
    ASSERT_EQ(IntVec({1, 2, 99, 3}), contents(list));
}

TEST(insertAt_when_full_expands) {
    int arr[] = {1, 2, 3, 4};
    IntArrayList list(arr, 4); /* full: size == capacity == 4 */
    list.insertAt(0, 2);
    ASSERT_EQ(IntVec({1, 2, 0, 3, 4}), contents(list));
    ASSERT_EQ(10, list.capacity());
}

TEST(insertAt_out_of_range_throws_range_error) {
    IntArrayList list = makeList({1, 2, 3});
    ASSERT_THROWS(std::range_error, list.insertAt(0, -1));
    ASSERT_THROWS(std::range_error, list.insertAt(0, 4));
    ASSERT_THROWS(std::range_error, list.insertAt(0, 1000));
}

TEST(insertAt_into_empty_nonzero_index_throws) {
    IntArrayList list;
    ASSERT_THROWS(std::range_error, list.insertAt(1, 1));
    ASSERT_EQ(0, list.size());
}

TEST(insertAt_failure_leaves_list_unchanged) {
    IntArrayList list = makeList({1, 2, 3});
    int capBefore = list.capacity();
    try { list.insertAt(7, 10); } catch (const std::exception &) {}
    ASSERT_EQ(IntVec({1, 2, 3}), contents(list));
    ASSERT_EQ(capBefore, list.capacity());
}

/* ======================= elementAt / first / last ======================= */

TEST(elementAt_every_index) {
    IntArrayList list = makeList({4, -8, 15, 16, 23, 42});
    ASSERT_EQ(4, list.elementAt(0));
    ASSERT_EQ(-8, list.elementAt(1));
    ASSERT_EQ(15, list.elementAt(2));
    ASSERT_EQ(42, list.elementAt(5));
}

TEST(elementAt_out_of_range_throws_range_error) {
    IntArrayList list = makeList({1, 2, 3});
    ASSERT_THROWS(std::range_error, list.elementAt(-1));
    ASSERT_THROWS(std::range_error, list.elementAt(3));
    ASSERT_THROWS(std::range_error, list.elementAt(100));
}

TEST(elementAt_on_empty_throws_range_error) {
    IntArrayList list;
    ASSERT_THROWS(std::range_error, list.elementAt(0));
}

TEST(elementAt_beyond_size_but_within_capacity_throws) {
    IntArrayList list = makeList({1, 2, 3}); /* capacity is 6 */
    ASSERT_THROWS(std::range_error, list.elementAt(4));
}

TEST(first_and_last) {
    IntArrayList list = makeList({7, 8, 9});
    ASSERT_EQ(7, list.first());
    ASSERT_EQ(9, list.last());
}

TEST(first_and_last_single_element) {
    IntArrayList list = makeList({5});
    ASSERT_EQ(5, list.first());
    ASSERT_EQ(5, list.last());
}

TEST(first_and_last_on_empty_throw_runtime_error) {
    IntArrayList list;
    ASSERT_THROWS(std::runtime_error, list.first());
    ASSERT_THROWS(std::runtime_error, list.last());
}

/* ======================= replaceAt / indexOf ======================= */

TEST(replaceAt_changes_only_target) {
    IntArrayList list = makeList({1, 2, 3, 4});
    list.replaceAt(20, 1);
    ASSERT_EQ(IntVec({1, 20, 3, 4}), contents(list));
    list.replaceAt(10, 0);
    list.replaceAt(40, 3);
    ASSERT_EQ(IntVec({10, 20, 3, 40}), contents(list));
    ASSERT_EQ(4, list.size());
}

TEST(replaceAt_out_of_range_throws_range_error) {
    IntArrayList list = makeList({1, 2});
    ASSERT_THROWS(std::range_error, list.replaceAt(0, -1));
    ASSERT_THROWS(std::range_error, list.replaceAt(0, 2));
    ASSERT_EQ(IntVec({1, 2}), contents(list));
}

TEST(indexOf_finds_first_occurrence) {
    IntArrayList list = makeList({5, 3, 5, 3});
    ASSERT_EQ(0, list.indexOf(5));
    ASSERT_EQ(1, list.indexOf(3));
}

TEST(indexOf_last_element) {
    IntArrayList list = makeList({1, 2, 3});
    ASSERT_EQ(2, list.indexOf(3));
}

TEST(indexOf_missing_returns_negative_one) {
    IntArrayList list = makeList({1, 2, 3});
    ASSERT_EQ(-1, list.indexOf(4));
    IntArrayList empty;
    ASSERT_EQ(-1, empty.indexOf(0));
}

TEST(indexOf_ignores_stale_values_past_size) {
    IntArrayList list = makeList({1, 2, 3});
    list.popFromBack(); /* 3 may still sit in the array -- it is not in the list */
    ASSERT_EQ(-1, list.indexOf(3));
}

/* ======================= removal ======================= */

TEST(removeAt_front_middle_back) {
    IntArrayList list = makeList({1, 2, 3, 4, 5});
    list.removeAt(0);
    ASSERT_EQ(IntVec({2, 3, 4, 5}), contents(list));
    list.removeAt(1);
    ASSERT_EQ(IntVec({2, 4, 5}), contents(list));
    list.removeAt(2);
    ASSERT_EQ(IntVec({2, 4}), contents(list));
}

TEST(removeAt_only_element) {
    IntArrayList list = makeList({1});
    list.removeAt(0);
    ASSERT_TRUE(list.isEmpty());
}

TEST(removeAt_out_of_range_throws_range_error) {
    IntArrayList list = makeList({1, 2, 3});
    ASSERT_THROWS(std::range_error, list.removeAt(-1));
    ASSERT_THROWS(std::range_error, list.removeAt(3));
    ASSERT_EQ(IntVec({1, 2, 3}), contents(list));
    IntArrayList empty;
    ASSERT_THROWS(std::range_error, empty.removeAt(0));
}

TEST(popFromFront_and_popFromBack) {
    IntArrayList list = makeList({1, 2, 3, 4});
    list.popFromFront();
    ASSERT_EQ(IntVec({2, 3, 4}), contents(list));
    list.popFromBack();
    ASSERT_EQ(IntVec({2, 3}), contents(list));
    list.popFromFront();
    list.popFromBack();
    ASSERT_TRUE(list.isEmpty());
}

TEST(pop_on_empty_throws_runtime_error) {
    IntArrayList list;
    ASSERT_THROWS(std::runtime_error, list.popFromFront());
    ASSERT_THROWS(std::runtime_error, list.popFromBack());
    ASSERT_EQ(0, list.size());
}

TEST(list_reusable_after_removing_everything) {
    IntArrayList list = makeList({1, 2, 3});
    list.popFromBack();
    list.popFromBack();
    list.popFromBack();
    list.pushAtBack(9);
    list.pushAtFront(8);
    ASSERT_EQ(IntVec({8, 9}), contents(list));
}

/* ======================= clear / shrinkToFit ======================= */

TEST(clear_empties_but_keeps_capacity) {
    IntArrayList list = makeList({1, 2, 3, 4, 5});
    int capBefore = list.capacity();
    list.clear();
    ASSERT_EQ(0, list.size());
    ASSERT_TRUE(list.isEmpty());
    ASSERT_EQ(capBefore, list.capacity());
    ASSERT_EQ("[]", list.toString());
}

TEST(clear_then_reuse) {
    IntArrayList list = makeList({1, 2, 3});
    list.clear();
    list.pushAtBack(7);
    ASSERT_EQ(IntVec({7}), contents(list));
}

TEST(clear_on_empty_is_fine) {
    IntArrayList list;
    ASSERT_NO_THROW(list.clear());
    ASSERT_EQ(0, list.size());
}

TEST(shrinkToFit_sets_capacity_to_size) {
    IntArrayList list = makeList({1, 2, 3, 4, 5, 6, 7}); /* capacity 14 */
    list.shrinkToFit();
    ASSERT_EQ(7, list.capacity());
    ASSERT_EQ(IntVec({1, 2, 3, 4, 5, 6, 7}), contents(list));
}

TEST(shrinkToFit_then_push_expands_from_new_capacity) {
    IntArrayList list = makeList({1, 2, 3}); /* capacity 6 */
    list.shrinkToFit();                    /* capacity 3 */
    list.pushAtBack(4);
    ASSERT_EQ(8, list.capacity());
    ASSERT_EQ(IntVec({1, 2, 3, 4}), contents(list));
}

TEST(shrinkToFit_on_empty_frees_array) {
    long before = liveArrayCount();
    {
        IntArrayList list = makeList({1, 2, 3});
        list.clear();
        list.shrinkToFit();
        ASSERT_EQ(0, list.capacity());
        ASSERT_EQ(before, liveArrayCount());
        list.pushAtBack(5); /* must still work afterwards */
        ASSERT_EQ(IntVec({5}), contents(list));
    }
}

/* ======================= toString ======================= */

TEST(toString_formats) {
    ASSERT_EQ("[7]", makeList({7}).toString());
    ASSERT_EQ("[1, 2, 3]", makeList({1, 2, 3}).toString());
    ASSERT_EQ("[-1, 0, 100]", makeList({-1, 0, 100}).toString());
}

/* ======================= copy constructor ======================= */

TEST(copy_constructor_copies_contents) {
    IntArrayList original = makeList({1, 2, 3});
    IntArrayList copy(original);
    ASSERT_EQ(IntVec({1, 2, 3}), contents(copy));
    ASSERT_EQ(IntVec({1, 2, 3}), contents(original));
}

TEST(copy_constructor_is_deep) {
    IntArrayList original = makeList({1, 2, 3});
    IntArrayList copy(original);
    copy.replaceAt(100, 0);
    copy.pushAtBack(4);
    original.replaceAt(-5, 2);
    ASSERT_EQ(IntVec({1, 2, -5}), contents(original));
    ASSERT_EQ(IntVec({100, 2, 3, 4}), contents(copy));
}

TEST(copy_constructor_of_empty_list) {
    IntArrayList original;
    IntArrayList copy(original);
    ASSERT_TRUE(copy.isEmpty());
    copy.pushAtBack(1);
    ASSERT_TRUE(original.isEmpty());
}

TEST(copy_constructor_capacity_holds_all_elements) {
    IntArrayList original = makeList({1, 2, 3, 4, 5});
    IntArrayList copy(original);
    ASSERT_TRUE(copy.capacity() >= copy.size());
}

/* ======================= assignment operator ======================= */

TEST(assignment_copies_contents) {
    IntArrayList a = makeList({1, 2, 3});
    IntArrayList b = makeList({9});
    b = a;
    ASSERT_EQ(IntVec({1, 2, 3}), contents(b));
}

TEST(assignment_is_deep) {
    IntArrayList a = makeList({1, 2, 3});
    IntArrayList b;
    b = a;
    b.pushAtFront(0);
    a.replaceAt(42, 1);
    ASSERT_EQ(IntVec({1, 42, 3}), contents(a));
    ASSERT_EQ(IntVec({0, 1, 2, 3}), contents(b));
}

TEST(assignment_from_empty_list) {
    IntArrayList a;
    IntArrayList b = makeList({1, 2, 3});
    b = a;
    ASSERT_TRUE(b.isEmpty());
    b.pushAtBack(5);
    ASSERT_EQ(IntVec({5}), contents(b));
}

TEST(assignment_larger_into_smaller) {
    IntArrayList big = makeList({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    IntArrayList small = makeList({1});
    small = big;
    ASSERT_EQ(contents(big), contents(small));
    ASSERT_TRUE(small.capacity() >= 10);
}

TEST(self_assignment_is_safe) {
    IntArrayList list = makeList({1, 2, 3});
    IntArrayList &alias = list;
    list = alias;
    ASSERT_EQ(IntVec({1, 2, 3}), contents(list));
    list.pushAtBack(4);
    ASSERT_EQ(IntVec({1, 2, 3, 4}), contents(list));
}

TEST(assignment_returns_reference_to_left_side) {
    IntArrayList a, b = makeList({1, 2});
    (a = b).pushAtBack(3);
    ASSERT_EQ(IntVec({1, 2, 3}), contents(a));
    ASSERT_EQ(IntVec({1, 2}), contents(b));
}

TEST(chained_assignment) {
    IntArrayList a, b, c = makeList({4, 5});
    a = b = c;
    ASSERT_EQ(IntVec({4, 5}), contents(a));
    ASSERT_EQ(IntVec({4, 5}), contents(b));
}

/* ======================= memory management ======================= */

TEST(memory_destructor_frees_array) {
    long before = liveArrayCount();
    {
        IntArrayList list = makeList({1, 2, 3, 4, 5});
    }
    ASSERT_EQ_MSG(before, liveArrayCount(),
                  "arrays still allocated after destructor ran");
}

TEST(memory_expand_frees_old_array) {
    long before = liveArrayCount();
    IntArrayList list;
    for (int i = 0; i < 100; i++) {
        list.pushAtBack(i);
        ASSERT_EQ_MSG(before + 1, liveArrayCount(),
                      "a single list should own exactly 1 array (did expand "
                      "delete [] the old one?) after " +
                          std::to_string(i + 1) + " pushes");
    }
}

TEST(memory_pushAtFront_and_insertAt_do_not_leak) {
    long before = liveArrayCount();
    {
        IntArrayList list;
        for (int i = 0; i < 50; i++) list.pushAtFront(i);
        for (int i = 0; i < 50; i++) list.insertAt(i, list.size() / 2);
        ASSERT_EQ(before + 1, liveArrayCount());
    }
    ASSERT_EQ(before, liveArrayCount());
}

TEST(memory_copy_constructor_allocates_one_array) {
    IntArrayList original = makeList({1, 2, 3});
    long before = liveArrayCount();
    {
        IntArrayList copy(original);
        ASSERT_EQ(before + 1, liveArrayCount());
    }
    ASSERT_EQ(before, liveArrayCount());
}

TEST(memory_assignment_frees_old_array) {
    IntArrayList a = makeList({1, 2, 3});
    IntArrayList b = makeList({4, 5, 6, 7, 8, 9, 10});
    long before = liveArrayCount();
    b = a;
    ASSERT_EQ_MSG(before, liveArrayCount(),
                  "operator= should free b's old array");
    IntArrayList &alias = b;
    b = alias;
    ASSERT_EQ_MSG(before, liveArrayCount(), "self-assignment changed memory");
}

TEST(memory_removals_clear_and_shrink_do_not_leak) {
    long before = liveArrayCount();
    {
        IntArrayList list = makeList({1, 2, 3, 4, 5, 6, 7, 8});
        list.removeAt(3);
        list.popFromFront();
        list.popFromBack();
        list.shrinkToFit();
        ASSERT_EQ(before + 1, liveArrayCount());
        list.clear();
        list.pushAtBack(1);
    }
    ASSERT_EQ(before, liveArrayCount());
}

/* ======================= big randomized test ======================= */

TEST(stress_random_operations_match_std_vector) {
    std::srand(15);
    IntArrayList list;
    IntVec model;
    for (int step = 0; step < 4000; step++) {
        int op = std::rand() % 8;
        int value = std::rand() % 1000 - 500;
        int n = static_cast<int>(model.size());
        std::string ctx = "step " + std::to_string(step);
        if (op <= 1) {
            list.pushAtBack(value);
            model.push_back(value);
        } else if (op == 2) {
            list.pushAtFront(value);
            model.insert(model.begin(), value);
        } else if (op == 3) {
            int idx = std::rand() % (n + 1);
            list.insertAt(value, idx);
            model.insert(model.begin() + idx, value);
        } else if (op == 4 && n > 0) {
            int idx = std::rand() % n;
            list.removeAt(idx);
            model.erase(model.begin() + idx);
        } else if (op == 5 && n > 0) {
            list.popFromFront();
            model.erase(model.begin());
        } else if (op == 6 && n > 0) {
            list.popFromBack();
            model.pop_back();
        } else if (op == 7 && n > 0) {
            int idx = std::rand() % n;
            list.replaceAt(value, idx);
            model[idx] = value;
        }
        ASSERT_EQ_MSG(static_cast<int>(model.size()), list.size(), ctx);
        if (step % 50 == 0) ASSERT_EQ_MSG(model, contents(list), ctx);
    }
    ASSERT_EQ(model, contents(list));
    ASSERT_EQ(bracket(model), list.toString());
}
