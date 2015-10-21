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

int main()
{

    led active_led;

    unlink(server_interface.c_str());

    if ( mkfifo (server_interface.c_str(), 0666) == -1 )
    {
        cerr << "Can not create fifo pipe " << server_interface << "!" << endl;
        return 1;
    }

    int fd_fifo = open (server_interface.c_str(), O_RDWR);

    if ( fd_fifo == -1 )
    {
        cerr << "Can not open fifo pipe " << server_interface << "!" << endl;
        return 1;
    }

    char MSG[MAX_MSG_SIZE];

    int readed_size;
    while ( ( readed_size = read(fd_fifo, MSG, MAX_MSG_SIZE)) && ( readed_size > 0 ) )
    {
        cout << "Message readed" << endl;
        string clean_message = string (MSG, readed_size - 1);
        cout << clean_message << endl;
        accept_new_user(clean_message, active_led);
    }

    cout << "After reading" << endl;

    close(fd_fifo);

    return 0;
}
