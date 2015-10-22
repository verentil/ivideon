#ifndef IVIDEON_LEDUSER_H
#define IVIDEON_LEDUSER_H

#include <fcntl.h>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "fifo_pipe.h"
#include "led.h"

using std::cerr;
using std::endl;
using std::unique_ptr;

extern const string WORKING_DIRECTORY;
extern const int MAX_MSG_SIZE;
extern const string EXIT_COMMAND;

class led_user
{
    string user_id;
    string user_interface;
    led * user_led;
    unique_ptr<fifo_pipe> user_pipe;
    void terminate();
    led_user(){};
public:
    led_user ( string id, led * active_led )
    {
        user_id = id;
        user_led = active_led;
        user_interface = WORKING_DIRECTORY + user_id;
        user_pipe = unique_ptr<fifo_pipe> ( new fifo_pipe ( user_interface ) );
    };
    ~led_user(){};
    void activate ();
};

#endif // IVIDEON_LEDUSER_H
