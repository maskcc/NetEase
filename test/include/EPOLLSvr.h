#include <sys/epoll.h> //epoll
#include <netinet/in.h>//sockaddr_in
#include <sys/socket.h>//socket, listen, bind, accept, connect, shutdown,sockaddr
#include <error.h>//errno, EINTR
#include <arpa/inet.h> //sockaddr_in, in_addr, ntohs, htonl
#include <unistd.h> //read, write, close
#include <stdint.h>
#include <string.h>

#include <thread>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <map>
#include <iostream>

//定时器
//定时执行过程, 能够添加过程, 删除过程
//写在主线程中, 能处理主线程数据, 在epoll里处理, epoll超时为 1秒

using _OnTimerHandler = std::function<void()>;
class Timer
{
    public:
        Timer();
        ~Timer();
    public:
        //创建timer, 返回timerID
        //tick 超时时间, 单位毫秒, handler 处理的函数
        uint64_t createTimer(uint64_t tick, const _OnTimerHandler& handler);
        uint64_t createTimer(uint64_t tick, _OnTimerHandler &&handler);
        //删除定时器
        bool cancelTimer(uint64_t id);
        //检查定时器
        void checkTimer();
        uint32_t getNextExpireTime();
    private: 
        //毫秒为单位
        uint64_t getSysTick();
        
    private:
        int64_t makeTimerID();
        std::map<uint64_t, _OnTimerHandler*> timerMap;
        const uint64_t startSysTime = getSysTick();

};

class DataBuffer
{
    public:
        DataBuffer(int32_t _sz, char* ptr);


    private:
        int32_t  sock; //文件描述符
        int32_t sz;    //数据包大小
        std::vector<char> buf;
    



};

