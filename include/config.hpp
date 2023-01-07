#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

class Config
{
public:
    static Config &get()
    {
        static Config my;
        return my;
    }

public:
    inline const std::string &getMyRoot()
    {
        return _myRoot;
    }
    inline const std::string &getDuration()
    {
        return _duration;
    }
    void setProcessID(int Id)
    {
        _processID = Id;
    }
    inline int getProcessID()
    {
        return _processID;
    }

public:
    const mode_t _fileMode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH | S_IWGRP | S_IWOTH; //0777

private:
    Config();
    ~Config();

private:
    std::string _duration = ""; //CPU周期总时长，单位是us，最小不能低于1000ms，也就是1000000us
    std::string _myRoot = "";
    int _processID = 0;
    std::string _loadFile = "";
};

#endif