/*
 * IntLinkedList_tests.cpp -- tests for PROBLEM L1.
 *
 * Run all:   make ll1
 * Run some:  ./build/ll1 insertAt pop
 *
 * Most tests check your list three ways: size(), toString() (follows next
 * pointers) and toReverseString() (follows prev pointers). If toString is
 * right but toReverseString is wrong, a prev pointer (or `back`) is broken.
 */

#include "IntLinkedList.h"
#include "test_framework.h"

#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

using IntVec = std::vector<int>;

static std::string bracket(const IntVec &v) {
    std::string s = "[";
    for (size_t i = 0; i < v.size(); i++) {
        if (i > 0) s += ", ";
        s += std::to_string(v[i]);
    }
    return s + "]";
}

static IntVec reversed(IntVec v) {
    std::reverse(v.begin(), v.end());
    return v;
}

static int nodes() { return IntLinkedList::liveNodeCount(); }

static IntLinkedList *makeList(const IntVec &v) {
    IntLinkedList *list = new IntLinkedList();
    for (int x : v) list->pushAtBack(x);
    return list;
}

/* Checks size, next pointers, prev pointers, and first/last (front/back). */
#define ASSERT_LIST(expected, list)                                           \
    do {                                                                      \
        const IntVec &e__ = (expected);                                       \
        ASSERT_EQ_MSG(static_cast<int>(e__.size()), (list).size(), "size()"); \
        ASSERT_EQ_MSG(bracket(e__), (list).toString(),                        \
                      "toString() -- follows next pointers from front");     \
        ASSERT_EQ_MSG(bracket(reversed(e__)), (list).toReverseString(),      \
                      "toReverseString() -- follows prev pointers from back"); \
        if (!e__.empty()) {                                                   \
            ASSERT_EQ_MSG(e__.front(), (list).first(), "first()");            \
            ASSERT_EQ_MSG(e__.back(), (list).last(), "last()");               \
        }                                                                     \
    } while (0)

/* ======================= constructors ======================= */

TEST(default_constructor_creates_empty_list) {
    int before = nodes();
    IntLinkedList list;
    ASSERT_EQ(0, list.size());
    ASSERT_TRUE(list.isEmpty());
    ASSERT_EQ("[]", list.toString());
    ASSERT_EQ("[]", list.toReverseString());
    ASSERT_EQ(before, nodes());
}

TEST(array_constructor) {
    int before = nodes();
    int arr[] = {3, 1, 4, 1, 5};
    IntLinkedList list(arr, 5);
    ASSERT_LIST(IntVec({3, 1, 4, 1, 5}), list);
    ASSERT_EQ(before + 5, nodes());
}

TEST(array_constructor_single_element) {
    int arr[] = {9};
    IntLinkedList list(arr, 1);
    ASSERT_LIST(IntVec({9}), list);
}

TEST(array_constructor_length_zero) {
    int arr[] = {1};
    IntLinkedList list(arr, 0);
    ASSERT_LIST(IntVec(), list);
    ASSERT_TRUE(list.isEmpty());
}

TEST(isEmpty_changes_as_elements_added_and_removed) {
    IntLinkedList list;
    ASSERT_TRUE(list.isEmpty());
    list.pushAtFront(1);
    ASSERT_FALSE(list.isEmpty());
    list.popFromFront();
    ASSERT_TRUE(list.isEmpty());
}

/* ======================= push ======================= */

TEST(pushAtBack_into_empty_sets_front_and_back) {
    IntLinkedList list;
    list.pushAtBack(5);
    ASSERT_LIST(IntVec({5}), list);
}

TEST(pushAtFront_into_empty_sets_front_and_back) {
    IntLinkedList list;
    list.pushAtFront(5);
    ASSERT_LIST(IntVec({5}), list);
}

TEST(pushAtBack_many) {
    IntLinkedList list;
    IntVec expected;
    for (int i = 0; i < 100; i++) {
        list.pushAtBack(i);
        expected.push_back(i);
    }
    ASSERT_LIST(expected, list);
}

TEST(pushAtFront_many) {
    IntLinkedList list;
    IntVec expected;
    for (int i = 0; i < 100; i++) {
        list.pushAtFront(i);
        expected.insert(expected.begin(), i);
    }
    ASSERT_LIST(expected, list);
}

TEST(mixed_pushes) {
    IntLinkedList list;
    list.pushAtBack(3);
    list.pushAtFront(2);
    list.pushAtBack(4);
    list.pushAtFront(1);
    ASSERT_LIST(IntVec({1, 2, 3, 4}), list);
}

TEST(toReverseString_basic) {
    IntLinkedList list;
    list.pushAtBack(1);
    list.pushAtBack(2);
    list.pushAtBack(3);
    ASSERT_EQ("[3, 2, 1]", list.toReverseString());
}

/* ======================= access ======================= */

TEST(first_and_last_on_empty_throw_runtime_error) {
    IntLinkedList list;
    ASSERT_THROWS(std::runtime_error, list.first());
    ASSERT_THROWS(std::runtime_error, list.last());
}

TEST(elementAt_every_index) {
    IntLinkedList *list = makeList({10, 20, 30, 40});
    ASSERT_EQ(10, list->elementAt(0));
    ASSERT_EQ(20, list->elementAt(1));
    ASSERT_EQ(30, list->elementAt(2));
    ASSERT_EQ(40, list->elementAt(3));
    delete list;
}

TEST(elementAt_out_of_range_throws_range_error) {
    IntLinkedList *list = makeList({1, 2, 3});
    ASSERT_THROWS(std::range_error, list->elementAt(-1));
    ASSERT_THROWS(std::range_error, list->elementAt(3));
    ASSERT_THROWS(std::range_error, list->elementAt(50));
    delete list;
    IntLinkedList empty;
    ASSERT_THROWS(std::range_error, empty.elementAt(0));
}

TEST(replaceAt_changes_value_without_new_nodes) {
    IntLinkedList *list = makeList({1, 2, 3});
    int before = nodes();
    list->replaceAt(10, 0);
    list->replaceAt(20, 1);
    list->replaceAt(30, 2);
    ASSERT_LIST(IntVec({10, 20, 30}), *list);
    ASSERT_EQ(before, nodes());
    delete list;
}

TEST(replaceAt_out_of_range_throws_range_error) {
    IntLinkedList *list = makeList({1, 2});
    ASSERT_THROWS(std::range_error, list->replaceAt(0, -1));
    ASSERT_THROWS(std::range_error, list->replaceAt(0, 2));
    ASSERT_LIST(IntVec({1, 2}), *list);
    delete list;
}

TEST(indexOf_first_occurrence_and_missing) {
    IntLinkedList *list = makeList({5, 3, 5, 3, 9});
    ASSERT_EQ(0, list->indexOf(5));
    ASSERT_EQ(1, list->indexOf(3));
    ASSERT_EQ(4, list->indexOf(9));
    ASSERT_EQ(-1, list->indexOf(7));
    delete list;
    IntLinkedList empty;
    ASSERT_EQ(-1, empty.indexOf(0));
}

/* ======================= insertAt ======================= */

TEST(insertAt_zero_into_empty) {
    IntLinkedList list;
    list.insertAt(7, 0);
    ASSERT_LIST(IntVec({7}), list);
}

TEST(insertAt_front_updates_front) {
    IntLinkedList *list = makeList({2, 3});
    list->insertAt(1, 0);
    ASSERT_LIST(IntVec({1, 2, 3}), *list);
    delete list;
}

TEST(insertAt_size_appends_and_updates_back) {
    IntLinkedList *list = makeList({1, 2});
    list->insertAt(3, 2);
    ASSERT_LIST(IntVec({1, 2, 3}), *list);
    delete list;
}

TEST(insertAt_middle_links_both_directions) {
    IntLinkedList *list = makeList({1, 2, 4, 5});
    list->insertAt(3, 2);
    ASSERT_LIST(IntVec({1, 2, 3, 4, 5}), *list);
    list->insertAt(0, 1);
    ASSERT_LIST(IntVec({1, 0, 2, 3, 4, 5}), *list);
    list->insertAt(9, 5);
    ASSERT_LIST(IntVec({1, 0, 2, 3, 4, 9, 5}), *list);
    delete list;
}

TEST(insertAt_out_of_range_throws_and_does_not_leak) {
    IntLinkedList *list = makeList({1, 2, 3});
    int before = nodes();
    ASSERT_THROWS(std::range_error, list->insertAt(0, -1));
    ASSERT_THROWS(std::range_error, list->insertAt(0, 4));
    ASSERT_LIST(IntVec({1, 2, 3}), *list);
    ASSERT_EQ_MSG(before, nodes(), "check the index BEFORE calling new");
    delete list;
    IntLinkedList empty;
    ASSERT_THROWS(std::range_error, empty.insertAt(1, 1));
}

/* ======================= insertInOrder ======================= */

TEST(insertInOrder_into_empty) {
    IntLinkedList list;
    list.insertInOrder(5);
    ASSERT_LIST(IntVec({5}), list);
}

TEST(insertInOrder_smallest_goes_to_front) {
    IntLinkedList *list = makeList({3, 5, 7});
    list->insertInOrder(1);
    ASSERT_LIST(IntVec({1, 3, 5, 7}), *list);
    delete list;
}

TEST(insertInOrder_largest_goes_to_back) {
    IntLinkedList *list = makeList({3, 5, 7});
    list->insertInOrder(10);
    ASSERT_LIST(IntVec({3, 5, 7, 10}), *list);
    delete list;
}

TEST(insertInOrder_middle) {
    IntLinkedList *list = makeList({3, 5, 7});
    list->insertInOrder(6);
    ASSERT_LIST(IntVec({3, 5, 6, 7}), *list);
    list->insertInOrder(4);
    ASSERT_LIST(IntVec({3, 4, 5, 6, 7}), *list);
    delete list;
}

TEST(insertInOrder_duplicates) {
    IntLinkedList *list = makeList({1, 3, 3, 5});
    list->insertInOrder(3);
    list->insertInOrder(1);
    list->insertInOrder(5);
    ASSERT_LIST(IntVec({1, 1, 3, 3, 3, 5, 5}), *list);
    delete list;
}

TEST(insertInOrder_builds_sorted_list) {
    std::srand(1);
    IntLinkedList list;
    IntVec expected;
    for (int i = 0; i < 200; i++) {
        int v = std::rand() % 100 - 50;
        list.insertInOrder(v);
        expected.push_back(v);
    }
    std::sort(expected.begin(), expected.end());
    ASSERT_LIST(expected, list);
}

/* ======================= pop / removeAt ======================= */

TEST(popFromFront_updates_front) {
    IntLinkedList *list = makeList({1, 2, 3});
    list->popFromFront();
    ASSERT_LIST(IntVec({2, 3}), *list);
    delete list;
}

TEST(popFromBack_updates_back) {
    IntLinkedList *list = makeList({1, 2, 3});
    list->popFromBack();
    ASSERT_LIST(IntVec({1, 2}), *list);
    list->pushAtBack(9);
    ASSERT_LIST(IntVec({1, 2, 9}), *list);
    delete list;
}

TEST(popFromFront_last_node_resets_front_and_back) {
    IntLinkedList list;
    list.pushAtBack(1);
    list.popFromFront();
    ASSERT_LIST(IntVec(), list);
    list.pushAtBack(2);
    ASSERT_LIST(IntVec({2}), list);
    list.pushAtFront(1);
    ASSERT_LIST(IntVec({1, 2}), list);
}

TEST(popFromBack_last_node_resets_front_and_back) {
    IntLinkedList list;
    list.pushAtFront(1);
    list.popFromBack();
    ASSERT_LIST(IntVec(), list);
    list.pushAtFront(2);
    ASSERT_LIST(IntVec({2}), list);
    list.pushAtBack(3);
    ASSERT_LIST(IntVec({2, 3}), list);
}

TEST(pop_on_empty_throws_runtime_error) {
    IntLinkedList list;
    ASSERT_THROWS(std::runtime_error, list.popFromFront());
    ASSERT_THROWS(std::runtime_error, list.popFromBack());
}

TEST(removeAt_front) {
    IntLinkedList *list = makeList({1, 2, 3});
    list->removeAt(0);
    ASSERT_LIST(IntVec({2, 3}), *list);
    delete list;
}

TEST(removeAt_back) {
    IntLinkedList *list = makeList({1, 2, 3});
    list->removeAt(2);
    ASSERT_LIST(IntVec({1, 2}), *list);
    delete list;
}

TEST(removeAt_middle_relinks_both_directions) {
    IntLinkedList *list = makeList({1, 2, 3, 4, 5});
    list->removeAt(2);
    ASSERT_LIST(IntVec({1, 2, 4, 5}), *list);
    list->removeAt(1);
    ASSERT_LIST(IntVec({1, 4, 5}), *list);
    delete list;
}

TEST(removeAt_only_node) {
    IntLinkedList *list = makeList({1});
    list->removeAt(0);
    ASSERT_LIST(IntVec(), *list);
    list->pushAtBack(5);
    ASSERT_LIST(IntVec({5}), *list);
    delete list;
}

TEST(removeAt_out_of_range_throws_range_error) {
    IntLinkedList *list = makeList({1, 2, 3});
    ASSERT_THROWS(std::range_error, list->removeAt(-1));
    ASSERT_THROWS(std::range_error, list->removeAt(3));
    ASSERT_LIST(IntVec({1, 2, 3}), *list);
    delete list;
    IntLinkedList empty;
    ASSERT_THROWS(std::range_error, empty.removeAt(0));
}

/* ======================= clear ======================= */

TEST(clear_empties_list_and_deletes_nodes) {
    int before = nodes();
    IntLinkedList *list = makeList({1, 2, 3, 4});
    list->clear();
    ASSERT_LIST(IntVec(), *list);
    ASSERT_TRUE(list->isEmpty());
    ASSERT_EQ_MSG(before, nodes(), "clear() must delete every node");
    list->pushAtBack(7);
    list->pushAtFront(6);
    ASSERT_LIST(IntVec({6, 7}), *list);
    delete list;
}

TEST(clear_on_empty_is_fine) {
    IntLinkedList list;
    ASSERT_NO_THROW(list.clear());
    ASSERT_LIST(IntVec(), list);
}

/* ======================= copy constructor ======================= */

TEST(copy_constructor_copies_contents) {
    IntLinkedList *original = makeList({1, 2, 3});
    IntLinkedList copy(*original);
    ASSERT_LIST(IntVec({1, 2, 3}), copy);
    ASSERT_LIST(IntVec({1, 2, 3}), *original);
    delete original;
}

TEST(copy_constructor_makes_new_nodes) {
    IntLinkedList *original = makeList({1, 2, 3});
    int before = nodes();
    IntLinkedList copy(*original);
    ASSERT_EQ_MSG(before + 3, nodes(), "a deep copy needs its own nodes");
    copy.replaceAt(100, 1);
    copy.popFromFront();
    ASSERT_LIST(IntVec({1, 2, 3}), *original);
    ASSERT_LIST(IntVec({100, 3}), copy);
    delete original;
    ASSERT_LIST(IntVec({100, 3}), copy); /* copy survives the original */
}

TEST(copy_constructor_of_empty) {
    IntLinkedList original;
    IntLinkedList copy(original);
    ASSERT_LIST(IntVec(), copy);
    copy.pushAtBack(1);
    ASSERT_TRUE(original.isEmpty());
}

/* ======================= assignment ======================= */

TEST(assignment_copies_contents) {
    IntLinkedList *a = makeList({1, 2, 3});
    IntLinkedList *b = makeList({9, 8});
    *b = *a;
    ASSERT_LIST(IntVec({1, 2, 3}), *b);
    ASSERT_LIST(IntVec({1, 2, 3}), *a);
    delete a;
    delete b;
}

TEST(assignment_is_deep) {
    IntLinkedList *a = makeList({1, 2, 3});
    IntLinkedList b;
    b = *a;
    a->replaceAt(50, 0);
    b.pushAtBack(4);
    ASSERT_LIST(IntVec({50, 2, 3}), *a);
    ASSERT_LIST(IntVec({1, 2, 3, 4}), b);
    delete a;
    ASSERT_LIST(IntVec({1, 2, 3, 4}), b);
}

TEST(assignment_from_empty) {
    IntLinkedList empty;
    IntLinkedList *b = makeList({1, 2});
    *b = empty;
    ASSERT_LIST(IntVec(), *b);
    b->pushAtBack(3);
    ASSERT_LIST(IntVec({3}), *b);
    delete b;
}

TEST(self_assignment_is_safe) {
    IntLinkedList *list = makeList({1, 2, 3});
    int before = nodes();
    IntLinkedList &alias = *list;
    *list = alias;
    ASSERT_LIST(IntVec({1, 2, 3}), *list);
    ASSERT_EQ(before, nodes());
    delete list;
}

TEST(assignment_returns_reference_and_chains) {
    IntLinkedList a, b;
    IntLinkedList *c = makeList({4, 5});
    a = b = *c;
    ASSERT_LIST(IntVec({4, 5}), a);
    ASSERT_LIST(IntVec({4, 5}), b);
    (a = *c).pushAtFront(3);
    ASSERT_LIST(IntVec({3, 4, 5}), a);
    delete c;
}

/* ======================= memory ======================= */

TEST(memory_destructor_deletes_every_node) {
    int before = nodes();
    IntLinkedList *list = makeList({1, 2, 3, 4, 5});
    delete list;
    ASSERT_EQ_MSG(before, nodes(), "nodes leaked by the destructor");
    int arr[] = {1, 2, 3};
    {
        IntLinkedList fromArray(arr, 3);
    }
    ASSERT_EQ(before, nodes());
}

TEST(memory_every_removal_deletes_its_node) {
    int before = nodes();
    IntLinkedList *list = makeList({1, 2, 3, 4, 5, 6, 7, 8});
    list->popFromFront();
    ASSERT_EQ_MSG(before + 7, nodes(), "popFromFront");
    list->popFromBack();
    ASSERT_EQ_MSG(before + 6, nodes(), "popFromBack");
    list->removeAt(2);
    ASSERT_EQ_MSG(before + 5, nodes(), "removeAt(middle)");
    list->removeAt(0);
    ASSERT_EQ_MSG(before + 4, nodes(), "removeAt(0)");
    list->removeAt(list->size() - 1);
    ASSERT_EQ_MSG(before + 3, nodes(), "removeAt(last)");
    delete list;
    ASSERT_EQ(before, nodes());
}

TEST(memory_assignment_deletes_old_nodes) {
    int before = nodes();
    IntLinkedList *a = makeList({1, 2});
    IntLinkedList *b = makeList({5, 6, 7, 8, 9});
    *b = *a;
    ASSERT_EQ_MSG(before + 4, nodes(), "operator= must delete b's old nodes");
    delete a;
    delete b;
    ASSERT_EQ(before, nodes());
}

/* ======================= randomized ======================= */

TEST(stress_random_operations_match_std_vector) {
    std::srand(15);
    int base = nodes();
    IntLinkedList list;
    IntVec model;
    for (int step = 0; step < 3000; step++) {
        int op = std::rand() % 9;
        int value = std::rand() % 200 - 100;
        int n = static_cast<int>(model.size());
        if (op == 0)      { list.pushAtBack(value);  model.push_back(value); }
        else if (op == 1) { list.pushAtFront(value); model.insert(model.begin(), value); }
        else if (op == 2) {
            int idx = std::rand() % (n + 1);
            list.insertAt(value, idx);
            model.insert(model.begin() + idx, value);
        } else if (op == 3 && n > 0) {
            int idx = std::rand() % n;
            list.removeAt(idx);
            model.erase(model.begin() + idx);
        } else if (op == 4 && n > 0) { list.popFromFront(); model.erase(model.begin()); }
        else if (op == 5 && n > 0)   { list.popFromBack();  model.pop_back(); }
        else if (op == 6 && n > 0) {
            int idx = std::rand() % n;
            list.replaceAt(value, idx);
            model[idx] = value;
        } else if (op == 7 && n > 0) {
            int idx = std::rand() % n;
            ASSERT_EQ(model[idx], list.elementAt(idx));
        } else if (op == 8 && n > 40) {
            list.clear();
            model.clear();
        }
        std::string ctx = "step " + std::to_string(step);
        ASSERT_EQ_MSG(static_cast<int>(model.size()), list.size(), ctx);
        ASSERT_EQ_MSG(base + static_cast<int>(model.size()), nodes(),
                      ctx + ": node count should equal size()");
        if (step % 20 == 0) {
            ASSERT_EQ_MSG(bracket(model), list.toString(), ctx);
            ASSERT_EQ_MSG(bracket(reversed(model)), list.toReverseString(), ctx);
        }
    }
    ASSERT_LIST(model, list);
}
