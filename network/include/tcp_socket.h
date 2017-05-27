/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tcp_socket.h
 * Author: admin
 *
 * Created on 2017年3月7日, 上午11:24
 */

#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H
//#pragma once
#include "svr_define.h"
#include "epoll_svr.h"
#include "monitor.h"
namespace easynet{
//已连接的服务器
    class EPOLLSvr;
    using EPOLLSvrPtr = std::shared_ptr<EPOLLSvr>;

    class TCPSocket : public IPlayer 
    {
        public:
            TCPSocket(EPOLLSvrPtr s);
            virtual void SetHandler(On_Socket_Handler h);
            virtual void OnNetMessage();
            virtual void KickOut();
            MONITOR m;
        private:
            MSG    *msg_;  //当前消息
            TCP_STEP step_; //当前进度
            bool connected_;
            
            On_Socket_Handler socket_handler_;
            EPOLLSvrPtr svr_;                    //epoll svr
    };
    using TCPSocketPtr = std::shared_ptr<TCPSocket>;
    
}
#endif /* TCP_SOCKET_H */

