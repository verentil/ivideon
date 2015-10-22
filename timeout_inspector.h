#ifndef IVIDEON_TIMEOUTINSPECTOR_H
#define IVIDEON_TIMEOUTINSPECTOR_H

#include <boost/thread.hpp>
#include <boost/thread/lock_guard.hpp>
#include <map>
#include <unistd.h>

#include "fifo_pipe.h"

using std::map;
using std::string;
using std::pair;

extern const string WORKING_DIRECTORY;
extern const int TIMEOUT_IN_SECONDS;
extern const string EXIT_COMMAND;

class led_user;

class timeout_inspector
{
    boost::mutex change_connections;
    map< led_user*, int > connections;
    void terminate_connection( led_user * );
public:
    void add ( led_user * user )
    {
        boost::lock_guard<boost::mutex> lock(change_connections);
        connections.insert( pair<led_user*, int>( user, TIMEOUT_IN_SECONDS ) );
    };
    void remove ( led_user * user )
    {
        boost::lock_guard<boost::mutex> lock(change_connections);
        connections.erase( user );
    };
    void renew ( led_user * user )
    {
        boost::lock_guard<boost::mutex> lock(change_connections);
        connections[ user ] = TIMEOUT_IN_SECONDS;
    };
    void trace ();
};

#endif // IVIDEON_TIMEOUTINSPECTOR_H
