#include "led_user.h"
#include "timeout_inspector.h"

const string led_user::welcome_message = "Welcome to led management interface!\n";

void led_user::terminate()
{
    const string BY_MSG = "By By";
    user_pipe->write_message( BY_MSG );
}

void led_user::activate(timeout_inspector & inspector)
{
    string command;
    while ( user_pipe->read_message( command ) > 0 )
    {
        if (disconnected)
        {
            std::cout << "User ID " << user_id << " disconnected. Client time out." << std::endl;
            break;
        }
        if (command == EXIT_COMMAND)
        {
            terminate();
            break;
        }
        inspector.renew (this);
        string response = user_led->perform(command);
        user_pipe->write_message( response );
    }
}
