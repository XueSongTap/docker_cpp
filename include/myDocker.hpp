#ifndef __MYDOCKER__
#define __MYDOCKER__

class myDocker
{
#define SIZE_STACK (1024 * 1024 * 20)
public:
    explicit myDocker(std::string exeScript)
        : _exeScript(std::move(exeScript))
    {
        _processId = Config::get().getProcessID();
    }
    void setMemoryLimitSize(std::string size)
    {
        _memoryLimit = true;
        _memoryLimitSize = std::move(size);
    }
    void setCpuLimitSize(std::string size)
    {
        _cpuLimit = true;
        _cpuLimitSize = std::move(size);
    }
    std::string getExeScript()
    {
          return _exeScript;
    }
public:
    /* void start(); */
    void dockerStart();
private:
    void mountProc();
    void initSystem();
    void initFileSystem();
    void createWriteLayer(const std::string rootUrl);
    void createMountPoint(const std::string rootUrl, const std::string mntUrl);
private:
    /* static int dockerMain(void *args) */ //不用静态函数会造成clone中函数指针类型不匹配，原因是clone
    //中的第一个参数的类型是不带this指针的函数指针类型参数
   /*  {
        auto docker = static_cast<myDocker *>(args);
        sleep(1);
        docker->GetCgroup().init(docker->GetPid());
        docker->memoryLimit();
        docker->cpuLimit();
        docker->initSystem();       
        docker->callBack(args);
        return 0;
    } */
    
    void memoryLimit()
    {
        if (_memoryLimit)
        {
            _myCgroup.cgroupMemoryLimit(_memoryLimitSize);
        }
    }
    void cpuLimit()
    {
        if(_cpuLimit)
        {
            _myCgroup.cgroupCpuLimit(_cpuLimitSize);
        }
    }
    Cgroup &GetCgroup()
    {
        return _myCgroup;
    }
    int GetPid() //获取生成的docker进程pid（注意是映射在外界namespace）
    {
        return _processId;
    }
private:
    Cgroup _myCgroup;
private:
    char children[SIZE_STACK] = "\0";
    int _processId = 0;
    std::string _exeScript;
private:
    std::string _memoryLimitSize;
    std::string _cpuLimitSize;
    bool _memoryLimit = false;
    bool _cpuLimit = false;
public:
    std::function<void(void *)> callBack;
};

#endif