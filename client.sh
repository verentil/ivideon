#!/bin/bash
#Client script for manage the led.

working_directory="/tmp/"
server_fifo="/tmp/led_server"
exit_command="exit"

my_ID=`date +%s`
fifo_filename="$working_directory""$my_ID"

while ( [ -f "$fifo_filename" ] ) do
  my_ID=`date +%s`
  fifo_filename="$working_directory""$my_ID"
done

mkfifo "$fifo_filename"

echo "$my_ID" > "$server_fifo" &

echo "Waiting server"

welcome_message=`cat "$fifo_filename"`

echo "$welcome_message"

echo "Please use this commands for manage the led or type \"$exit_command\" for exit."

continue=true

while ( "$continue" ) do
  read command
#  echo "$command "
  if [ "$command" == "$exit_command" ]
    then
      continue=false
  fi
  echo "$command" > "$fifo_filename" &
  response=`cat "$fifo_filename" &` 
  echo "$response"
done

rm "$fifo_filename"


