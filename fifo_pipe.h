#ifndef IVIDEON_FIFOPIPE_H
#define IVIDEON_FIFOPIPE_H

#include <fcntl.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

extern const int MAX_MSG_SIZE;

using std::cerr;
using std::endl;
using std::string;

class fifo_pipe
{
    string pipe_file;
    int pipe_description;
    fifo_pipe(){};
    void fifo_close()
    {
        close(pipe_description);
    };
    bool fifo_open (const int mode);
public:
    fifo_pipe(string filename) : pipe_file(filename) {};
    ~fifo_pipe(){};
    int read_message (string &);
    void write_message (string);
    bool initialise();
};

#endif // IVIDEON_FIFOPIPE_H
