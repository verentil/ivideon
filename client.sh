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

continue=true

while ( "$continue" ) do
  read command
  echo "[$my_ID] inserted command is: \"$command \""
  if [ "$command" == "$exit_command" ]
    then
      continue=false
  fi
  echo "$command" > "$fifo_filename"
  echo "Reading begin"
#  read response < "$fifo_filename"
  response=`cat "$fifo_filename" &` 
  echo "$response"
done

rm "$fifo_filename"


