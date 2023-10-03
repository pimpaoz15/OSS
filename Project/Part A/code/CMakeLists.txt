cmake_minimum_required(VERSION 2.6)
 
# Locate GTest
# if using LINUX change this path to the path of your googletest folder for: /usr/src/gtest
add_subdirectory("/Users/pimpao/Library/CloudStorage/OneDrive-Personal/Code/C/OSS/Project/Part A/code/googletest" ${CMAKE_BINARY_DIR}/gtest)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(test_arduino_code test_arduino_code.cpp arduino_code.c)
target_link_libraries(test_arduino_code ${GTEST_LIBRARIES})

add_executable(test_i386_code test_i386_code.cpp i386_code.c)
target_link_libraries(test_i386_code ${GTEST_LIBRARIES})
