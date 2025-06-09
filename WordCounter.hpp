#pragma once

#include <unordered_set>
#include <string>
#include <ranges>
#include <span>
#include <algorithm>

namespace counter_utils
{
    // Processes a chunk of the input data in a separate thread
    // Extracts words and inserts them into an unordered_set corresponding to the thread
    inline void ProcessChunk(
        std::span<const char> data,
        std::size_t start,
        std::size_t end,
        std::unordered_set<std::string>& set)
    {
        size_t i = start;
        
        while (i < end && data[i] != ' ')
        {
            i++;
        }

        std::string word;
        while (i < end)
        {
            word.clear();

            // Read a word (sequence of non-space characters)
            while (i < end && data[i] != ' ')
            {
                word += data[i];
                i++;
            }

            // Insert word into the corresponding shard if it's not empty
            if (!word.empty())
            {
                set.insert(std::move(word));
            }

            // Skip any spaces between words
            while (i < end && data[i] == ' ') ++i;
        }
    }

    // Checks if all characters in the input are either lowercase letters [a..z] or spaces
    constexpr bool IsValidInput(std::span<const char> data)
    {
        return std::ranges::all_of(data, [](char c) {
            return c == ' ' || (c >= 'a' && c <= 'z');
        });
    }
    static_assert(IsValidInput(std::span<const char>("  hello world", 11)));
    static_assert(IsValidInput(std::span<const char>("abc xyz", 7)));
    static_assert(IsValidInput(std::span<const char>("", 0)));
    static_assert(!IsValidInput(std::span<const char>("Hello world", 11)));
    static_assert(!IsValidInput(std::span<const char>("hello_world", 11)));
    static_assert(!IsValidInput(std::span<const char>("hello123", 8)));
}