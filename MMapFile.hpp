#pragma once

#include <filesystem>
#include <span>
#include <sys/stat.h>

// A class that memory-maps a file for read-only access
class MMapFile {
public:
    MMapFile(const std::filesystem::path& path);
    ~MMapFile();

    // Returns a span representing the mapped file contents (read-only)
    std::span<const char> Data() const;

private:
    int fd;             // File descriptor for the opened file
    struct stat st;     // Stores metadata about the file (e.g., size)
    char* data;         // Pointer to the mapped memory region
};
