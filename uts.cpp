#include "stdfx.hpp"



int main(int argc, char **argv)
{
    cmdLine myCmd;
    myCmd.parse(argc,argv);
    json infor;
    initJson(infor,argc,argv);
    Connect myConnect;
    startSecondProcess(myConnect,infor, myCmd);
    return 0;
}