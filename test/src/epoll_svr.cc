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
    DataBuffer::DataBuffer(int32_t s, int32_t sz) {
        buf_.resize(sz);
        sz_    = sz;
        sock_  = s;
        pos_   = 0;
    }
    DataBuffer::~DataBuffer(){
        LOG(INFO) << "DataBuffer Destructed! sz:" << sz_ << " sock_:" << sock_ << "pos:" << pos_;
    }
    int32_t DataBuffer::AddData(int32_t sz, const char * ptr) {
        //至少要先把长度发过来
        if(pos_ + sz > sz_){
            //错误, 数据长度不对
            return -1;
        }
        memcpy(&buf_[pos_], ptr, sz);
        pos_ += sz;
        return sz_ - pos_;
    }
    int32_t DataBuffer::NeedData() {
        LOG(INFO)<< "sz_:" << sz_ << " pos_:" << pos_;
        int32_t len = sz_ - pos_;
        if(len < 0){
            return -1;
        }
        return len;

    }
    void DataBuffer::Resize(int32_t sz){
        buf_.resize(sz);
        sz_ = sz;
        
    }
    const void* DataBuffer::GetBuffPtr(){
        return &buf_[0];        
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
        SetNonBlock(fd);
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
                LOG(ERROR) << "errno[" << errno << "]" << " info[" << strerror(errno) << "]";
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


//Socket
    Socket::Socket() {
        addr_ = "";
        fd_ = NetPackage::kINVALID_FD;
        port_ = -1;
    }

//IPlayer
    IPlayer::IPlayer() {        
        identify_ = next_id();
    }
    IPlayer::~IPlayer() {
    }
    void IPlayer::OnNetMessage() {
    }
    bool IPlayer::Send() {
    }
    uint64_t IPlayer::getID() {
        return identify_;
    }
    void IPlayer::KickOut() {
        NetPackage::Close(peer_.fd_);
    }
    void IPlayer::SetHandler(On_Accept_Handler h){
        
    }
    void IPlayer::SetHandler(On_Socket_Handler h){
        
    }

//TCPEvent
    TCPEvent::TCPEvent() {        
        read_able_ = false;
        write_able_ = false;
        fd_ = NetPackage::kINVALID_FD;
        memset(&ev_, 0, sizeof(ev_));
    }
    void TCPEvent::Proc() {
        int32_t event = ev_.events;
        if( event & EPOLLERR || event & EPOLLHUP){
            LOG(ERROR) << "epoll error or hup";
            return;
        }
        if( event & EPOLLIN ){
            read_able_ = true;
        }
        if( event & EPOLLOUT ){
            write_able_ = true;
        }
        // TODO 需要处理 EPOLLERR, 
        return;

    }
   
    void TCPEvent::SetPlayer(uint64_t id) {
        player_id_ = id;
    }


//EPOLLSvr
    EPOLLSvr::EPOLLSvr(){
        stop_ = false;
        now_connections_ = 0;
        max_connections_ = 1000;   //默认最大连接数量1000
        time_out_ = 30;            //默认30秒超时
        sock_handler_ = nullptr;
        memset(ev_ids_, 0, sizeof(ev_ids_));
        memset(buff_, 0, sizeof(buff_));
    }
    EPOLLSvr::~EPOLLSvr() {

        close(epoll_fd_);
    }

    bool EPOLLSvr::Init(uint16_t port, int32_t max_connection,  On_Accept_Handler h1, On_Socket_Handler h2, int32_t timeout, int32_t window, bool nodelay) {
        epoll_fd_ = epoll_create(max_connection);
        if(NetPackage::kINVALID_FD == epoll_fd_){
            LOG(FATAL) << "epoll create fail";
            return false;
        }
        
        max_connections_ = max_connection;
        time_out_ = timeout;
        LOG(INFO) << "最大连接数量[" << max_connection << "]";
        auto acceptor = std::make_shared<TCPAccept>(port, shared_from_this()); 
        acceptor.get()->SetHandler(h1);
        acceptor.get()->Listen();
        sock_handler_ = h2;
        
        // 疑问:可不可以先注册事件再listen, epoll 不不会出错?
        return RegEvent(EPOLL_CTL_ADD, EPOLLIN,  acceptor);

    }
    
    //op , option, 是EPOLL_CTL_ADD 或 EPOLL_CTL_MOD 或  EPOLL_CTL_DEL
    //event, 监听的事件类型, 是 EPOLLIN , 或 EPOLLOUT
    bool EPOLLSvr::RegEvent(int32_t op, int32_t event, IPlayerPtr player){
        auto tcpev = std::make_shared<TCPEvent>();
        auto pl = player.get();        
        auto ev = tcpev.get();        
        
        ev->SetPlayer(pl->getID());
        ev->ev_.events = event;
        ev->ev_.data.ptr= ev;
        ev->fd_ = pl->peer_.fd_;
        events_map_[pl->getID()] = tcpev;
        player_map_[pl->getID()] = player;
        
        if( 0 != epoll_ctl(epoll_fd_, op, ev->fd_, &(ev->ev_))) {
            LOG(ERROR) << "epoll_ctl fail fd[" << ev->fd_ << "]"; 
            return false;
        }
        return true;

    }

    bool EPOLLSvr::AddConnection(IPlayerPtr player){
        if(nullptr == player.get()){
            LOG(ERROR) << "player is nullptr";
            return false;
        }
        if( now_connections_ >= max_connections_ ){
            LOG(WARNING) << "too many connections now[" << now_connections_ << "] max[" << max_connections_ << "]";
            //TODO 还应该将 player关闭掉 
            player.get()->KickOut();            
            return false;
        }
        
        ++now_connections_;
        if(!RegEvent(EPOLL_CTL_ADD, EPOLLIN , player)){
            LOG(ERROR) << "regevent fail";
            return false;
        }        
        player.get()->SetHandler(sock_handler_);
        player_map_[player.get()->getID()] = player;
        
        return true;
        
    }
    bool EPOLLSvr::Start() {
        //返回事件的数量
        int32_t c = 0;
        while(!stop_) {
            c = this->Wait();
            this->Svc(c);
        }

    }

    bool EPOLLSvr::SendMessage(IPlayerPtr player, void* msg, int32_t sz) {
        //TODO 添加发送缓冲区, 异步发送
        //先阻塞发送
        MSG *pMsg = (MSG* )buff_;
        pMsg->header.version_ = 1000;
        pMsg->header.size_ = sz;
        pMsg->header.serial_ = 0;
        pMsg->header.reserve_ = 0;
        memcpy(pMsg->body, msg, sz);
        int32_t sendcnt = 0;        
        int32_t leftcnt = sz;        
        while(leftcnt > 0){
            sendcnt = NetPackage::Write(player.get()->peer_.fd_, buff_, sz);
            if(sendcnt < 0){
                if( EAGAIN == errno || EWOULDBLOCK == errno ){
                    continue;
                }else{
                    return false;
                }
            }            
            leftcnt -= sendcnt;       
        }
        return true;
    }
    bool EPOLLSvr::Connect(std::string dest, bool reconnect) {

    }

    int32_t EPOLLSvr::Svc(int32_t c) {        
        for(int i = 0; i < c; ++i) {
            TCPEventPtr e = events_map_[ev_ids_[i]];
            if( nullptr == e.get() ){
                LOG(ERROR) << "event is nullptr";
                return -1;
            }
            player_map_[e.get()->player_id_]->OnNetMessage();

        }
        return 0;
        
    }

    //毫秒级别的timeout
    int32_t EPOLLSvr::Wait() {
        //TODO add timer here
        
        int32_t ret = epoll_wait(epoll_fd_, events_, MAX_EVENTS, -1);
        if( 0 == ret ){
            //time out
            return 0;
        }

        if( -1 == ret ){
            if( EINTR == errno ){
                LOG(INFO) << "eintr happen" ;
                return 0;
            }else{
                return ret;
            }           
        }

        for(int c = 0; c < ret; ++c) {
            TCPEvent* e = (TCPEvent*) events_[c].data.ptr;
            e->Proc();
            ev_ids_[c] = e->player_id_;

        }
        return ret;

    }

//TCPSocket 已连接的服务器
    TCPSocket::TCPSocket(EPOLLSvrPtr s) {
        socket_handler_ = nullptr;
        svr_ = s;
        msg_ = nullptr;
        step_ = READ_HEAD;
    }
    void TCPSocket::SetHandler(On_Socket_Handler h){
        socket_handler_ = h;
        
    }
   
    void TCPSocket::OnNetMessage(){      
        char *buff = svr_.get()->buff_;
        for(;;){
            //一直读, 直到出错 
            int32_t ret = NetPackage::Read(peer_.fd_, buff, MAX_SOCK_BUFF);
            if(0 == ret ){     
                LOG(INFO) << "connection closed by peer fd[" << peer_.fd_ << "]";
                this->KickOut();
                return;
            }

            if( ret < 0 ){
                if( EAGAIN  == errno || EWOULDBLOCK == errno ){
                    //再次read, buff将从头开始填充
                    buff = svr_.get()->buff_;  
                    continue;

                }else{   
                    LOG(INFO) << "read fail! fd[" << peer_.fd_ << "] errno[" << errno << "] msg[" << strerror(errno) << "]";
                    this->KickOut();  
                    return;
                }

            }


            int32_t more_data = ret;

            while( more_data > 0){
                if( nullptr == peer_.buff_.get() ){  
                    peer_.buff_ = std::make_shared<DataBuffer>(peer_.fd_, HEADER_SZ);      
                }

                auto data_buffer = peer_.buff_.get();
               
                int32_t need_data = data_buffer->NeedData();

                //读取包头
                if( READ_HEAD == step_ ){
                    if( more_data < need_data ) {
                        //包头没有读完整
                        data_buffer->AddData(more_data, buff);
                        return;
                    }
                    data_buffer->AddData(need_data, buff);  
                    //指向body的头指针, 向前添加已经读过的内存
                    buff += need_data;
                    more_data = (more_data - need_data) < 0 ? 0:(more_data - need_data);

                    msg_ = (MSG* )data_buffer->GetBuffPtr();

                    //为body 申请内存
                    data_buffer->Resize(msg_->header.size_ + HEADER_SZ);  
                    //重新申请内存后, 以前的msg_指向的内容不能再使用了
                    msg_ = (MSG* )data_buffer->GetBuffPtr();
                    need_data = data_buffer->NeedData();                

                    step_ = READ_BODY;            
                }

                //现在的step 肯定是 READ_BODY
                if( more_data > 0 ) {
                    //读取body
                    if(more_data < need_data) {
                        data_buffer->AddData(more_data, buff);
                        return;
                    }

                    data_buffer->AddData(need_data, buff);
                    more_data = (more_data - need_data) < 0 ? 0:(more_data - need_data);
                    //buff读取后指针后移
                    buff += need_data;

                    auto f = socket_handler_;
                    //客户程序只需要截获到数据信息就行, 不用关心包头                    
                    f(this->getID(), data_buffer->GetBuffPtr() + sizeof(HEADER),msg_->header.size_);
                    //自动删除已经用过的packet
                    auto tmp = std::move(peer_.buff_);
                    peer_.buff_ = nullptr;//是不是多此一举
                    //读取新的包头
                    step_ = READ_HEAD;

                }
            }   
        }
    }
    void TCPSocket::KickOut() {
        IPlayer::KickOut();
        //TODO 需要再 EPOLLSvr 的map 中删除事件events_map_ 和连接信息 player_map_
    }

//TCPAccept
    TCPAccept::TCPAccept(int32_t port, EPOLLSvrPtr s) {
        peer_.port_ = port;
        accept_handler_ = nullptr;
        svr_ = s;
    }
    void TCPAccept::SetHandler(On_Accept_Handler h){
        accept_handler_ = h;
        
    }
    bool TCPAccept::Listen(bool reuse) {
        if(!NetPackage::Listen(&peer_.fd_, "0.0.0.0", peer_.port_, reuse)){
            LOG(FATAL) << "Listen failed on port " << peer_.port_ ;
            return false;
        }
        LOG(INFO) << "Listen success on port [" << peer_.port_ << "] fd[" << peer_.fd_ << "]";
        return true;
    }
    TCPAccept::~TCPAccept() {
    }
    void TCPAccept::OnNetMessage() {
        int32_t cliFD = NetPackage::kINVALID_FD;
        std::string ip = "";
        uint16_t port = 0;
        bool ret = false;
        //do{ //ET 模式
            ret = NetPackage::Accept( peer_.fd_, &cliFD, &ip, &port);
            if(false == ret){
                // 可能会有问题, 当只是发生了 eintr, EAGAIN, EWOULDBLOCK 或其他错误时, 还有没接收的连接
                //LOG(INFO) << "accept fail" ;
                return;
            }             

            TCPSocketPtr sock = std::make_shared<TCPSocket>(this->svr_.get()->shared_from_this());
            auto client = sock.get();
            client->peer_.addr_ = std::move(ip);
            client->peer_.fd_ = cliFD;
            client->peer_.port_ = port;   

            NetPackage::SetNonBlock(cliFD);
            NetPackage::SetNoDelay(cliFD);
            NetPackage::SetReuse(cliFD);
       
            svr_.get()->AddConnection(sock);            
            auto f = accept_handler_;
            f(client->getID());
        //}while(ret);
        
    }
