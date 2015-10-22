#include "led.h"

const string SUCCESS_RESPONSE = "OK";
const string FAIL_RESPONSE = "FAILED";

template<>
const set<string> led_state::led_options_template<led_options( state )>::possible_value { "off", "on" };

template<>
const set<string> led_color::led_options_template<led_options( color )>::possible_value { "red", "green", "blue" };

template<>
const set<string> led_rate::led_options_template<led_options( rate )>::possible_value { "0", "1", "2", "3", "4", "5" };

led::led()
{
    command_map.insert( pair <string, led_options_base * > ("set-led-state", & state) );
    command_map.insert( pair <string, led_options_base * > ("get-led-state", & state) );
    command_map.insert( pair <string, led_options_base * > ("set-led-color", & color) );
    command_map.insert( pair <string, led_options_base * > ("get-led-color", & color) );
    command_map.insert( pair <string, led_options_base * > ("set-led-rate", & rate) );
    command_map.insert( pair <string, led_options_base * > ("get-led-rate", & rate) );
}

string led::perform ( string command_and_argument )
{
    stringstream input(command_and_argument);
    string command;
    input >> command;
    auto search_result = command_map.find(command);
    if ( search_result != command_map.end() )
    {
        string command_type = "";       // Type of command is a first 3 symbols.
        for ( int i = 0 ; i < 3 ; i++ )
            command_type += command[i];
        if ( command_type == "get" ) return search_result->second->get();
        else
        {
            string first_arg = "";
            input >> first_arg;         // The program performs set-command with a first argument only!
            return search_result->second->set(first_arg);
        }
    }
    return FAIL_RESPONSE;
}

string led::get_possible_commands()
{
    string possible_commands = "";
    for (auto command : command_map)
        possible_commands += command.first + " ";
    return possible_commands;
}

