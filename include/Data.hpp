#ifndef __DATA_HPP__
#define __DATA_HPP__

struct Arg
{
    Arg(json &myJson)
    {
        processId = myJson["processId"];
        argc = myJson["argc"].get<int>();
        argv = new char*[argc];
        for(int i = 0;i < argc; ++i)
        {
            std::string temp = myJson[std::to_string(i)].get<std::string>();
            argv[i] = new char[temp.size() + 1];
            memset(argv[i],0,temp.size() + 1);
            memcpy(argv[i],temp.c_str(),temp.size());
        }
    }
    ~Arg()
    {
        for(int i = 0; i < argc;++i)
        {
            delete [] argv[i];
        }
        delete [] argv;
    }
    int argc = 0;
    char **argv = nullptr;
    int processId = 0;
};



#endif