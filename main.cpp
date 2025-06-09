#include <iostream>
#include <vector>
#include <thread>
#include <filesystem>
#include <system_error>
#include "WordCounter.hpp"
#include "MMapFile.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Please provide file as an argument\n";
        return EXIT_FAILURE;
    }

    try
    {
        const std::filesystem::path file_path = argv[1];
        MMapFile file(file_path);                 // Memory-map the input file
        auto data = file.Data();            // Get the memory-mapped data as a span

        if (data.empty())
        {
            std::cout << "0\n";
            return EXIT_SUCCESS;
        }

        // Remove trailing new line if present
        if (data.back() == '\n' || data.back() == '\r')
        {
            data = data.subspan(0, data.size() - 1);
        }

        // Validate input to ensure only lowercase letters and spaces are present
        if (!counter_utils::IsValidInput(data))
        {
            std::cerr << "Invalid input! Only lowercase letters [a..z] and spaces are allowed.\n";
            return EXIT_FAILURE;
        }

        const std::size_t thread_count = std::thread::hardware_concurrency(); // Use number of hardware threads
        std::vector<std::unordered_set<std::string>> sets(thread_count);      // One set per thread for local results

        std::vector<std::thread> workers;
        const std::size_t chunk_size = data.size() / thread_count;           // Divide data into roughly equal chunks

        for (size_t t = 0; t < thread_count; ++t)
        {
            std::size_t start = t * chunk_size;
            std::size_t end = (t == thread_count - 1) ? data.size() : (t + 1) * chunk_size;

            // Extend the chunk to the next space to avoid splitting words
            if (end < data.size())
            {
                while (end < data.size() && data[end] != ' ') ++end;
            }

            // Launch a thread to process the chunk
            workers.emplace_back(counter_utils::ProcessChunk, data, start, end, std::ref(sets[t]));
        }

        // Wait for all threads to finish
        std::ranges::for_each(workers, &std::thread::join);

        std::vector<std::string> flat;
        // Flatten the shard sets into a single vector to avoid multiple insertion to unordered_set
        std::ranges::copy(sets | std::views::join, std::back_inserter(flat));

        // Insert all words into a final unordered_set to deduplicate
        std::unordered_set<std::string> all_unique_words(flat.begin(), flat.end());
      
        // Output the total number of unique words
        std::cout << all_unique_words.size() << '\n';

    }
    catch (const std::system_error& e)
    {
        std::cerr << "System error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
