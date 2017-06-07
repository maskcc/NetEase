/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "event_loop.h"
namespace easynet{
    
EventLoop::EventLoop(){
    
}
EventLoop::~EventLoop(){
    close(efd_);
    
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
    
}

bool EventLoop::run_once(int timeout){
    
    
}


}