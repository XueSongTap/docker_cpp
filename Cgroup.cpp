#include "stdfx.hpp"
#include "myErrMacroDefin.hpp"

inline bool Cgroup::setDirMod(const std::string path)
{
    myErr(stat(path.c_str(), &_fileState), "stat");
    myErr(chmod(path.c_str(), Config::get()._fileMode), "chmod");
    _path = std::move(path);
    return true;
}

inline bool Cgroup::setDirBackMod()
{
    myErr(chmod(_path.c_str(), _fileState.st_mode), "chmod");
    return true;
}

bool Cgroup::init(int processId)
{
    _processId = processId;
    return true;
}

bool Cgroup::cgroupMemoryLimit(const std::string size)
{
    std::string temp = _aimDocker["memory"].second;
    _aimDocker["memory"].first = true;
    creatCgroupNode(temp);
    cgroupWrite(temp + "/" + "memory.limit_in_bytes", std::move(size));
    cgroupWrite(temp + "/" + "tasks", std::to_string(_processId));
    return true;
}

bool Cgroup::creatCgroupNode(std::string path)
{
    DIR *d = nullptr;
    std::string tempPath = path.substr(0, path.rfind("/"));
    setDirMod(tempPath);
    std::cout << path << std::endl;
    if ((d = opendir(path.c_str())) == nullptr)
    {
        perror("opendir");
        myErr(mkdir(path.c_str(), _dockerMod), "mkdir");
    }
    else
    {
        perror("opendir");
        std::cout << "dir is exited!\n";
        myErr(closedir(d), "closedir");
    }
    setDirBackMod();
    return true;
}

bool Cgroup::cgroupCpuLimit(std::string size)
{
    std::string temp = _aimDocker["cpuacct"].second;
    _aimDocker["cpuacct"].first = true;
    creatCgroupNode(temp);
    cgroupWrite(temp + "/" + "cpu.cfs_period_us", Config::get().getDuration());
    cgroupWrite(temp + "/" + "cpu.cfs_quota_us", std::move(size));
    cgroupWrite(temp + "/" + "tasks", std::to_string(_processId));
    return true;
}

bool Cgroup::cgroupWrite(const std::string path, const std::string strContent)
{
    std::ofstream ofs(path, std::ios::app | std::ios::binary);
    ofs.clear();
    if (ofs.bad())
    {
        std::cout << "文件打开失败\n";
        std::cout << "line:" << __LINE__;
        std::cout << " file:" << __FILE__;
        std::cout << std::endl;
        return false;
    }
    ofs.write(strContent.c_str(), strContent.size());
    if (ofs.bad())
    {
        return false;
    }
    return true;
}

void Cgroup::initHierarchyPath()
{
    _hierarchyMount.reserve(20);
    std::string fileNamePath("/proc/mounts");
    FILE *file = setmntent(fileNamePath.c_str(), "r");
    struct mntent *myMntent = nullptr;
    if (!file)
    {
        perror("setmntent");
        exit(0);
    }
    while (myMntent = getmntent(file))
    {
        if (std::string(myMntent->mnt_type) == "cgroup")
        {
            std::string temp(myMntent->mnt_opts);
            auto pos = temp.rfind(",") + 1;
            temp = temp.substr(pos);
            _hierarchyMount[temp] = myMntent->mnt_dir;
            _aimDocker[temp].second = _hierarchyMount[temp] + "/myDocker";
            _aimDocker[temp].first = false;
        }
    }
    endmntent(file);
}

Cgroup::Cgroup()
{
    initHierarchyPath();
}

Cgroup::~Cgroup()//暂时先不做处理，资源的清理和回收。目前暂定方案采用通信的方式把信息发送给父进程。
{
    for (auto &ele : _aimDocker)
    {
        if (ele.second.first)
        {
            myErr(remove(ele.second.second.c_str()), "remove");
            std::cout << ele.second.second << std::endl;
        }
    }
}