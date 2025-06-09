cmake_minimum_required(VERSION 3.10)

project(UniqueWordsCounter)

set(CMAKE_CXX_STANDARD 20)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)


add_executable(UniqueWordsCounter
    main.cpp
    MMapFile.cpp
)

add_executable(test_wordcounter tests/test_wordcounter.cpp)
target_link_libraries(test_wordcounter gtest_main)