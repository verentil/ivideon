#include <iostream>
#include <boost/thread.hpp>
#include <unistd.h>

#include "fifo_pipe.h"
#include "led.h"
#include "led_user.h"
#include "timeout_inspector.h"

using namespace std;
using boost::thread;

const int MAX_MSG_SIZE = 100;
const string WORKING_DIRECTORY = "/tmp/";
const string EXIT_COMMAND = "exit";
const string SERVER_INTERFACE = WORKING_DIRECTORY + "led_server";

void accept_new_user(string user_id, led & user_led, timeout_inspector & inspector)
{
    led_user new_user(user_id, &user_led);
    inspector.add (&new_user);
    new_user.activate (inspector);
    inspector.remove (&new_user);
    cout << "User connection terminated (ID: " << user_id << ")" << endl;
}

int main()
{
    led active_led;

    fifo_pipe server_pipe(SERVER_INTERFACE);

    server_pipe.initialise();

    timeout_inspector inspector;

    thread inspector_thread ( boost::bind( &timeout_inspector::trace, &inspector ) );
    inspector_thread.detach();

    string new_user_id;
    while ( server_pipe.read_message(new_user_id) > 0 )
    {
        cout << "New connection with user ID: " << new_user_id << endl;

        thread * user_thread = new thread( &accept_new_user, new_user_id, boost::ref(active_led), boost::ref(inspector) );

        usleep(10000);		// Time delay for correct thread initialisation.

        user_thread->detach();
    }

    return 0;
}
