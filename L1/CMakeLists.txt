cmake_minimum_required(VERSION 2.6)
 
# Locate GTest
add_subdirectory("/Users/pimpao/Library/CloudStorage/OneDrive-Personal/Code/C/OSS/L1/googletest" ${CMAKE_BINARY_DIR}/gtest)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Link main program
add_executable(runMain main.c func_to_test.c)
target_link_libraries(runMain m)
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests test_main.cpp test_suit_factorial.cpp test_suit_isprime.cpp test_suit_squareroot.cpp func_to_test.c)
target_link_libraries(runTests ${GTEST_LIBRARIES})
