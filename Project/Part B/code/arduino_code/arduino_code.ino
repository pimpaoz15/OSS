// --------------------------------------
// Include files
// --------------------------------------
#include <string.h>
#include <stdio.h>
#include <time.h>

// --------------------------------------
// CONSTANTS
// --------------------------------------

#define SHIP_SPECIFC_HEAT 0.9
#define SHIP_MASS 10.0         // Kg
#define HEATER_POWER 150.0     // J/sec
#define SUNLIGHT_POWER 50.0    // J/sec
#define HEAT_POWER_LOSS -100.0 // J/sec

#define ORBIT_POINTS_SIZE 20
#define ORBIT_TIME 300.0 // sec

// --------------------------------------
// Types
// --------------------------------------

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
   short int cmd;            // command to respond to
   short int status;         // boolean to state if execution went well
   short int sunlight_on;    // boolean to state if sunlight is on
   float temperature;        // value of the temperature
   struct position position; // value of the position
};

// --------------------------------------
// PUBLIC STATUS (GLOBAL VARIABLES)
// --------------------------------------

// boolean with the status of the heater
int heater_on = 0;
// boolean with the status of the sunlight
int sunlight_on = 0;
// Save the actual temperature of the ship
double temperature = 0.0;
// save the last time temperature was computed
double time_temperature = 0.0;
// inital time of the orbit
double init_time_orbit = 0.0;
// actual position of the ship
struct position position = {0.0, 0.0, 0.0};

bool command_in_process = false;
bool response_ready = false;

// last command message received
struct cmd_msg last_cmd_msg = {NO_CMD, 0};
// next response message to be send
struct res_msg next_res_msg = {NO_CMD, 0};

// --------------------------------------
// PRIVATE STATUS (STATIC GLOBAL VARIABLES)
// --------------------------------------

// position data for the orbit
static struct position orbit_points[ORBIT_POINTS_SIZE] = {
    {3000.00, 0, 12000.0},
    {2853.169548885460, 1854.101966249680, 11412.678195541800},
    {2427.050983124840, 3526.711513754840, 9708.203932499370},
    {1763.355756877420, 4854.101966249680, 7053.423027509680},
    {927.050983124842, 5706.339097770920, 3708.203932499370},
    {0.0, 6000.0, 0.0},
    {-927.050983124842, 5706.339097770920, -3708.203932499370},
    {-1763.355756877420, 4854.101966249680, -7053.423027509680},
    {-2427.050983124840, 3526.711513754840, -9708.203932499370},
    {-2853.169548885460, 1854.101966249680, -11412.678195541800},
    {-3000.0, 0.0, -12000.0},
    {-2853.169548885460, -1854.101966249690, -11412.678195541800},
    {-2427.050983124840, -3526.711513754840, -9708.203932499370},
    {-1763.355756877420, -4854.101966249680, -7053.423027509680},
    {-927.050983124843, -5706.339097770920, -3708.203932499370},
    {0.0, -6000.0, 0.0},
    {927.050983124842, -5706.339097770920, 3708.203932499370},
    {1763.355756877420, -4854.101966249690, 7053.423027509680},
    {2427.050983124840, -3526.711513754840, 9708.203932499370},
    {2853.169548885460, -1854.101966249690, 11412.678195541800}};

// Command message parity
unsigned char cmd_parity = (unsigned char)0;

//---------------------------------------------------------------------------
//                           AUXILIAR FUNCTIONS
//---------------------------------------------------------------------------

// --------------------------------------
// Function: getClock
// --------------------------------------
double getClock()
{
   return ((double)millis()) / 1000.0;
}

// --------------------------------------
// Function: delayClock
// --------------------------------------
void delayClock(double time)
{
   delay(time * 1000.0);
}

//---------------------------------------------------------------------------
//                           MAIN FUNCTIONS
//---------------------------------------------------------------------------
// --------------------------------------
// Function: comm_server
// --------------------------------------
int comm_server()
{
   static int count = 0;
   unsigned char car_aux;

   // If there were a received msg, send the processed answer or ERROR if none.
   // then reset for the next request.
   // NOTE: this requires that between two calls of com_server all possible
   //       answers have been processed.
   if (command_in_process)
   {
      // if there is an answer send it, else error
      if (!response_ready)
      {
         next_res_msg.cmd = NO_CMD;
         next_res_msg.status = 1;
      }
      // compute parity
      unsigned char res_parity = (unsigned char)0;
      for (int i = 0; i < sizeof(struct res_msg); i++)
      {
         // compute parity (xor)
         res_parity = res_parity ^ ((unsigned char *)(&next_res_msg))[i];
      }
      Serial.write((unsigned char *)(&next_res_msg), sizeof(struct res_msg));
      Serial.write((unsigned char *)(&res_parity), 1);
      // reset flags and buffers
      command_in_process = false;
      response_ready = false;
      memset((unsigned char *)(&last_cmd_msg), 0, sizeof(struct cmd_msg));
      memset((unsigned char *)(&next_res_msg), 0, sizeof(struct res_msg));
   }

   while (Serial.available())
   {
      // read one character
      car_aux = Serial.read();
      if ((count == 0) && (car_aux > 4))
      {
         continue;
      }
      // Check if it is the last byte of the msg or not.
      if (count == sizeof(struct cmd_msg))
      {
         // check parity error
         if (cmd_parity != car_aux)
         {
            // set error answer
            last_cmd_msg.cmd = NO_CMD;
            last_cmd_msg.set_heater = 0;
            command_in_process = true;
            next_res_msg.cmd = NO_CMD;
            next_res_msg.status = 2;
            response_ready = true;
            cmd_parity = (unsigned char)0;
            count = 0;
            // end loop
            break;
         }
         // finish reading msg
         cmd_parity = (unsigned char)0;
         count = 0;
         command_in_process = true;

         break;
      }
      else
      {
         // Store the character
         ((unsigned char *)(&last_cmd_msg))[count] = car_aux;
         // compute parity (xor)
         cmd_parity = cmd_parity ^ car_aux;
      }

      // Increment the count
      count++;
   }
}

/**********************************************************
 *  Function: get_temperature
 *********************************************************/
void get_temperature()
{
   // Calculate the elapsed time since the last temperature update
   double current_time = getClock();
   double elapsed_time = current_time - time_temperature;

   // Calculate the total power gained or lost by the satellite
   double total_power = HEAT_POWER_LOSS;
   if (heater_on)
   {
      total_power += HEATER_POWER;
   }
   if (sunlight_on)
   {
      total_power += SUNLIGHT_POWER;
   }

   // Calculate the energy transferred
   double energy_transferred = total_power * elapsed_time;

   // Update the temperature using the energy transfer formula
   temperature += energy_transferred / (SHIP_SPECIFC_HEAT * SHIP_MASS);

   // Update the last temperature update time
   time_temperature = current_time;
}

/**********************************************************
 *  Function: get_position
 *********************************************************/

void get_position()
{
   // Calculate the time elapsed since the last orbit started (relative time)
   // fmod is used to get the division module of the result and the time for a single orbit
   double current_time = getClock();
   double relative_time = fmod(current_time - init_time_orbit, ORBIT_TIME);

   // Calculate the indices of the two consecutive positions in the orbit_points array
   int previous_position_index = (int)((relative_time * ORBIT_POINTS_SIZE) / ORBIT_TIME);
   int next_position_index = (previous_position_index + 1) % ORBIT_POINTS_SIZE;

   // Calculate the offset ratio of the actual position
   double offset_ratio = (relative_time / (ORBIT_TIME / ORBIT_POINTS_SIZE)) - previous_position_index;

   // Update each position coordinate
   position.x = orbit_points[previous_position_index].x * (1 - offset_ratio) +
                orbit_points[next_position_index].x * offset_ratio;
   position.y = orbit_points[previous_position_index].y * (1 - offset_ratio) +
                orbit_points[next_position_index].y * offset_ratio;
   position.z = orbit_points[previous_position_index].z * (1 - offset_ratio) +
                orbit_points[next_position_index].z * offset_ratio;
}

/**********************************************************
 *  Function: exec_cmd_msg
 *********************************************************/

void exec_cmd_msg()
{
   // Initialize the response message with default values
   next_res_msg.cmd = last_cmd_msg.cmd;
   next_res_msg.status = 0; // Default status is failure (0)

   switch (last_cmd_msg.cmd)
   {
   case SET_HEAT_CMD:
      // Update the heater status based on the command
      if (last_cmd_msg.set_heater == 1)
      {
         heater_on = 1;
      }
      else if (last_cmd_msg.set_heater == 0)
      {
         heater_on = 0;
      }
      // Set the status in the response message to indicate success
      next_res_msg.status = 1;
      break;

   case READ_SUN_CMD:
      // Set the status in the response message to indicate success
      next_res_msg.status = 1;
      // Set the sunlight_on value in the response message
      next_res_msg.data.sunlight_on = sunlight_on;
      break;

   case READ_TEMP_CMD:
      // Set the status in the response message to indicate success
      next_res_msg.status = 1;
      // Get the current temperature and update it in the response message
      get_temperature();
      next_res_msg.data.temperature = temperature;
      break;

   case READ_POS_CMD:
      // Set the status in the response message to indicate success
      next_res_msg.status = 1;
      // Get the current position and update it in the response message
      get_position();
      next_res_msg.data.position = position;
      break;

   case NO_CMD:
   default:
      // For NO_CMD or unknown commands, there is no specific response.
      break;
   }

   // Set the last received command variable to NO_CMD
   last_cmd_msg.cmd = NO_CMD;
}

/**********************************************************
 *  Function: read_sun_sensor
 *********************************************************/
void read_sun_sensor()
{
}

/**********************************************************
 *  Function: set_heater
 *********************************************************/
void set_heater()
{
}

// --------------------------------------
// Function: setup
// --------------------------------------
void setup()
{
   // Setup Serial Monitor
   Serial.begin(9600);
}

// --------------------------------------
// Function: loop
// --------------------------------------
void loop()
{
   comm_server();
   exec_cmd_msg();
   get_temperature();
   get_position();
   read_sun_sensor();
   set_heater();
   delay(100);
}
