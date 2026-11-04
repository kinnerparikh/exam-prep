#include "printSubsets.h"
#include "test_framework.h"

#include <algorithm>
#include <sstream>
#include <vector>

using Lines = std::vector<std::string>;

static void checkSubsets(const std::vector<int> &values) {
    std::ostringstream out;
    long live = testing::liveArrayCount();
    printSubsets(values.data(), static_cast<int>(values.size()), out);
    ASSERT_EQ(live, testing::liveArrayCount());
    std::string output = out.str();
    ASSERT_TRUE(!output.empty() && output.back() == '\n');

    Lines actual;
    std::istringstream input(output);
    std::string line;
    while (std::getline(input, line)) actual.push_back(line);

    Lines expected;
    for (unsigned mask = 0; mask < (1U << values.size()); mask++) {
        std::ostringstream subset;
        subset << '{';
        bool first = true;
        for (std::size_t i = 0; i < values.size(); i++) {
            if ((mask & (1U << i)) != 0) {
                if (!first) subset << ", ";
                subset << values[i];
                first = false;
            }
        }
        subset << '}';
        expected.push_back(subset.str());
    }
    std::sort(expected.begin(), expected.end());
    std::sort(actual.begin(), actual.end());
    ASSERT_EQ(expected, actual);
}

TEST(empty_input_prints_one_empty_subset) {
    std::ostringstream out;
    printSubsets(nullptr, 0, out);
    ASSERT_EQ(std::string("{}\n"), out.str());
}

TEST(single_element) {
    checkSubsets({7});
}

TEST(example_and_original_order_with_negative_values) {
    checkSubsets({1, 2});
    checkSubsets({9, -2, 0});
}

TEST(all_subsets_once_without_buffer_leaks) {
    checkSubsets({8, 3, -1, 5, 0, 12, -9, 2});
}

TEST(append_output_and_restore_state_between_calls) {
    int values[] = {4, 1};
    std::ostringstream first;
    printSubsets(values, 2, first);
    ASSERT_EQ(4, values[0]);
    ASSERT_EQ(1, values[1]);
    std::ostringstream second;
    second << "prefix\n";
    printSubsets(nullptr, 0, second);
    ASSERT_EQ(std::string("prefix\n{}\n"), second.str());
    checkSubsets({6, 2, 10});
}
