#include <gtest/gtest.h>
#include <unistd.h>
#include <stdio.h>

extern "C"
{
#include "arduino_code.h"
}

/**********************************************************
 *  Set heater on test
 *********************************************************/
TEST(ArduinoTest, SetHeaterOn)
{
    // Test setting the heater on
    last_cmd_msg.cmd = SET_HEAT_CMD;
    last_cmd_msg.set_heater = 1;

    exec_cmd_msg();

    // Check if the heater is turned on
    EXPECT_EQ(heater_on, 1);
    // Check if the response status is set to success (1)
    EXPECT_EQ(next_res_msg.status, 1);
    // Check if the response command matches the last command
    EXPECT_EQ(next_res_msg.cmd, SET_HEAT_CMD);
}

/**********************************************************
 *  Read sunlight test
 *********************************************************/
TEST(ArduinoTest, ReadSunlight)
{
    // Test reading sunlight status
    last_cmd_msg.cmd = READ_SUN_CMD;

    exec_cmd_msg();

    // Check if the response status is set to success (1)
    EXPECT_EQ(next_res_msg.status, 1);
    // Check if the response command matches the last command
    EXPECT_EQ(next_res_msg.cmd, READ_SUN_CMD);
}

/**********************************************************
 *  Read temperature test
 *********************************************************/
TEST(ArduinoTest, ReadTemperature)
{
    // Test reading temperature
    last_cmd_msg.cmd = READ_TEMP_CMD;

    exec_cmd_msg();

    // Check if the response status is set to success (1)
    EXPECT_EQ(next_res_msg.status, 1);
    // Check if the response command matches the last command
    EXPECT_EQ(next_res_msg.cmd, READ_TEMP_CMD);
}

/**********************************************************
 *  Get position test
 *********************************************************/
TEST(ArduinoTest, ReadPosition)
{
    // Test reading position
    last_cmd_msg.cmd = READ_POS_CMD;

    exec_cmd_msg();

    // Check if the response status is set to success (1)
    EXPECT_EQ(next_res_msg.status, 1);
    // Check if the response command matches the last command
    EXPECT_EQ(next_res_msg.cmd, READ_POS_CMD);
}

/**********************************************************
 *  Arduino exec_cmd_msg() Tests
 *********************************************************/
TEST(ArduinoTest, ExecCmdMsgTest)
{
    // Test setting the heater on
    last_cmd_msg.cmd = SET_HEAT_CMD;
    last_cmd_msg.set_heater = 1;

    exec_cmd_msg();

    // Check if the heater is turned on
    EXPECT_EQ(heater_on, 1);
    // Check if the response status is set to success (1)
    EXPECT_EQ(next_res_msg.status, 1);
    // Check if the response command matches the last command
    EXPECT_EQ(next_res_msg.cmd, SET_HEAT_CMD);

    // Test reading sunlight status
    last_cmd_msg.cmd = READ_SUN_CMD;

    exec_cmd_msg();

    // Check if the response status is set to success (1)
    EXPECT_EQ(next_res_msg.status, 1);
    // Check if the response command matches the last command
    EXPECT_EQ(next_res_msg.cmd, READ_SUN_CMD);

    // Test reading temperature
    last_cmd_msg.cmd = READ_TEMP_CMD;

    exec_cmd_msg();

    // Check if the response status is set to success (1)
    EXPECT_EQ(next_res_msg.status, 1);
    // Check if the response command matches the last command
    EXPECT_EQ(next_res_msg.cmd, READ_TEMP_CMD);
}

/**********************************************************
 *  Main function
 *********************************************************/
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
