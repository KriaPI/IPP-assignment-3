cmake_minimum_required(VERSION 3.25)
project(Template LANGUAGES CXX)
enable_testing()

# Set C++ language standard and required variables
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(defaults)
include(default_compiler_flags)
include(sanitizers)
include(coverage)

# include(FetchContent)
# FetchContent_Declare(googletest
#   GIT_REPOSITORY https://github.com/google/googletest.git
#   GIT_TAG 063de7e9578f82b369302001269680b4b1553359
# )
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

add_subdirectory(source)
# add_subdirectory(test)

include(project_tools)
project_enable_clang_format()
project_enable_clang_tidy()