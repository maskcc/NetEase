/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "tcp_socket.h"

using namespace easynet;

TcpSocket::TcpSocket(EventLoopPtr loop):event_loop_(loop){
    
    
}

TcpSocket::~TcpSocket(){
    
}

void TcpSocket::on_message(int32_t events){
    
    
}

bool TcpSocket::do_receive(OnReceiveHandler h){
    if ( false == event_loop_->ev_control_.add_in_event(event_loop_->get_efd(), event_data_)){
        LOG(ERROR) << "add receive event fail!";
        NetPackage::Close(event_data_.fd_);
        event_data_.status_ = LS_CLOSED;
        return false;
    }
    on_receive_ = std::move(h);
    
    return true;
     
    
}
void TcpSocket::attach(uint64_t id, int32_t fd, std::string ip, int32_t port){
    ip_ = std::move(ip);
    port_ = port;
    identify_ = id;
    event_data_.fd_ = fd;
    event_data_.tcpsocket_ = shared_from_this();
    event_data_.link_type_ = LT_CLIENT;
    event_data_.status_ = LS_ESTABLISHED; 
    event_data_.epoll_event_.data.ptr = &event_data_;
}
