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

namespace easynet{    
class EventLoop{
public:
    EventLoop();
    ~EventLoop();
public:
    // 初始化
    bool initialize();
    bool run_once(int timeout);
    
protected:
    
    
    
private:
    int32_t efd_;
    int32_t sock_pair_[2];
    EventData event_data_;
    
};

}
#endif /* EVENT_LOOP_H */

