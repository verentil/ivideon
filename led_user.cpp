#include "led_user.h"

int led_user::open_pipe_for_read (int & pipe_description)
{
    pipe_description = open (user_interface.c_str(), O_RDONLY);
    if ( pipe_description == -1 )
    {
        cerr << "Can not open user fifo pipe " << user_interface << " for read!" << endl;
        return 1;
    }
    return 0;
}

int led_user::send_message_to_user (int & pipe_description, string message)
{
    close(pipe_description);
    pipe_description = open (user_interface.c_str(), O_WRONLY);
    if ( pipe_description == -1 )
    {
        cerr << "Can not open user fifo pipe " << user_interface << " for write!" << endl;
    } else {
        write(pipe_description, message.c_str(), message.size() + 1 );
        close(pipe_description);
    }
    return open_pipe_for_read (pipe_description);
}

void led_user::terminate (int & pipe_description)
{
    const string BY_MSG = "By By";
    close(pipe_description);
    pipe_description = open (user_interface.c_str(), O_WRONLY);
    if ( pipe_description == -1 )
    {
        cerr << "Can not open user fifo pipe " << user_interface << " for write!" << endl;
    } else {
        write(pipe_description, BY_MSG.c_str(), BY_MSG.size() + 1 );
        close(pipe_description);
    }
}

void led_user::activate()
{
    int pipe_description;
    if ( open_pipe_for_read( pipe_description ) > 0 )
        return;

    char MSG[MAX_MSG_SIZE];
    int readed_size;
    while ( ( readed_size = read(pipe_description, MSG, MAX_MSG_SIZE) ) && ( readed_size > 0 ) )
    {
        string command(MSG, readed_size - 1);
        if (command == EXIT_COMMAND)
        {
            terminate ( pipe_description );
            break;
        }        
        string response = user_led->perform(command);
        if ( send_message_to_user( pipe_description, response ) > 0 )
           break;
    }
}
