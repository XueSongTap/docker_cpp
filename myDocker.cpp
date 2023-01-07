#include "stdfx.hpp"
#include "myErrMacroDefin.hpp"

void myDocker::mountProc()
{
    auto flag = MS_NOEXEC | MS_NOSUID | MS_NODEV;
    myErr(mount("proc", "/proc", "proc", flag, ""), "mount");
}


/* void myDocker::start()
{
    std::cout << "welcome to myDocker\n";
    _processId = clone(&dockerMain, children + SIZE_STACK, CLONE_NEWNET | CLONE_NEWUSER | CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWUTS | SIGCHLD, this); //子进程退出时会发出信号给父进程 避免僵尸进程，记得将进程间的资源传递进行优化，单纯的值传递效率太低。
    std::cout << "processId is " << _processId << std::endl;

    std::string command("echo -17 >");
    command += "/proc/" + std::to_string(_processId) + "/oom_adj";
    myErr(system(command.c_str()), "system");//这个地方需要同步，目前是用sleep函数暂代

    myErr(waitpid(_processId, nullptr, 0), "waitpid");
    std::cout << "成功退出\n";
} */

void myDocker::dockerStart()
{
    GetCgroup().init(GetPid());
    memoryLimit();
    cpuLimit();
    initSystem();
    callBack(this);
}

void myDocker::initSystem()
{

    myErr(chdir(Config::get().getMyRoot().c_str()),"chdir");
    myErr(mount(Config::get().getMyRoot().c_str(),Config::get().getMyRoot().c_str(),"bind",MS_BIND|MS_REC,""),"mount");
    std::string tempPath = Config::get().getMyRoot() + "/.pivot_Root";
    myErr(mkdir(tempPath.c_str(),Config::get()._fileMode),"mdkir");
    myErr(syscall(SYS_pivot_root,Config::get().getMyRoot().c_str(),tempPath.c_str()),"pivot_root");   
    myErr(chdir("/"),"chdir");
    tempPath = "/.pivot_Root";//前面更换了rootfs后，跳到根目录应该是当前根目录，所以，是/.pivot_Root
    
    mountProc();//挂载proc文件系统，内存文件系统，为top等命令提供获取内存运行时系统数据的接口。
                //必须在umount2函数卸载旧rootfs挂载点前挂载，否者会爆出无权限的错误。
    
    myErr(umount2(tempPath.c_str(),MNT_DETACH),"umount");  
    myErr(remove(tempPath.c_str()),"remove");

    myErr(mount("tmpfs","/dev","tmpfs",MS_NOSUID|MS_STRICTATIME,"mode=755"), "mount");
}

void myDocker::initFileSystem()//目前废弃不用。
{
    createWriteLayer("/root");
    createMountPoint("/root","/root/mnt");
}
 
void myDocker::createWriteLayer(const std::string rootUrl)
{
    std::string aimPath = rootUrl + "/writeLayer";
    myErr(mkdir(aimPath.c_str(), Config::get()._fileMode),"mdkir");
}

void myDocker::createMountPoint(const std::string rootUrl, const std::string mntUrl)
{
    myErr(mkdir(mntUrl.c_str(),Config::get()._fileMode),"mkdir");
    std::string dirs = "dirs=" + rootUrl+ "/writeLayer:" + rootUrl + "/busybox";
    std::string order;
    order.reserve(100);
    order.append("/bin/mount -t aufs -o ");
    order.append(dirs);
    order.append(" none ");
    order.append(mntUrl);
    
    std::cout << order << std::endl;

    myErr(system(order.c_str()), "system");
}