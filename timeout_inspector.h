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
// Class control a number of users connection by disconnecting inactive users.
// The inactive users - users that dont sent any messages over time.
// Critical time defined by constant TIMEOUT_IN_SECONDS.
{
    boost::mutex change_connections;
    map< led_user*, int > connections;
    // The map used to store information about active users and appropriate time before disconnecting then (second value in seconds).
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
    // If user sent any message before disconnecting then remaning time (second value at the map) becomes TIMEOUT_IN_SECONDS again.
    {
        boost::lock_guard<boost::mutex> lock(change_connections);
        connections[ user ] = TIMEOUT_IN_SECONDS;
    };
    void trace ();				// The main inspector method.
};

#endif // IVIDEON_TIMEOUTINSPECTOR_H
