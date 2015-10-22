#include "fifo_pipe.h"

bool fifo_pipe::fifo_open (const int mode)
{
    pipe_description = open (pipe_file.c_str(), mode);

    if ( pipe_description == -1 )
    {
        string display_mode;
        switch (mode)
        {
        case O_RDONLY:
            display_mode = " for reading!";
            break;
        case O_WRONLY:
            display_mode = " for writing!";
            break;
        case O_RDWR:
            display_mode = " for reading and writing!";
            break;
        default:
            display_mode = "!";
        }
        cerr << "Can not open fifo pipe " << pipe_file << display_mode << endl;
        return false;
    }
    return true;
}

int fifo_pipe::read_message (string & read_result)
{
    int result_size = 0;
    if ( ! fifo_open ( O_RDONLY) )
        return result_size;

    char MSG[MAX_MSG_SIZE];
    result_size = read(pipe_description, MSG, MAX_MSG_SIZE);
    if ( result_size > 0 )
        read_result = string (MSG, result_size - 1);

    fifo_close();
    return result_size;
}

void fifo_pipe::write_message (string message)
{
    if ( fifo_open (O_WRONLY) )
    {
        write(pipe_description, message.c_str(), message.size() + 1 );
        fifo_close();
    }
}

bool fifo_pipe::initialise()
{
    unlink(pipe_file.c_str());

    if ( mkfifo (pipe_file.c_str(), 0666) == -1 )
    {
        cerr << "Can not create fifo pipe " << pipe_file << "!" << endl;
        return false;
    }
    return true;
}

