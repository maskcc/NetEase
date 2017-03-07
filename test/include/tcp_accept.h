/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tcp_accept.h
 * Author: admin
 *
 * Created on 2017年3月7日, 上午11:25
 */

#ifndef TCP_ACCEPT_H
#define TCP_ACCEPT_H
#include "svr_define.h"
#include "iplayer.h"
#include "net_package.h"
#include "epoll_svr.h"
namespace easynet{
//监听服务器
    class EPOLLSvr;
    using EPOLLSvrPtr = std::shared_ptr<EPOLLSvr>;
    class TCPAccept : public IPlayer
    {
        public:
            TCPAccept(int32_t port, EPOLLSvrPtr s);
            bool Listen(bool reuse = true);
            virtual void OnNetMessage();
            ~TCPAccept();
            
            virtual void SetHandler(On_Accept_Handler h);
        private:            
            On_Accept_Handler accept_handler_;  //接收事件后的回调函数
            EPOLLSvrPtr svr_;                    //epoll svr
            


    };
    using TCPAcceptPtr = std::shared_ptr<TCPAccept>;

}

#endif /* TCP_ACCEPT_H */

