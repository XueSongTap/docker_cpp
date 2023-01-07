#include "stdfx.hpp"

Config::Config()
{
    tinyxml2::XMLDocument doc;
    std::string cwd;
    cwd.reserve(100);
    cwd = getcwd(nullptr,0);
    cwd.append("/Config/config.xml");
    doc.LoadFile(cwd.c_str());
    _loadFile = std::move(cwd);
    int err = doc.ErrorID();
    if (0 != err)
    {
        std::cout << getcwd(nullptr,0) << std::endl;
        std::cout << "加载xml文件失败 错误码：" << err << std::endl;
        exit(-1);
    }
    auto nodeFirst = doc.FirstChildElement("Config");
    _duration = nodeFirst->FirstChildElement("cpu")->GetText();
    _myRoot = nodeFirst->FirstChildElement("busybox")->GetText();
    
}
Config::~Config()
{
    
}