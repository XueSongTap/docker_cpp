#ifndef __CGROUP_HPP__
#define __CGROUP_HPP__

class Cgroup
{
private: 
    int _processId = 0;
    std::string _memoryHierarchyMount = ""; //一些配置资料抽成configure类（后面扩展）
    std::unordered_map<std::string,std::string> _hierarchyMount;
    std::string _memoryAimDocker = "";
    std::unordered_map<std::string,std::pair<bool,std::string>> _aimDocker;
    mode_t _dockerMod = S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH;//设定创造cgroup节点时的文件权限
    struct stat _fileState;//用来保存使用setDirMod时，获取的文件状态信息.
    std::string _path;//用来保存使用setDirMod时，获取的文件路径.
private:
    bool setDirMod(const std::string path);//设定指定文件权限为777。
    bool setDirBackMod();//只能恢复到最近一次使用setDirMod时获取的文件的权限状态。
    void initHierarchyPath();//初始化载入Cgroup目录。
    bool creatCgroupNode(std::string path);//创造指定路径的Cgroup节点
    bool cgroupWrite(const std::string path,const std::string strContent);//往指定路径的cgroup节点文件写入指定内容
public:
    bool cgroupMemoryLimit(const std::string size);//限定容器的内存使用
    bool cgroupCpuLimit(const std::string size);//限定容器的CPU使用
    bool init(int processId);
    ~Cgroup();
    Cgroup();
};
#endif