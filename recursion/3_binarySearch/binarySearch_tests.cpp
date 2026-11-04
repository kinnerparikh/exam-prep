#include "binarySearch.h"
#include "test_framework.h"

TEST(empty_array) {
    ASSERT_EQ(-1, binarySearch(nullptr, 0, 7));
}

TEST(single_element_hit_and_miss) {
    int values[] = {4};
    ASSERT_EQ(0, binarySearch(values, 1, 4));
    ASSERT_EQ(-1, binarySearch(values, 1, 3));
    ASSERT_EQ(-1, binarySearch(values, 1, 5));
}

TEST(every_index_in_odd_and_even_prefixes) {
    int values[] = {-9, -3, 0, 4, 8, 20};
    for (int n : {5, 6}) {
        for (int i = 0; i < n; i++) {
            ASSERT_EQ(i, binarySearch(values, n, values[i]));
        }
    }
    ASSERT_EQ(-1, binarySearch(values, 5, 20));
}

TEST(absent_below_between_and_above) {
    int values[] = {-5, 0, 2, 8};
    for (int target : {-6, -1, 1, 3, 9}) {
        ASSERT_EQ(-1, binarySearch(values, 4, target));
    }
}

TEST(any_matching_duplicate_index_is_valid) {
    int values[] = {1, 2, 2, 2, 3};
    int index = binarySearch(values, 5, 2);
    ASSERT_TRUE(index >= 1 && index <= 3);
    ASSERT_EQ(2, values[index]);
}

TEST(large_array_searches_both_ends) {
    int values[100001];
    for (int i = 0; i < 100001; i++) values[i] = 2 * i;
    ASSERT_EQ(0, binarySearch(values, 100001, 0));
    ASSERT_EQ(100000, binarySearch(values, 100001, 200000));
    ASSERT_EQ(-1, binarySearch(values, 100001, 199999));
}
