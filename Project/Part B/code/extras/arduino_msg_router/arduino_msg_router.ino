// --------------------------------------
// Include files
// --------------------------------------
#include <string.h>
#include <stdio.h>
#include <time.h>

// --------------------------------------
// CONSTANTS
// --------------------------------------

// --------------------------------------
// Types
// --------------------------------------

// structture of a position on the orbit
struct position {
    float x;
    float y;
    float z;
};

// list of commands to be send
enum command {NO_CMD=0, SET_HEAT_CMD=1, READ_SUN_CMD=2, 
        READ_TEMP_CMD=3, READ_POS_CMD=4};

// structure of command message
struct cmd_msg {
    short int cmd;  // command to execute
    short int set_heater;     // boolean to set or unset the heater
};

// structure of response message
struct res_msg {
    short int cmd;  // command to respond to
    short int status;        // boolean to state if execution went well
    short int sunlight_on;   // boolean to state if sunlight is on
    float temperature;   // value of the temperature
    struct position position;   // value of the position
};

// --------------------------------------
// PUBLIC STATUS (GLOBAL VARIABLES)
// --------------------------------------

// boolean with the status of the heater
int heater_on = 0;

// last command message received
struct cmd_msg last_cmd_msg = {NO_CMD, 0};
// next response message to be send
struct res_msg next_res_msg = {NO_CMD, 0};

//---------------------------------------------------------------------------
//                           AUXILIAR FUNCTIONS 
//---------------------------------------------------------------------------

// --------------------------------------
// Function: send_msg
// --------------------------------------
void send_msg ()
{

    Serial.print("MSG: (");
    // compute parity
    unsigned char res_parity = (unsigned char) 0;
    int ret = 0;

    for (int i=0; i<sizeof(struct cmd_msg); i++) {
       // compute parity (xor)
       res_parity = res_parity ^ ((unsigned char *)(&last_cmd_msg))[i];
    }

    // send message to slave
    ret = Serial.write((char *)&last_cmd_msg,sizeof(struct cmd_msg));
    if( ret < sizeof(struct cmd_msg)){
        Serial.println("ERROR: write Request");
    }
    // send parity to slave
    ret = Serial.write((char *)&res_parity,1);
    if(ret < 1){
        Serial.println("ERROR: write parity");
    }
    Serial.println(")");

}

// --------------------------------------
// Function: delayClock
// --------------------------------------
void recv_msg ()
{

    // compute parity
    unsigned char res_parity = (unsigned char) 0;
    unsigned char parity = (unsigned char) 0;
    int ret = 0;

    // send message to slave
    ret = Serial.readBytes((char *)&next_res_msg,sizeof(struct res_msg));
    if( ret < sizeof(struct res_msg)){
        Serial.println("ERROR: read Response");
    }

    for (int i=0; i<sizeof(struct res_msg); i++) {
       // compute parity (xor)
       res_parity = res_parity ^ ((unsigned char *)(&next_res_msg))[i];
    }

    // send parity to slave
    ret = Serial.readBytes((char *)&parity,1);
    if (ret < 1) {
        Serial.println("ERROR: read parity");
    } else if (parity != res_parity) {
        Serial.println("ERROR: read wrong parity");
        next_res_msg.cmd = NO_CMD;
        next_res_msg.status = 2;
    }
}

//---------------------------------------------------------------------------
//                           MAIN FUNCTIONS
//---------------------------------------------------------------------------
// --------------------------------------
// Function: send_cmd_msg
// --------------------------------------
void send_cmd_msg (int cmd)
{
    // set no command
    if (cmd == NO_CMD) {
        last_cmd_msg.cmd = NO_CMD;

    // set set heater command msg
    } else if (cmd == SET_HEAT_CMD) {
       heater_on = 1 - heater_on;
       last_cmd_msg.cmd = SET_HEAT_CMD;
       last_cmd_msg.set_heater = heater_on;

    // set read sunlight command msg
    } else if (cmd == READ_SUN_CMD) {
        last_cmd_msg.cmd = READ_SUN_CMD;

    // set read temperature command msg
    } else if (cmd == READ_TEMP_CMD) {
        last_cmd_msg.cmd = READ_TEMP_CMD;

    // set read position command msg
    } else if (cmd == READ_POS_CMD) {
        last_cmd_msg.cmd = READ_POS_CMD;
    }

}

// --------------------------------------
// Function: recv_res_msg
// --------------------------------------
void recv_res_msg ()
{
    char buffer[64];
    char num_str1[10];
    char num_str2[10];
    char num_str3[10];

    memset(buffer,'\0', 64);
    memset(num_str1,'\0', 10);
    memset(num_str2,'\0', 10);
    memset(num_str3,'\0', 10);

    // unpack no command
    if (next_res_msg.cmd == NO_CMD) {
        Serial.println("Ans: NO_CMD");
    // unpack set heater response msg
    } else if (next_res_msg.cmd == SET_HEAT_CMD) {
        Serial.println("Ans: SET_HEAT_CMD");
        sprintf(buffer,"Stat: %d",next_res_msg.status);
        Serial.println(buffer);
        sprintf(buffer,"Heat: %d",heater_on);
        Serial.println(buffer);
    // unpack read sunlight response msg
    } else if (next_res_msg.cmd == READ_SUN_CMD) {
        Serial.println("Ans: READ_SUN_CMD");
        sprintf(buffer,"Stat: %d",next_res_msg.status);
        Serial.println(buffer);
        sprintf(buffer,"Sun: %d",next_res_msg.sunlight_on);
        Serial.println(buffer);

    // unpack read temperature response msg
    } else if (next_res_msg.cmd == READ_TEMP_CMD) {
        Serial.println("Ans: READ_TEMP_CMD");
        sprintf(buffer,"Stat: %d",next_res_msg.status);
        Serial.println(buffer);
        dtostrf(next_res_msg.temperature,9,2,num_str1);
        sprintf(buffer,"Temp: %s",num_str1);
        Serial.println(buffer);

    // unpack read position response msg
    } else if (next_res_msg.cmd == READ_POS_CMD) {
        Serial.println("Ans: READ_POS_CMD");
        sprintf(buffer,"Stat: %d",next_res_msg.status);
        Serial.println(buffer);
        dtostrf(next_res_msg.position.x,9,2,num_str1);
        dtostrf(next_res_msg.position.y,9,2,num_str2);
        dtostrf(next_res_msg.position.z,9,2,num_str3);
        sprintf(buffer,"Pos: %s, %s, %s",num_str1, num_str2, num_str3);
        Serial.println(buffer);
    }

    //set response to no command
    next_res_msg.cmd = NO_CMD;
    return;

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
    static int option = 0;

    switch (option) {
      case 0: 
        Serial.println("Send: Null cmd");
        break;
      case 1: 
        Serial.println("Send: Set heater on/off");
        break;
      case 2: 
        Serial.println("Send: Read sun sensor");
        break;
      case 3: 
        Serial.println("Send: Read temperature");
        break;
      case 4: 
        Serial.println("Send: Read position");
        break;       
    }

    // wait 1000 ms
    delay(1000);

    // send cmd
    send_cmd_msg(option);
    send_msg();

    // wait 400 ms
    delay(400);

    //receive answer
    recv_msg();
    recv_res_msg();

    // wait 1000 ms
    delay(2000);
    
    option = (option + 1) % 5;
}
