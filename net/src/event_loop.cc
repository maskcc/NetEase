/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "event_loop.h"
#include "epoll_event.h"
using namespace easynet;
    
EventLoop::EventLoop(){
    buffer_ = (uint8_t *) malloc(1024* 4);
}
EventLoop::~EventLoop(){
    close(efd_);
    free(buffer_);
}

bool EventLoop::initialize(){
    efd_ = epoll_create(MAX_EFD);
    if(NetPackage::kINVALID_FD == efd_){
        LOG(FATAL) << "epoll create fail";
        return false;
    }
    if (socketpair(AF_LOCAL, SOCK_STREAM, 0, sock_pair_) != 0){
        LOG(FATAL) << "init local sock pari fail";
        return false;        
    }
    NetPackage::SetNonBlock(sock_pair_[0]);
    NetPackage::SetNonBlock(sock_pair_[1]);
    NetPackage::SetNoDelay(sock_pair_[0]);
    NetPackage::SetNoDelay(sock_pair_[1]);
    event_data_.fd_ = sock_pair_[1];
    event_data_.link_type_ = LT_SERVICE;  
    event_data_.status_ = LS_ESTABLISHED;
    event_data_.epoll_event_.data.ptr = &event_data_;
    
    ev_control_.add_in_event(efd_, event_data_);
    
    
}

void EventLoop::run_once(int timeout){
    int32_t count = epoll_wait(efd_, events_, MAX_EVENTS, timeout);
    if( -1 == count ){
        if( EINTR == errno ){
            LOG(INFO) << "eintr happen" ;
            return;
        }
    }    
    //time out 
    {
        LOG(INFO) << "timeout";
        if( 0 == count ){
            return;
        }
    }
    for( int32_t c = 0; c < count; ++c ){
        EventData *event = (EventData*)( events_[c].data.ptr );
        if( event->link_type_ == LT_SERVICE && event->acceptor_){
            event->acceptor_->on_message(event->epoll_event_.events);
        }else if( event->link_type_ == LT_CLIENT && event->tcpsocket_){
            event->tcpsocket_->on_message(event->epoll_event_.events);
        }else{
            LOG(ERROR) << "unknown link type type:" << event->link_type_;
        }        
        
    }
    
}

