#include "stdfx.hpp"
//只能放在实现上，不能放在声明上，否则会出错

//可分为两级日志输出，接近系统调用的采取myErr输出系统错误信息，不包括文件名和行号。
//接近自己写的应用层调用，采用logErr输出，要输出文件名和行号。
int myErr(int err, const std::string mistakeInfo, const std::string fileName, const int line) //将来扩展，实现可变参数列表。
{
    if (err == -1)
    {
        std::cout << "file is " << fileName << " Line is " << line << " ";
        perror(mistakeInfo.c_str());
        std::cout << std::endl;
        return -1;
    }
    return 0;
}

int logErr(int err, const std::string fileName, const int line)
{
    if (err == -1)
    {
        std::cout << "file is " << fileName << " Line is " << line << " ";
        return -1;
    }
    return 0;
}

/* class MyErr
:public std::exception
{
public:
    MyErr(std::string e)
    {
        descriptoiio = std::move(e);
    }
    const char*
    what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT
    {
        return descriptoiio.c_str();
    }
private:
    std::string descriptoiio;
};

void test()
{
    try
    {
        std::exception e;
        throw e;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
} */

int dockerMain(void *args)
{
    sleep(1);
    /*  std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    test();
    std::cout << __FILE__ << ":" << __LINE__ << std::endl; */
    json myjson;
    Connect &myConnect = *(static_cast<Connect *>(args));
    if (readInitInformation(myConnect, myjson) == -1)
    {
        std::cout << __FILE__ << " :" << __LINE__ << std::endl;
        return -1;
    }
    Arg data(myjson);
    cmdLine myCmd;
    Config::get().setProcessID(data.processId);
    myCmd.parse(data.argc, data.argv);

    myCmd.exeFul();
    return 0;
}

#define myErr(x, y) myErr(x, y, __FILE__, __LINE__)

int sendInitInformation(Connect &myConnect, const json &iniInformation)
{
    return myConnect.sendMessage(iniInformation.dump());
}
int readInitInformation(Connect &myConnect, json &initInformation)
{
    std::string message;
    int result = myConnect.readMessage(message);
    initInformation = json::parse(message);
    return result;
}

/* char children[SIZE_STACK] = "\0"; */
void startSecondProcess(Connect &myConnect, json &initInformation, cmdLine &myCmd)
{
    std::cout << "welcome to myDocker\n";

    static char children[SIZE_STACK] = "\0"; //目前看来只能用静态局部变量来解决，其它方式都有问题。
    /* char children[SIZE_STACK] = "\0"; */
    /* std::unique_ptr<char> children(new char[SIZE_STACK]);
    memset(children.get(),0,SIZE_STACK); */
    Config::get().setProcessID(clone(dockerMain, children /* .get() */ + SIZE_STACK, CLONE_NEWNET | CLONE_NEWUSER | CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWUTS | SIGCHLD, &myConnect)); //子进程退出时会发出信号给父进程 避免僵尸进程，记得将进程间的资源传递进行优化，单纯的值传递效率太低。
    initInformation["processId"] = Config::get().getProcessID();
    sendInitInformation(myConnect, initInformation);
    myConnect._mySocket.closeWrite();
    std::cout << "processId is " << Config::get().getProcessID() << std::endl;

    std::string command("echo -17 >");
    command += "/proc/" + std::to_string(Config::get().getProcessID()) + "/oom_adj";
    myErr(system(command.c_str()), "system"); //这个地方需要同步，目前是用sleep函数暂代.

    if (!myCmd.getDetch())
    {
        std::unique_ptr<int> status(new int(0));
        myErr(waitpid(Config::get().getProcessID(), status.get(), 0), "waitpid");
        std::cout << WTERMSIG(*status) << std::endl;
        std::cout << "成功退出\n";
        return ;
    }
    std::cout << "父进程结束" << std::endl;
    return;
}

void initJson(json &myJson, int argc, char **argv)
{
    myJson["argc"] = argc;
    for (int i = 0; i < argc; ++i)
    {
        myJson[std::to_string(i)] = argv[i];
        std::cout << argv[i] << " ";
    }
    std::cout << std::endl;
}