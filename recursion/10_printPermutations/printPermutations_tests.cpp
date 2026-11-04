#include "printPermutations.h"
#include "test_framework.h"

#include <algorithm>
#include <sstream>
#include <vector>

using Lines = std::vector<std::string>;

static void checkPermutations(const std::string &text) {
    std::string original = text;
    std::ostringstream out;
    long live = testing::liveArrayCount();
    printPermutations(text, out);
    ASSERT_EQ(original, text);
    ASSERT_EQ(live, testing::liveArrayCount());
    std::string output = out.str();
    ASSERT_TRUE(!output.empty() && output.back() == '\n');

    Lines actual;
    std::istringstream input(output);
    std::string line;
    while (std::getline(input, line)) actual.push_back(line);
    std::sort(actual.begin(), actual.end());

    Lines expected;
    std::string permutation = text;
    std::sort(permutation.begin(), permutation.end());
    do {
        expected.push_back(permutation);
    } while (std::next_permutation(permutation.begin(), permutation.end()));
    ASSERT_EQ(expected, actual);
}

TEST(empty_string_has_one_empty_permutation) {
    std::ostringstream out;
    printPermutations("", out);
    ASSERT_EQ(std::string("\n"), out.str());
}

TEST(single_and_two_characters) {
    checkPermutations("x");
    checkPermutations("ba");
}

TEST(all_choices_are_undone_before_next_branch) {
    checkPermutations("cab");
    checkPermutations("dcba");
}

TEST(case_and_spaces_are_literal_characters) {
    checkPermutations("a A");
}

TEST(larger_input_has_no_missing_or_duplicate_lines) {
    checkPermutations("gfedcba");
}

TEST(appends_to_stream_and_resets_between_calls) {
    std::ostringstream out;
    out << "prefix\n";
    printPermutations("z", out);
    printPermutations("", out);
    ASSERT_EQ(std::string("prefix\nz\n\n"), out.str());
}
