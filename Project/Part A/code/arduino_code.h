/**********************************************************
 *  INCLUDES
 *********************************************************/

#include <stdio.h>

/**********************************************************
 *  TYPES
 *********************************************************/

// structture of a position on the orbit
struct position
{
    float x;
    float y;
    float z;
};

// list of commands to be send
enum command
{
    NO_CMD = 0,
    SET_HEAT_CMD = 1,
    READ_SUN_CMD = 2,
    READ_TEMP_CMD = 3,
    READ_POS_CMD = 4
};

// structure of command message
struct cmd_msg
{
    short int cmd;        // command to execute
    short int set_heater; // boolean to set or unset the heater
};

// structure of response message
struct res_msg
{
    short int cmd;    // command to respond to
    short int status; // boolean to state if execution went well
    union
    {
        short int sunlight_on;    // boolean to state if sunlight is on
        float temperature;        // value of the temperature
        struct position position; // value of the position
    } data;
};

/**********************************************************
 *  PUBLIC STATUS (GLOBAL VARIABLES)
 **********************************************************/

// boolean with the status of the heater
extern int heater_on;
// boolean with the status of the sunlight
extern int sunlight_on;
// Save the actual temperature of the ship
extern double temperature;
// save the last time temperature was computed
extern double time_temperature;
// inital time of the orbit
extern double init_time_orbit;
// actual position of the ship
extern struct position position;

// last command message received
extern struct cmd_msg last_cmd_msg;
// next response message to be send
extern struct res_msg next_res_msg;

//---------------------------------------------------------------------------
//                           AUXILIAR FUNCTIONS
//---------------------------------------------------------------------------

/**********************************************************
 *  Function: getClock
 *********************************************************/
double getClock();

//---------------------------------------------------------------------------
//                           MAIN FUNCTIONS
//---------------------------------------------------------------------------

/**********************************************************
 *  Function: get_temperature
 *********************************************************/
void get_temperature();

/**********************************************************
 *  Function: get_position
 *********************************************************/
void get_position();

/**********************************************************
 *  Function: exec_cmd_msg
 *********************************************************/
void exec_cmd_msg();
