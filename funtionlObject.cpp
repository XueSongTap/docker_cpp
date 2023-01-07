#include "stdfx.hpp"
#include "myErrMacroDefin.hpp"

void start_exe::operator()(void *args)
{
    auto docker = static_cast<myDocker *>(args);
    myErr(execl(docker->getExeScript().c_str(), nullptr), "execl");
}
void start_command::operator()(void *args)
{
    auto docker = static_cast<myDocker *>(args);
    myErr(system(docker->getExeScript().c_str()), "system");
}