#ifndef __FUNTIONLOBJECT__HPP__
#define __FUNTIONLOBJECT__HPP__

struct start_exe{
    void operator()(void *args);
};

struct start_command{
    void operator()(void *args);
};

#endif