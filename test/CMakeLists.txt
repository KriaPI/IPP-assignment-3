include(GoogleTest)

# Define executable targets
add_executable(example test.cpp)

# Link targets
target_link_libraries(example PRIVATE
    GTest::gtest GTest::gtest_main integralApproximator
)

# Define tests
gtest_discover_tests(example)