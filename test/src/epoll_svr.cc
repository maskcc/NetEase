#include "epoll_svr.h"
using namespace easynet;
//统计信息

MONITOR MONITOR_SVR;
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

    bool EPOLLSvr::SendMessage(IPlayerPtr player, const void* msg, int32_t sz) {
        //TODO 添加发送缓冲区, 异步发送
        //先阻塞发送
        char *pPos = buff_;
        MSG *pMsg = (MSG* )buff_;
        pMsg->header.version_ = VERSION;
        pMsg->header.size_ = sz;
        pMsg->header.serial_ = 0;
        pMsg->header.reserve_ = 0;
        memcpy(pPos + sizeof(HEADER), msg, sz);
        int32_t sendcnt = 0;        
        int32_t leftcnt = sz;        
        while(leftcnt > 0){
            sendcnt = NetPackage::Write(player.get()->peer_.fd_, pPos, leftcnt);
            if(sendcnt < 0){
                if( EAGAIN == errno || EWOULDBLOCK == errno ){
                    continue;
                }else{
                    return false;
                }
            }            
            leftcnt -= sendcnt;   
            pPos += sendcnt;
        }
        return true;
    }
    TCPConnectorPtr EPOLLSvr::Connect(std::string dest, int32_t port, bool reconnect) {
        TCPConnectorPtr conn = std::make_shared<TCPConnector>(shared_from_this());
        
        auto sock = conn.get()->socket_;
        auto c = sock.get();
        
        c->peer_.addr_ = dest;
        c->peer_.port_ = port;
        conn.get()->Connect();
        
        this->AddConnection(sock);
        return conn;
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
