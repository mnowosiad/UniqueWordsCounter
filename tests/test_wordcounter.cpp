#include <gtest/gtest.h>
#include "../WordCounter.hpp"

TEST(ProcessChunkTest, ExtractsWordsCorrectly) {
    const std::string input = "hello world test example";
    std::unordered_set<std::string> result;

    std::span<const char> data(input.data(), input.size());
    counter_utils::ProcessChunk(data, 0, input.size(), result);

    EXPECT_EQ(result.size(), 4);
    EXPECT_TRUE(result.contains("hello"));
    EXPECT_TRUE(result.contains("world"));
    EXPECT_TRUE(result.contains("test"));
    EXPECT_TRUE(result.contains("example"));
}

TEST(ProcessChunkTest, ExtractsWordsCorrectlyHorseDog) {
    const std::string input = "a horse and a dog";
    std::unordered_set<std::string> result;

    std::span<const char> data(input.data(), input.size());
    counter_utils::ProcessChunk(data, 0, input.size(), result);

    EXPECT_EQ(result.size(), 4);
}

TEST(ProcessChunkTest, ExtractWordsCorrectlyBeginWithSpace)
{
    const std::string input = " apple banana  carrot apple  ";
    std::unordered_set<std::string> result;

    // Create a span from the input
    std::span<const char> data(input.data(), input.size());

    counter_utils::ProcessChunk(data, 0, input.size(), result);

    // Expected words (note: duplicates should be eliminated)
    std::unordered_set<std::string> expected = {"apple", "banana", "carrot"};

    EXPECT_EQ(result, expected);
}


TEST(ProcessChunkTest, HandlesMultipleSpaces) {
    const std::string input = "   one    two   three ";
    std::unordered_set<std::string> result;

    counter_utils::ProcessChunk(std::span<const char>(input.data(), input.size()), 0, input.size(), result);

    EXPECT_EQ(result.size(), 3);
    EXPECT_TRUE(result.contains("one"));
    EXPECT_TRUE(result.contains("two"));
    EXPECT_TRUE(result.contains("three"));
}

TEST(ProcessChunkTest, HandlesEmptyInput)
{
    const std::string input = "";
    std::unordered_set<std::string> result;

    counter_utils::ProcessChunk(std::span<const char>(input.data(), input.size()), 0, input.size(), result);

    EXPECT_TRUE(result.empty());
}

TEST(ProcessChunkTest, HandlesAllSpaces) {
    const std::string input = "     ";
    std::unordered_set<std::string> result;

    counter_utils::ProcessChunk(std::span<const char>(input.data(), input.size()), 0, input.size(), result);

    EXPECT_TRUE(result.empty());
}

TEST(ProcessChunkTest, RespectsChunkBoundaries) {
    const std::string input = "abc def ghi jkl";
    std::unordered_set<std::string> result;

    // This chunk starts in the middle of a word ("def"), so it should not extract any word
    size_t start = 4;  // 'd'
    size_t end = 7;    // 'f'
    counter_utils::ProcessChunk(std::span<const char>(input.data(), input.size()), start, end, result);

    EXPECT_TRUE(result.empty());
}

TEST(ProcessChunkTest, HandlesTrailingWord) {
    const std::string input = "word";
    std::unordered_set<std::string> result;

    counter_utils::ProcessChunk(std::span<const char>(input.data(), input.size()), 0, input.size(), result);

    EXPECT_EQ(result.size(), 1);
    EXPECT_TRUE(result.contains("word"));
}
