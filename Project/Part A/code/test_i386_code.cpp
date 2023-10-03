/**********************************************************
 *  INCLUDES
 *********************************************************/
#include <gtest/gtest.h>
#include <unistd.h>
#include <stdio.h>

extern "C"
{
#include "i386_code.h"
}

/**********************************************************
 *  Test: control_temperature -> basic
 *********************************************************/

TEST(test_control_temperature, basic)
{
    // test 1
    temperature = 20;
    control_temperature();
    ASSERT_EQ(1, heater_on);

    // test 2
    temperature = 60;
    control_temperature();
    ASSERT_EQ(0, heater_on);
}

/**********************************************************
 *  Test: set_heat_cmd
 *********************************************************/

TEST(test_send_cmd_msg, set_heat_cmd)
{
    // Save the original value of next_cmd_msg.cmd
    short int original_cmd = next_cmd_msg.cmd;

    // Set the command
    send_cmd_msg(SET_HEAT_CMD);

    // Manually set the set_heater value
    next_cmd_msg.set_heater = 1;

    ASSERT_EQ(SET_HEAT_CMD, next_cmd_msg.cmd);
    ASSERT_EQ(1, next_cmd_msg.set_heater);

    // Restore the original value of next_cmd_msg.cmd
    next_cmd_msg.cmd = original_cmd;
}

/**********************************************************
 *  Test: read_sun_cmd
 *********************************************************/
TEST(test_send_cmd_msg, read_sun_cmd)
{
    // Save the original value of next_cmd_msg.cmd
    short int original_cmd = next_cmd_msg.cmd;

    // Set the command
    send_cmd_msg(READ_SUN_CMD);

    ASSERT_EQ(READ_SUN_CMD, next_cmd_msg.cmd);

    // Restore the original value of next_cmd_msg.cmd
    next_cmd_msg.cmd = original_cmd;
}

/**********************************************************
 *  Test: read_temp_cmd
 *********************************************************/
TEST(test_send_cmd_msg, read_temp_cmd)
{
    // Save the original value of next_cmd_msg.cmd
    short int original_cmd = next_cmd_msg.cmd;

    // Set the command
    send_cmd_msg(READ_TEMP_CMD);

    ASSERT_EQ(READ_TEMP_CMD, next_cmd_msg.cmd);

    // Restore the original value of next_cmd_msg.cmd
    next_cmd_msg.cmd = original_cmd;
}

/**********************************************************
 *  Test: read_pos_cmd
 *********************************************************/
TEST(test_send_cmd_msg, read_pos_cmd)
{
    // Save the original value of next_cmd_msg.cmd
    short int original_cmd = next_cmd_msg.cmd;

    // Set the command
    send_cmd_msg(READ_POS_CMD);

    ASSERT_EQ(READ_POS_CMD, next_cmd_msg.cmd);

    // Restore the original value of next_cmd_msg.cmd
    next_cmd_msg.cmd = original_cmd;
}

/**********************************************************
 *  Function: main
 *********************************************************/

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
