add_executable(exercise1 exercise-1/exercise1.cpp)
add_executable(exercise2 exercise-2/exercise2.cpp)
add_executable(exercise3 exercise-3/exercise3.cpp)
add_executable(exercise4 exercise-4/exercise4.cpp)

target_link_libraries(exercise1 PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(exercise2 PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(exercise3 PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(exercise4 PRIVATE OpenMP::OpenMP_CXX)