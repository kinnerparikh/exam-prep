#include "mergeSort.h"
#include "test_framework.h"

#include <algorithm>
#include <vector>

using IntVec = std::vector<int>;

TEST(empty_and_single_element_need_no_buffer) {
    long before = testing::totalArrayAllocations();
    mergeSort(nullptr, 0);
    int values[] = {8};
    mergeSort(values, 1);
    ASSERT_EQ(8, values[0]);
    ASSERT_EQ(before, testing::totalArrayAllocations());
}

TEST(odd_length_with_duplicates_and_negatives) {
    int values[] = {4, -1, 4, 0, -8, 2, 2};
    mergeSort(values, 7);
    ASSERT_EQ(IntVec({-8, -1, 0, 2, 2, 4, 4}), IntVec(values, values + 7));
}

TEST(sorted_reversed_and_all_equal) {
    for (IntVec values : {IntVec{-3, 0, 4, 9}, IntVec{9, 4, 0, -3},
                          IntVec{5, 5, 5, 5}}) {
        IntVec expected = values;
        std::sort(expected.begin(), expected.end());
        mergeSort(values.data(), static_cast<int>(values.size()));
        ASSERT_EQ(expected, values);
    }
}

TEST(subrange_keeps_neighbors_unchanged) {
    int values[] = {99, 8, 2, 5, 1, 88};
    mergeSort(values + 1, 4);
    ASSERT_EQ(IntVec({99, 1, 2, 5, 8, 88}), IntVec(values, values + 6));
}

TEST(one_shared_buffer_is_freed) {
    int values[] = {5, 2, 4, 1, 3};
    long live = testing::liveArrayCount();
    long total = testing::totalArrayAllocations();
    mergeSort(values, 5);
    ASSERT_EQ(IntVec({1, 2, 3, 4, 5}), IntVec(values, values + 5));
    ASSERT_EQ(total + 1, testing::totalArrayAllocations());
    ASSERT_EQ(live, testing::liveArrayCount());
}

TEST(varied_sizes_match_standard_sort) {
    for (int n : {2, 3, 31, 128, 257, 100000}) {
        IntVec values(n);
        for (int i = 0; i < n; i++) values[i] = (i * 37 + n) % 1009 - 504;
        IntVec expected = values;
        std::sort(expected.begin(), expected.end());
        mergeSort(values.data(), n);
        ASSERT_EQ(expected, values);
    }
}
