#include "MMapFile.hpp"
#include <stdexcept>
#include <system_error>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

MMapFile::MMapFile(const std::filesystem::path& path) :
    fd(-1),
    st{},
    data(nullptr)
{
    fd = ::open(path.c_str(), O_RDONLY);
    if (fd < 0) throw std::system_error(errno, std::generic_category(), "XDFailed to open file: " + path.string());

    if (fstat(fd, &st) < 0)
    {
        ::close(fd);
        throw std::system_error(errno, std::generic_category(), "Failed to stat file");
    }

    if (st.st_size == 0) return;

    data = static_cast<char*>(::mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (data == MAP_FAILED)
    {
        ::close(fd);
        throw std::system_error(errno, std::generic_category(), "Failed to mmap file");
    }
}

MMapFile::~MMapFile() {
    if (data && data != MAP_FAILED) {
        ::munmap(data, st.st_size);
    }
    if (fd >= 0) {
        ::close(fd);
    }
}

std::span<const char> MMapFile::Data() const
{
    return {data, static_cast<std::size_t>(st.st_size)};
}
