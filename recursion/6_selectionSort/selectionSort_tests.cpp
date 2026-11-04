#include "selectionSort.h"
#include "test_framework.h"

#include <algorithm>
#include <vector>

using IntVec = std::vector<int>;

TEST(empty_and_single_element) {
    selectionSort(nullptr, 0);
    int values[] = {7};
    selectionSort(values, 1);
    ASSERT_EQ(7, values[0]);
}

TEST(reverse_sorted) {
    int values[] = {5, 4, 3, 2, 1};
    selectionSort(values, 5);
    ASSERT_EQ(IntVec({1, 2, 3, 4, 5}), IntVec(values, values + 5));
}

TEST(duplicates_negative_values_and_zero) {
    int values[] = {3, -2, 3, 0, -2, 1};
    selectionSort(values, 6);
    ASSERT_EQ(IntVec({-2, -2, 0, 1, 3, 3}), IntVec(values, values + 6));
}

TEST(already_sorted_and_all_equal) {
    int sorted[] = {-4, 0, 2, 8};
    selectionSort(sorted, 4);
    ASSERT_EQ(IntVec({-4, 0, 2, 8}), IntVec(sorted, sorted + 4));
    int equal[] = {6, 6, 6, 6};
    selectionSort(equal, 4);
    ASSERT_EQ(IntVec({6, 6, 6, 6}), IntVec(equal, equal + 4));
}

TEST(subrange_does_not_touch_neighbors_or_allocate_arrays) {
    int values[] = {99, 4, 1, 3, 88};
    long before = testing::totalArrayAllocations();
    selectionSort(values + 1, 3);
    ASSERT_EQ(before, testing::totalArrayAllocations());
    ASSERT_EQ(IntVec({99, 1, 3, 4, 88}), IntVec(values, values + 5));
}

TEST(varied_inputs_match_standard_sort) {
    for (int n : {2, 3, 16, 51, 120}) {
        IntVec values(n);
        for (int i = 0; i < n; i++) values[i] = (i * 37 + n) % 29 - 14;
        IntVec expected = values;
        std::sort(expected.begin(), expected.end());
        selectionSort(values.data(), n);
        ASSERT_EQ(expected, values);
    }
}
