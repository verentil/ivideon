#ifndef IVIDEON_LED_H
#define IVIDEON_LED_H

#include <map>
#include <set>
#include <sstream>

using std::string;
using std::set;
using std::map;
using std::pair;
using std::stringstream;

extern const string SUCCESS_RESPONSE;
extern const string FAIL_RESPONSE;

class led_options_base
{
public:
    virtual string get () = 0;
    virtual string set (string ) = 0;
};

template <int option_number>
class led_options_template : public led_options_base
{
    static const std::set<string> possible_value;
    string value = *possible_value.begin();
public:
    string get ()
    {
        return SUCCESS_RESPONSE + " " + value;
    }
    string set ( string new_value )
    {
        if ( possible_value.count(new_value) > 0 )
        {
            value = new_value;
            return SUCCESS_RESPONSE;
        }
        return FAIL_RESPONSE;
    }
};

enum led_options { state, color, rate };

class led_state : public led_options_template <led_options( state )> {};

class led_color : public led_options_template <led_options( color )> {};

class led_rate : public led_options_template <led_options( rate )> {};

class led
{
    led_state state;
    led_color color;
    led_rate rate;
    map <string, led_options_base * > command_map;
    // The map structure is: a possible command -> due object.
    // Example:
    // The command to set led rate to 2 ("set-led-rate 2") split to the command ("set-led-rate") and argument ("2"). The first - is a Key for the map.
    // The due object is a rate object of a led_rate class. The pointer to this is a second value for the map.
public:
    led ();
    string perform (string );
    ~led () {};
};


#endif // IVIDEON_LED_H
