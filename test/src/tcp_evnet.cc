/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "tcp_event.h"
using namespace easynet;
//TCPEvent
    TCPEvent::TCPEvent() {        
        read_able_ = false;
        write_able_ = false;
        fd_ = NetPackage::kINVALID_FD;
        memset(&ev_, 0, sizeof(ev_));
    }
    void TCPEvent::Proc() {
        int32_t event = ev_.events;
        if( event & EPOLLERR || event & EPOLLHUP){
            LOG(ERROR) << "epoll error or hup";
            return;
        }
        if( event & EPOLLIN ){
            read_able_ = true;
        }
        if( event & EPOLLOUT ){
            write_able_ = true;
        }
        // TODO 需要处理 EPOLLERR, 
        return;

    }
   
    void TCPEvent::SetPlayer(uint64_t id) {
        player_id_ = id;
    }
