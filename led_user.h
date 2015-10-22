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
// Tis class defines user connection session. 
// It has required members and method for communication with user.
// Class has the friend "timeout_inspector" - control the user connection by changing member "disconnected".
// The object of this class lives till running the  method "activate". For more detail see the function "accept_new_user" in "main.cpp".
{
    static const string WELCOME_MESSAGE;
    string user_id;
    string user_interface;
    led * user_led;
    unique_ptr<fifo_pipe> user_pipe;
    friend timeout_inspector;
    bool disconnected = false;
    void terminate();
    led_user(){};				// A user must have an ID and link to managed LED.
public:
    led_user ( string id, led * active_led )
    {
        user_id = id;
        user_led = active_led;
        user_interface = WORKING_DIRECTORY + user_id;
        user_pipe = unique_ptr<fifo_pipe> ( new fifo_pipe ( user_interface ) );
        string first_message = WELCOME_MESSAGE +
                               "Correct command list:\n" +
                               user_led->get_possible_commands() + "\n";
        user_pipe->write_message(first_message);
    };
    ~led_user(){};
    void activate (timeout_inspector &);
};

#endif // IVIDEON_LEDUSER_H
