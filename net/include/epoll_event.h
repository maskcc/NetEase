/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   epoll_event.h
 * Author: admin
 *
 * Created on 2017年6月8日, 下午1:58
 */

#ifndef EPOLL_EVENT_H
#define EPOLL_EVENT_H

#include "svr_define.h"
#include "event_data.h"
namespace easynet{
class EpollEvent{
public:
    bool add_in_event(int32_t efd, EventData& ev);
    bool add_out_event(int32_t efd, EventData& ev);
    bool add_inout_event(int32_t efd, EventData& ev);
    bool remove_in_event(int32_t efd, EventData& ev);
    bool remove_out_event(int32_t efd, EventData& ev);
    bool remove_inout_event(int32_t efd, EventData& ev);
    bool add_event(int32_t efd, int32_t option, int32_t event, EventData& data);
    
};
}
#endif /* EPOLL_EVENT_H */

