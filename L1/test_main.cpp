// test_main.cpp
#include <limits.h>
#include <gtest/gtest.h>

extern "C"
{
#include "func_to_test.h"
}

// Test the squareRoot function with the value 123.145 with 1e-6 tolerance
TEST(SquareRootTest, SquareRootTest1)
{
    double result = squareRoot(123.145);
    EXPECT_NEAR(result, 11.097072, 1e-6);
}

// Test the squareRoot function with the value 63924.12356 with 1e-6 tolerance
TEST(SquareRootTest, SquareRootTest2)
{
    double result = squareRoot(63924.12356);
    EXPECT_NEAR(result, 252.832204, 1e-6);
}

// Test the squareRoot function with the value 72346.18452 with 1e-6 tolerance
TEST(SquareRootTest, SquareRootTest3)
{
    double result = squareRoot(72346.18452);
    EXPECT_NEAR(result, 268.972461, 1e-6);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
