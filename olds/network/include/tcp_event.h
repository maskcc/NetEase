/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tcp_event.h
 * Author: admin
 *
 * Created on 2017年3月7日, 下午1:25
 */

#ifndef TCP_EVENT_H
#define TCP_EVENT_H
#include "svr_define.h"
#include "net_package.h"
namespace easynet{
//定义 epoll_event
using EPOLL_EV = struct epoll_event;
//epoll net event
    class TCPEvent
    {
        public:
            TCPEvent();
            void Proc();             //处理
            void SetPlayer(uint64_t ptr);
            
        public:            
            EPOLL_EV ev_;       //epoll 事件指针            
            uint64_t player_id_; //相关联的 player
            /**可以存储重复的数据*/
            int32_t fd_;       //监听的fd
            
        private:
            bool read_able_;    //可读
            bool write_able_;   //可写

    };
    using TCPEventPtr = std::shared_ptr<TCPEvent>;

}

#endif /* TCP_EVENT_H */

