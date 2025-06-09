# UniqueWordsCounter

This project counts the number of unique words in a text file using multithreading and memory-mapped file access.

## Prerequisites
- CMake (version 3.10 or higher recommended)
- A C++20 compatible compiler (e.g., g++, clang++)
- Make (or another build tool supported by CMake)

## Build Instructions

1. **Clone the repository** (if you haven't already):
   ```bash
   git clone <repo-url>
   cd UniqueWordsCounter
   ```

2. **Create a build directory and configure the project:**
   ```bash
   mkdir -p build
   cd build
   cmake ..
   ```

3. **Build the project:**
   ```bash
   make
   ```
   This will generate the `UniqueWordsCounter` executable in the `build/` directory.

## Usage

Run the program from the build directory, providing a path to a text file as an argument:

```bash
./UniqueWordsCounter ../test_inputs/sample_input.txt
```

- The program will output the number of unique words found in the file.
- Only lowercase letters [a..z] and spaces are considered valid input. Any other characters will cause the program to exit with an error.

## Testing

If you have built the test target (e.g., `test_wordcounter`), you can run it as follows:

```bash
./test_wordcounter
```

## Notes
- Input files should be plain text and encoded in UTF-8.
- The program uses all available hardware threads for processing.

## License
See [LICENSE](LICENSE) for details.
