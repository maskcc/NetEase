#ifndef  TEST_EPOLLSVR_H_
#define  TEST_EPOLLSVR_H_

#include "svr_define.h"
#include "iplayer.h"
#include "monitor.h"
#include "socket.h"
#include "net_package.h"
#include "tcp_event.h"
#include "tcp_socket.h"
#include "tcp_accept.h"
#include "tcp_connector.h"
#include "timer.h"


//EPOLLSvr
    /*
     
     */
namespace easynet{    
    class TCPConnector;
    using TCPConnectorPtr = std::shared_ptr<TCPConnector>;
    
    class EPOLLSvr
        : public std::enable_shared_from_this<EPOLLSvr>
    {
        public:
            ~EPOLLSvr();
            EPOLLSvr();
            //初始化
            //port 端口号, max_connection 最大连接数, 当连接到来时的绑定函数, 
            //timeout 超时多少秒删除连接, window 接收发送窗口, 默认2048, nodelay 不延迟, 默认true
            bool Init(uint16_t port, int32_t max_connection, On_Accept_Handler h1, On_Socket_Handler h2,
                        On_Timer_Handler h3,int32_t timeout = 30,int32_t window = 2048,  bool nodelay = true);  
            bool Start();
            bool SendMessage(uint64_t id, const void *msg, int32_t sz, int32_t type);
            TCPConnectorPtr Connect(std::string dest, int32_t port, bool reconnect); //是否重连
            int32_t Wait();
            int32_t Svc(int32_t c);
            
            //op , option, 是EPOLL_CTL_ADD 或 EPOLL_CTL_MOD 或  EPOLL_CTL_DEL
            //event, 监听的事件类型, 是 EPOLLIN , 或 EPOLLOUT
            bool RegEvent(int32_t op, int32_t event, IPlayerPtr player);
            bool AddConnection(IPlayerPtr player);
        public:
            char buff_[MAX_SOCK_BUFF];      //接收网络消息的最大长度为 64K, 不用清零, 使用时直接覆盖
            int32_t epoll_fd_; //epoll fd, 用来监听发送接收消息的
        private:
            
            int32_t  now_connections_;       //当前的连接数量
            int32_t max_connections_;       //最大连接数量
            int32_t time_out_;              //超时, 过多久服务器客户端没有消息断开连接
            std::map<uint64_t, IPlayerPtr> player_map_; //所有连接信息都在这
            std::map<uint64_t, TCPEventPtr> events_map_; //保存消息的地图, key是指向事件的指针地址
            Timer timer_;
            EPOLL_EV events_[MAX_EVENTS];   //缓存epoll_wait 的事件
            uint64_t ev_ids_[MAX_EVENTS];   //记录事件的id            
            bool stop_;                     //是否已经停止, 用来从外部关服
            On_Socket_Handler  sock_handler_;//处理收发消息的handler
            On_Timer_Handler   timer_handler_;//定时器
            

    };
   

}


#endif // TEST_EPOLLSVR_H_

