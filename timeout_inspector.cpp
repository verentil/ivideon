#include "timeout_inspector.h"
#include "led_user.h"

const int TIMEOUT_IN_SECONDS = 60;

void timeout_inspector::trace ()
{
    while (true)
    {
        usleep(1000000);		 // sleep for 1000000mcs = 1s
        boost::lock_guard<boost::mutex> lock(change_connections);
        for ( auto user : connections )
        {
            connections [user.first] --;
            if ( user.second == 0 )
                terminate_connection (user.first);
        }
    }
}

void timeout_inspector::terminate_connection ( led_user * user )
{
    user->disconnected = true;
    user->user_pipe->write_message( EXIT_COMMAND );
}


