#include "led_user.h"

void led_user::terminate()
{
    const string BY_MSG = "By By";
    user_pipe->write_message( BY_MSG );
}

void led_user::activate()
{
    string command;
    while ( user_pipe->read_message( command ) > 0 )
    {
        if (command == EXIT_COMMAND)
        {
            terminate();
            break;
        }
        string response = user_led->perform(command);
        user_pipe->write_message( response );
    }
}
