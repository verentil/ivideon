#ifndef IVIDEON_LEDUSER_H
#define IVIDEON_LEDUSER_H

#include <iostream>
#include <memory>

#include "fifo_pipe.h"
#include "led.h"

using std::unique_ptr;

extern const string WORKING_DIRECTORY;
extern const int MAX_MSG_SIZE;
extern const string EXIT_COMMAND;

class timeout_inspector;

class led_user
{
    static const string welcome_message;
    string user_id;
    string user_interface;
    led * user_led;
    unique_ptr<fifo_pipe> user_pipe;
    friend timeout_inspector;
    bool disconnected = false;
    void terminate();
    led_user(){};
public:
    led_user ( string id, led * active_led )
    {
        user_id = id;
        user_led = active_led;
        user_interface = WORKING_DIRECTORY + user_id;
        user_pipe = unique_ptr<fifo_pipe> ( new fifo_pipe ( user_interface ) );
        string first_message = welcome_message +
                               "Correct command list:\n" +
                               user_led->get_possible_commands() + "\n";
        user_pipe->write_message(first_message);
    };
    ~led_user(){};
    void activate (timeout_inspector &);
};

#endif // IVIDEON_LEDUSER_H
