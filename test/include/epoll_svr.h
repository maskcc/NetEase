#ifndef  TEST_EPOLLSVR_H_
#define  TEST_EPOLLSVR_H_

#include <sys/epoll.h> //epoll
#include <netinet/in.h>//sockaddr_in
#include <sys/socket.h>//socket, listen, bind, accept, connect, shutdown,sockaddr
#include <error.h>//errno, EINTR
#include <arpa/inet.h> //sockaddr_in, in_addr, ntohs, htonl
#include <unistd.h> //read, write, close
#include <stdint.h>
#include <string.h>
#include <sys/fcntl.h>
#include <netinet/tcp.h> //TCP_NODELAY

#include <thread>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <map>
#include <iostream>
#include <memory>
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include <glog/logging.h>

const int32_t MAX_EVENTS = 128;

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
            uint64_t CreateTimer(uint64_t tick, const _OnTimerHandler& handler);
            uint64_t CreateTimer(uint64_t tick, _OnTimerHandler &&handler);
            //删除定时器
            bool CancelTimer(uint64_t id);
            //检查定时器
            void CheckTimer();
            uint32_t GetNextExpireTime();
        private: 
            //毫秒为单位
            uint64_t GetSysTick();

        private:
            int64_t MakeTimerID();
            std::map<uint64_t, _OnTimerHandler*> timer_map_;
            const uint64_t start_sys_time_ = GetSysTick();

    };

//发送的数据包缓冲结构
class DataBuffer
    {
        public:
            //sock, 连接的fd
            //sz, 包的总长
            //rsz, real sz, 接收到的包的长度
            //ptr, 数据地址
            DataBuffer(int32_t sock, int32_t sz, int32_t rez, const char* ptr);
            void AddData(int32_t sz, const char* ptr){}

        private:
            int32_t  sock_;  //文件描述符
            int32_t  sz_;    //数据包大小
            int32_t  pos_;   //当前发送的字节下标
            std::vector<uint8_t> buf_;

    };
using DataBufferPtr = std::shared_ptr<DataBuffer>;

//一些统计信息
class MONITOR
    {
        public:
            MONITOR(){recv_pack_count_ = 0; send_pack_count_ = 0;}
            void AddRecvPack(){++recv_pack_count_;}
            void AddSendPack(){++send_pack_count_;}
        private:
            std::atomic<uint64_t> recv_pack_count_; //收到的包数量
            std::atomic<uint64_t> send_pack_count_; //收到的包数量

    };

//线程安全的消息队列
//放入的参数应该是智能指针
template <class T>
class SafeQueue
    {
        public:
            SafeQueue()
            {
                stop_ = false;
            }
            ~SafeQueue()
            {

            }
            void Init(int32_t sz)
            {
                max_sz_ = sz;

            }

            bool IsFull()
            {
                std::unique_lock<std::mutex> locker(mt_);
                return msg_queue_.size() >= max_sz_;

            }
            bool IsEmpty()
            {
                std::unique_lock<std::mutex> locker(mt_);
                return msg_queue_.empty();

            }
            bool Put(const T &msg)
            {   
                std::unique_lock<std::mutex> locker(mt_);
                auto cb = [this]{//服务没停, 空间没满就能一直put
                    return stop_ || IsNotFull() ;
                };  
                not_full_.wait(locker, cb);
                if(stop_){
                    return false; //已经停止
                }   

                msg_queue_.push(msg);
                not_empty_.notify_one();

                return true;
            }

            bool Pour(std::vector<T> &vec)
            {
                std::unique_lock<std::mutex> locker(mt_);
                auto cb = [this]{//服务没停, 空间没空就能一直获取消息
                    return stop_ || IsNotEmpty() ;
                };
                not_empty_.wait(locker, cb);
                if(stop_){
                    return false; //已经停止
                }

                //msg = std::move(msgQ);
                while(!msg_queue_.empty()){ 
                    vec.push_back(msg_queue_.back());
                    msg_queue_.pop(); 
                }
                not_full_.notify_one();

                return true;
            }

        private:
            bool IsNotFull()
            {
                return msg_queue_.size() < max_sz_;
            }

            bool IsNotEmpty()
            {
                return !msg_queue_.empty();
            }

        private:
            bool stop_;         //是否已经停止
            uint32_t max_sz_;     //队列最大消息数量
            std::queue<T> msg_queue_; //消息队列
            std::mutex mt_;      //互斥变量
            std::condition_variable not_empty_;
            std::condition_variable not_full_;

    };

class IPlayer
{
    public:
        virtual void OnNetMessage();
        virtual bool Send();
    private:
        uint64_t identify_;//用来标志自己的身份
        int32_t  fd_;      //当前连接fd

};
//连接信息的智能指针
using IPlayerPtr = std::shared_ptr<IPlayer>;
class Socket
{
    public:
        bool Send();    //发送消息
        bool Receive(); //接收消息
    private:
        int32_t fd_;         //链接信息
        std::string  addr_;  //地址
        int32_t port_;       //端口
        int64_t id_;         //唯一标识符
        


};
using SocketPtr = std::shared_ptr<Socket>;

//已连接的服务器
class TCPSocket : public IPlayer
{

};

//监听服务器
class TCPAccept : public IPlayer
{


};

class UDPSocket : public IPlayer
{

};

class TCPConnector : public IPlayer
{
    public:
        //自动重连 
        void OnReconnect(){}
    private:
        //是否进行重连
        bool do_reconn_; 
        //重连时间
        int32_t reconn_time_; 
        //重连次数
        int32_t reconn_count_; 
        //最多重连次数
        int32_t max_reconn_count_; 

};

//定义 epoll_event
using EPOLL_EV = struct epoll_event;
//epoll net event
class TCPEvent
{
    public:
        TCPEvent();
        void Proc();             //处理


    private:
        EPOLL_EV ev_;
        bool read_able_;    //可读
        bool write_able_;   //可写
        SocketPtr sock_; //当前连接


};

class EPOLLSvr
{
    public:
        bool SendMessage(IPlayerPtr player, void *msg, int32_t sz);
        bool Connect(std::string dest, bool reconnect); //是否重连
        int32_t Wait();
    private:
        int32_t epoll_fd_; //epoll fd, 用来监听发送接收消息的
        std::map<uint64_t, IPlayerPtr> player_map_; //所有连接信息都在这
        Timer timer_;
        EPOLL_EV events_[MAX_EVENTS];

};
class NetPackage
{
    public:
        static const int32_t kMAX_BACK_LOG = 1024;
        static const int32_t kINVALID_FD = -1;
        static bool IsIPV6(const std::string& ip);
        static bool SetNonBlock(int32_t fd);
        static bool SetNoDelay(int32_t fd);
        static bool SetReuse(int32_t fd);
        static bool SetIPV6Only(int32_t fd, bool only);
        //端口号要注意是uint16_t 类型, 如果不是unsigned, 会有负数端口号
        static bool Listen(int32_t *fd, const std::string& listenIP, uint16_t port, bool reuse);
        static bool Accept6(int32_t listen_fd, int32_t *fd, std::string *ip, uint16_t *port);
        static bool Accept(int32_t listen_fd, int32_t *fd, std::string *ip, uint16_t *port);
        static bool Close(int32_t fd);
        static int32_t Read(int32_t fd, void *buf, int32_t sz);
        static int32_t Write(int32_t fd, const void *buf, int32_t sz);
        static bool Connect(int *connFD, const std::string& remoteIP, uint16_t remotePort);
};



#endif

