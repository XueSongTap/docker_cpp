#include "stdfx.hpp"
#include "myErrMacroDefin.hpp"

Socket::Socket()
{
    std::cout << "Socket()" << std::endl;
}

int Socket::init() //只在父进程中执行
{
    _isFather = true;
    return myErr(pipe(fd), "pipe");
}

int Socket::closeRead()
{
    _isFather = true;
    int result = myErr(close(fd[0]), "close");
    fd[0] = -1; //如果是fd是-1说明资源已释放，或未开启描述符。
    return result;
}

int Socket::closeWrite()
{
    _isFather = false;
    int result = myErr(close(fd[1]), "close");
    fd[1] = -1;
    return result;
}

inline const Socket &Connect::getSocket()
{
    return _mySocket;
}

inline int Socket::getReadFileDescription()
{
    return fd[0];
}

inline int Socket::getWriteFileDescription()
{
    return fd[1];
}

Socket::~Socket()
{
    if (_isFather)
    {
        if (fd[1] != -1)
        {
            myErr(close(fd[1]), "close");
        }
    }
    else
    {
        if (fd[0] != -1)
        {
            myErr(close(fd[0]), "close");
        }
    }
    std::cout << "~Socket()" << std::endl;
}

Connect::~Connect()
{
    std::cout << "~Connect()" << std::endl;
}

int Connect::sendMessage(const std::string &message)
{
    if (_mySocket.closeRead() == -1)
    {
        std::cout << __FILE__ << ":" << std::endl;
        return -1;
    }
    int result = 0;
    if ((result = write(_mySocket.getWriteFileDescription(), message.c_str(), message.size())) != message.size())
    {
        std::cout << "error!write:" << __FILE__ << ":" << __LINE__ << std::endl;
        return result;
    }
    return result;
}

int Connect::readMessage(std::string &message)
{
    if (_mySocket.closeWrite() == -1)
    {
        std::cout << __FILE__ << ":" << std::endl;
        return -1;
    }
    size_t size = message.size();
    char buf[1000] = "\0";
    int res = 0;
    while (res = read(_mySocket.getReadFileDescription(), buf, sizeof(buf)))
    {
        if (res == -1)
        {
            return -1;
        }
        message.append(buf);
    }
    return message.size() - size;
}