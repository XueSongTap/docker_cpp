#ifndef __CMDLINE_HPP__
#define __CMDLINE_HPP__

class cmdLine
{
private:
    CLI::App app;
    bool _isExe = false;
    bool _isCpuSumTime = false;
    std::string _memorySize = "";
    std::string _exePath = "";
    std::string _cpuSize = "";
    bool _volume = false;
private:
    bool helpFlag = false;
private:
    bool initRunCommand();
    void initCommand();
public:
    cmdLine();
    int parse(int argc,char **argv);
    void exeFul();
};

#endif