#include "sumArray.h"
#include "test_framework.h"

#include <climits>

TEST(empty_array) {
    ASSERT_EQ(0LL, sumArray(nullptr, 0));
}

TEST(single_element) {
    int values[] = {-8};
    ASSERT_EQ(-8LL, sumArray(values, 1));
}

TEST(mixed_values_and_zeroes) {
    int values[] = {4, -7, 0, 9, -2};
    ASSERT_EQ(4LL, sumArray(values, 5));
}

TEST(only_requested_prefix_is_used) {
    int values[] = {3, 5, 100};
    ASSERT_EQ(8LL, sumArray(values, 2));
    ASSERT_EQ(3, values[0]);
    ASSERT_EQ(5, values[1]);
    ASSERT_EQ(100, values[2]);
}

TEST(sum_can_exceed_int_range) {
    int values[] = {INT_MAX, INT_MAX, -1};
    ASSERT_EQ(2LL * INT_MAX - 1, sumArray(values, 3));
}

TEST(longer_array_matches_running_sum) {
    int values[300];
    long long expected = 0;
    for (int i = 0; i < 300; i++) {
        values[i] = i % 11 - 5;
        expected += values[i];
    }
    ASSERT_EQ(expected, sumArray(values, 300));
}
