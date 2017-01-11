#include "epoll_svr.h"

//timer
Timer::Timer() {

}

Timer::~Timer() {

}

uint64_t Timer::CreateTimer(uint64_t tick, const _OnTimerHandler& h) {
    return CreateTimer(tick, bind(h));
}

uint64_t Timer::CreateTimer(uint64_t tick, _OnTimerHandler &&h) {
    uint64_t t = GetSysTick() - this->start_sys_time_;
    //超时距离 startSysTime 的时间差为 t
    t += tick;
    _OnTimerHandler *pFunc = new _OnTimerHandler(std::move(h));
    timer_map_.insert(make_pair(t, pFunc));
    return t;
}

bool Timer::CancelTimer(uint64_t id) {

}

void Timer::CheckTimer() {

}

uint64_t Timer::GetSysTick() {
    return (uint64_t)(std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch()).count());

}

uint32_t Timer::GetNextExpireTime() {
    uint32_t tm = -1;
    if(!timer_map_.empty()) {
        tm = timer_map_.begin()->first - GetSysTick() - start_sys_time_;
    }
    tm = std::min(tm, 100U);
    return tm;

}

//DataBuffer
DataBuffer::DataBuffer(int32_t s, int32_t sz, const char* ptr) {
    buf_.resize(sz);
    sz_ = sz;
    sock_ = s;
    memcpy(&buf_[0], ptr, sz);

}

//统计信息
MONITOR MONITOR_SVR;


//网络的包装函数
    bool NetPackage::IsIPV6(const std::string& ip) {
        bool isV6 = ip.find(':') != std::string::npos;
        return isV6;
    }

    bool NetPackage::SetNonBlock(int fd) {
        return fcntl((fd), F_SETFL, fcntl(fd, F_GETFL)|O_NONBLOCK) == 0;
    }

    bool NetPackage::SetNoDelay(int fd) {
        int bTrue = true?1:0; return setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char*)&bTrue, sizeof(bTrue)) == 0;
    }

    bool NetPackage::SetReuse(int fd) {
        int bReuse = 1;return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &bReuse, sizeof(bReuse)) == 0;
    }

    bool NetPackage::SetIPV6Only(int fd, bool only) {
        int ipv6only = only ? 1 : 0;return setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &ipv6only, sizeof(ipv6only)) == 0;
    }

    bool NetPackage::Listen(int32_t *listen_fd, const std::string& listenIP, uint16_t port, bool reuse) {
        int32_t fd = kINVALID_FD;
        bool isV6 = IsIPV6(listenIP); 
        if(isV6) {
            fd = socket(AF_INET6, SOCK_STREAM, 0);
        } else {
            fd = socket(AF_INET, SOCK_STREAM, 0);
        }
        //SetNonBlock(fd);
        if(reuse){
            SetReuse(fd);
        }

        if(isV6){
            sockaddr_in6 addr = {0};
            addr.sin6_family = AF_INET6;
            addr.sin6_addr = in6addr_any;
            addr.sin6_port = htons(port);
            if(bind(fd, (sockaddr *)&addr, sizeof(addr)) != 0){
                //LOG(ERROR)
                ::close(fd);
                return false;
            }
        }else{
            sockaddr_in addr = {0};
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = INADDR_ANY;
            addr.sin_port = htons(port);
            if(bind(fd, (sockaddr *)&addr, sizeof(addr)) != 0){
                //LOG(ERROR)
                ::close(fd);
                return false;
            }
            if(listen(fd,kMAX_BACK_LOG) != 0){
                //LOG(ERROR)
                ::close(fd);
                return false;
            }
        }

        (*listen_fd) = fd;
        return true;
    }

    //ipv6的 accept
    bool NetPackage::Accept6(int32_t listen_fd, int32_t *fd, std::string *ip, uint16_t *port) {
        char remote_ip[64] = {0};
        uint16_t acc_port = 0;
        int32_t acc_fd = kINVALID_FD;
        sockaddr_in6 cltaddr = {0};
        socklen_t len = sizeof(cltaddr);
        acc_fd = accept(static_cast<int>(listen_fd), (sockaddr*)&cltaddr, &len);

        if(acc_fd <= 0){
            //接收失败
            //LOG(ERROR)
            return false;
        }
        inet_ntop(AF_INET6, &cltaddr.sin6_addr,  remote_ip, 64);
        acc_port = ntohs(cltaddr.sin6_port);
        (*ip) = std::string(remote_ip);
        (*port) = acc_port;
        (*fd) = acc_fd;
        
        return true;
    }

    //ipv4 的 accept
    bool NetPackage::Accept(int32_t listen_fd, int32_t *fd, std::string *ip, uint16_t *port) {
        char remote_ip[64] = {0};
        uint16_t acc_port = 0;
        int32_t acc_fd = kINVALID_FD;
        sockaddr_in cltaddr = {0};
        socklen_t len = sizeof(cltaddr);
        acc_fd = accept(static_cast<int>(listen_fd), (sockaddr*)&cltaddr, &len);

        if(acc_fd <= 0){
            //接收失败
            //LOG(ERROR)
            return false;
        }
        inet_ntop(AF_INET, &cltaddr.sin_addr,  remote_ip, 64);
        acc_port = ntohs(cltaddr.sin_port);
        (*ip) = std::string(remote_ip);
        (*port) = acc_port;
        (*fd) = acc_fd;
        
        return true;
    }

    bool NetPackage::Close(int32_t fd) {
        if(kINVALID_FD == fd){
            //fd不对
            //LOG(ERROR)
            return false;
        }
        ::close(fd);

    }

    //没有要求一次读完数据 
    int32_t NetPackage::Read(int32_t fd, void *buf, int32_t sz) {
        return ::read(fd, buf, sz);

    }
    int32_t NetPackage::Write(int32_t fd, const void *buf, int32_t sz) {
        return ::write(fd, buf, sz);
    }

    bool NetPackage::Connect(int *connFD, const std::string& remoteIP, uint16_t remotePort){
        bool isIPV6 = IsIPV6(remoteIP); 
        int32_t fd = kINVALID_FD;
        int32_t ret = -1;
        if(isIPV6){
            fd = socket(AF_INET6, SOCK_STREAM, 0);
            if(kINVALID_FD == fd){
                //fd 不对
                //LOG(ERROR)
                return false;
            }
            SetNonBlock(fd);
            sockaddr_in6 addr = {0};
            addr.sin6_family = AF_INET6;
            inet_pton(AF_INET6, remoteIP.c_str(), &addr.sin6_addr);
            addr.sin6_port = htons(remotePort);
            ret = connect(fd, (sockaddr *)&addr, sizeof(addr));
        }else{
            fd = socket(AF_INET, SOCK_STREAM, 0);
            if(kINVALID_FD == fd){
                //fd 不对
                //LOG(ERROR)
                return false;
            }
            SetNonBlock(fd);
            sockaddr_in addr = {0};
            addr.sin_family = AF_INET;
            inet_pton(AF_INET, remoteIP.c_str(), &addr.sin_addr);
            addr.sin_port = htons(remotePort);
            ret = connect(fd, (sockaddr *)&addr, sizeof(addr));
        }
        if( 0 != ret && errno != EINPROGRESS){
            //connect 失败
            //LOG(ERROR)
            return false;
        }

        (*connFD) = fd;
        return true;
    }


