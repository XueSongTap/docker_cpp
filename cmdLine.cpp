#include "stdfx.hpp"



bool cmdLine::initRunCommand()
{
    auto run = app.add_subcommand("run", "创造指定的容器");
   
    run->add_flag("-e",_isExe,"指示execPath是可执行文件还是linux命令");
    run->add_flag("-d",_isDetch,"指定容器后台运行");
    run->add_option("--memoryLimit", _memorySize, "限制容器的内存资源");
    run->add_option("--cpuLimit", _cpuSize, "限制可使用的cpu资源");
    run->add_option("--execPath", _exePath, "可执行文件路径或者可执行linux命令")->required(); //测试required意思是否是这个选项必须要，否者命令不成立
    run->add_flag("-v",_volume,"数据卷");
    return true;
}

cmdLine::cmdLine()
{
    app.description("myDocker is simple container runtime implementation.\
        The purpose of this project is to learn how docker works and how to write a docker \
        by ourselves. Enjoy it, just for fun.");
    app.name("myDocker");
    initCommand();
}

int cmdLine::parse(int argc, char **argv)
{
    try
    {
        app.parse(argc,argv);
    }
    catch (const CLI::ParseError &e)
    {
        exit(app.exit(e));
    }
    return 0;
}

void cmdLine::exeFul()
 {
    if (app.got_subcommand("run"))
    {
        auto run = app.get_subcommand("run");
        printf("%s\n",_exePath.c_str());
        myDocker docker(_exePath);     
        if (_isExe)
        {
            docker.callBack = start_exe();
        }else{
            docker.callBack = start_command();
        }       
        if (run->count("--cpuLimit"))
        {
            docker.setCpuLimitSize(_cpuSize);
        }
        if (run->count("--memoryLimit"))
        {
            docker.setMemoryLimitSize(_memorySize);
        }
        docker.dockerStart();
    }
    else
    {
         if(_isCpuSumTime)
        {
            std::cout << Config::get().getDuration() << " 即时长为1000ms\n";
            return;
        }
    }
}

void cmdLine::initCommand()
{
    app.add_flag("--cpuSumTime",_isCpuSumTime,"屏幕打印设定的CPU周期时长");
    initRunCommand();
}

bool cmdLine::getDetch()
{
    return _isDetch;
}