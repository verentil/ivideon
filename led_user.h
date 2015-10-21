#ifndef IVIDEON_LEDUSER_H
#define IVIDEON_LEDUSER_H

#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "led.h"

using std::cerr;
using std::endl;

extern const string WORKING_DIRECTORY;
extern const int MAX_MSG_SIZE;
extern const string EXIT_COMMAND;

class led_user
{
    string user_id;
    string user_interface;
    led * user_led;
    int open_pipe_for_read (int &);
    int send_message_to_user (int &, string);
    void terminate (int &);
    led_user(){};
public:
    led_user ( string id, led * active_led )
    {
        user_id = id;
        user_led = active_led;
        user_interface = WORKING_DIRECTORY + user_id;
    };
    ~led_user(){};
    void activate ();
};

#endif // IVIDEON_LEDUSER_H
