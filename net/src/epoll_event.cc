/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "epoll_event.h"
using namespace easynet;

bool EpollEvent::add_in_event(int32_t efd, EventData& ev){
    return add_event(efd, EPOLL_CTL_ADD, EPOLLIN, ev);  
    
    
}


bool EpollEvent::add_out_event(int32_t efd, EventData& ev){
    return add_event(efd, EPOLL_CTL_ADD, EPOLLOUT, ev); 
    
    
}


bool EpollEvent::add_inout_event(int32_t efd, EventData& ev){
    return add_event(efd, EPOLL_CTL_ADD, EPOLLIN|EPOLLOUT, ev);   
    
    
}


bool EpollEvent::remove_in_event(int32_t efd, EventData& ev){
    return add_event(efd, EPOLL_CTL_DEL, EPOLLIN|EPOLLOUT, ev);  
    
    
}


bool EpollEvent::remove_out_event(int32_t efd, EventData& ev){
    return add_event(efd, EPOLL_CTL_DEL, EPOLLOUT, ev);  
    
}


bool EpollEvent::remove_inout_event(int32_t efd, EventData& ev){    
    return add_event(efd, EPOLL_CTL_DEL, EPOLLIN|EPOLLOUT, ev);    
    
}

//op , option, 是EPOLL_CTL_ADD 或 EPOLL_CTL_MOD 或  EPOLL_CTL_DEL
    //event, 监听的事件类型, 是 EPOLLIN , 或 EPOLLOUT
bool EpollEvent::add_event(int32_t efd, int32_t option, int32_t event, EventData& ev){
    ev.epoll_event_.events = event;
    if( 0 != epoll_ctl( efd, option, ev.fd_, &ev.epoll_event_) ){
        LOG(ERROR) << "epoll_ctl fail fd[" << ev.fd_ << "]"; 
        return false;        
    }
    return true;
    
}