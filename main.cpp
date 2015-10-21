#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <boost/thread.hpp>
#include <unistd.h>
#include <fcntl.h>

#include "led.h"
#include "led_user.h"

using namespace std;
using boost::thread;

const int MAX_MSG_SIZE = 100;
const string WORKING_DIRECTORY = "/tmp/";
const string EXIT_COMMAND = "exit";
const string server_interface = WORKING_DIRECTORY + "led_server";

void accept_new_user(string user_id, led & user_led)
{
    led_user new_user(user_id, & user_led);
    new_user.activate();
}

bool pipe_initialise ()
{
    unlink(server_interface.c_str());

    if ( mkfifo (server_interface.c_str(), 0666) == -1 )
    {
        cerr << "Can not create fifo pipe " << server_interface << "!" << endl;
        return false;
    }
    return true;
}

bool pipe_open (int & pipe_description)
{
    pipe_description = open (server_interface.c_str(), O_RDONLY);

    if ( pipe_description == -1 )
    {
        cerr << "Can not open fifo pipe " << server_interface << "!" << endl;
        return false;
    }
    return true;
}

bool read_from_pipe (string & response)
{
    int pipe_description;
    if ( ! pipe_open (pipe_description) )
        return false;

    char MSG[MAX_MSG_SIZE];
    int readed_response_size = read(pipe_description, MSG, MAX_MSG_SIZE);
    if ( readed_response_size > 0 )
        response = string (MSG, readed_response_size - 1);
    else
    {
        close(pipe_description);
        return false;
    }

    close(pipe_description);
    return true;
}

int main()
{

    led active_led;

    pipe_initialise();

    string new_user_id;
    while ( read_from_pipe(new_user_id) )
    {
        cout << "New connection with user ID: " << new_user_id << endl;
        accept_new_user(new_user_id, active_led);
        cout << "User connection terminated (ID: " << new_user_id << ")" << endl;
    }

    return 0;
}
