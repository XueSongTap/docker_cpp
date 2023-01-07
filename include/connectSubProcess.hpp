#ifndef __CONNECT_HPP__
#define __CONNECT_HPP__


class Socket
{
public:
    Socket();
    int init();
    int getReadFileDescription();
    int getWriteFileDescription();
    int closeRead();
    int closeWrite();
    ~Socket();
private:
    int fd[2];
    bool _isFather;//判断是否是父进程
};

class Connect //创造管道用于父子进程间的通信.
{
public:
    explicit
    Connect()
    {
        if(_mySocket.init() == -1)
        {
            std::cout << __FILE__ << " Line:" << __LINE__ << std::endl; 
        }
    }
    ~Connect();   
    int sendMessage(const std::string &message);
    int readMessage(std::string &message);
    const Socket &getSocket();
public:
    Socket _mySocket;
private:
    int _fd = 0;
    bool _isFather = false;
};




#endif