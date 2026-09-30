
## Building
It is recommended to use a preset to configure the project. Type

    cmake --preset <preset name>

where "preset name" should be replaced with any of the presets listed when running `cmake --list-presets`. To compile everything after configuration, type

    cmake --build <build directory>

> **_NOTE:_** `<build folder>` is the generated directory's name and it is assumed here that `<build folder>` has been set to `build`. If you prefer a different name for the build directory, replace every instance of "build" (that refers to a directory name) within the commands listed here.    



## Useful commands
List all tests:

    ctest --test-dir build --show-only

Run all tests:

    ctest --test-dir build

Run a single test:

    ctest --test-dir build -R <test name>


> **_NOTE:_** -R is actually used for regular expression matching, but you can specify the name of a test because it is a valid regular expression.

Generate test coverage (__only when using GCC, Gcov and Gcovr__): 

    cmake --build build --target=coverage

Format files of all CMake targets with Clang-format:

    cmake --build build --target=format

Run static analysis on files using Clang-tidy:

    cmake --build build --target=lint

## Modifying the template
Use CMake's `add_executable` and `add_library` for adding source files. To define header search paths for a compiler, in other words include directories, use `target_include_directories`. 


Sanitizers such as _AddressSanitizer_, _UndefinedBehaviorSanitizer_ and _ThreadSanitizer_ can be useful when tracking down bugs. To enable them within your project, set the CMake cache variables `ENABLE_ASAN`, `ENABLE_UBSAN` and `ENABLE_TSAN` to `ON`. This can be done by adding `-DENABLE_ASAN=ON` or using [ccmake](https://cmake.org/cmake/help/latest/manual/ccmake.1.html) on Unix platforms. 

> _**NOTE**_: this template supports the use of _AddressSanitizer_, _UndefinedBehaviorSanitizer_ and _ThreadSanitizer_. Memory sanitizer is not supported and compilers on Windows operating systems may not support some of the mentioned sanitizers.