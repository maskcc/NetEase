/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   socket.h
 * Author: admin
 *
 * Created on 2017年3月7日, 上午11:22
 */

#ifndef SOCKET_H
#define SOCKET_H
#include "svr_define.h"
#include "data_buffer.h"
#include "net_package.h"
#include "tcp_event.h"
namespace easynet{
//连接信息 Socket
    class DataBuffer;
    class Socket
    {
        public:
            Socket();
            bool Send();    //发送消息
            bool Receive(); //接收消息
        public:
            std::string  addr_;  //地址
            int32_t fd_;        //连接的fd
            int32_t port_;      //监听的端口号
            int64_t last_time_; //上次通信的时间戳, 用来踢掉长时间不通讯的连接
            DataBufferPtr buff_;
            
    };
    using SocketPtr = std::shared_ptr<Socket>;
}


#endif /* SOCKET_H */

