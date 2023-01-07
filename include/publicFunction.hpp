#ifndef __PUBLICFUNCTION_HPP__
#define __PUBLICFUNCTION_HPP__

class Connect;

int myErr(int err, const std::string mistakeInfo, const std::string fileName, const int line);//扩展，写一个日志模块
int logErr(int err, const std::string fileName, const int line);
std::string initMistakeInfo(std::string functionName);
void startSecondProcess(Connect &myConnect, json &initInformation);
int dockerMain(void *);
int sendInitInformation( Connect &myConnect, const json &iniInformation);
int readInitInformation( Connect &myConnect, json &initInformation);
void initJson(json &myJson,int argc, char **argv);

#endif