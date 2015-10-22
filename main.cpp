#include <iostream>
#include <boost/thread.hpp>

#include "fifo_pipe.h"
#include "led.h"
#include "led_user.h"

using namespace std;
using boost::thread;

const int MAX_MSG_SIZE = 100;
const string WORKING_DIRECTORY = "/tmp/";
const string EXIT_COMMAND = "exit";
const string SERVER_INTERFACE = WORKING_DIRECTORY + "led_server";

void accept_new_user(string user_id, led & user_led)
{
    led_user new_user(user_id, & user_led);
    new_user.activate();
}

int main()
{
    led active_led;

    fifo_pipe server_pipe(SERVER_INTERFACE);

    server_pipe.initialise();

    string new_user_id;
    while ( server_pipe.read_message(new_user_id) > 0 )
    {
        cout << "New connection with user ID: " << new_user_id << endl;
        accept_new_user(new_user_id, active_led);
        cout << "User connection terminated (ID: " << new_user_id << ")" << endl;
    }

    return 0;
}
