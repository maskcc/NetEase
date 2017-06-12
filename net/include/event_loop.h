/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   event_loop.h
 * Author: admin
 *
 * Created on 2017年6月7日, 下午2:39
 */

#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H
#include "svr_define.h"
#include "event_data.h"
#include "epoll_event.h"
#include "tcp_accept.h"
#include "tcp_socket.h"

namespace easynet{    
 
class EventLoop{
public:
    EventLoop();
    ~EventLoop();
public:
    // 初始化
    bool initialize();
    void run_once(int timeout);
    void reg_event(const EventData& ev); 
    void* get_buffer(){
        return buffer_;
    }
    int32_t get_efd() const{
        return efd_;
    }

    //epoll 事件封装
    EpollEvent  ev_control_;
    
    
private:
    int32_t efd_;
    int32_t sock_pair_[2];
    EventData event_data_;
    epoll_event events_[MAX_EVENTS];   //缓存epoll_wait 的事件
    uint8_t *buffer_;
    
};

}
#endif /* EVENT_LOOP_H */

