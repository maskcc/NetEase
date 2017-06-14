/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "tcp_accept.h"
#include "event_loop.h"

using namespace easynet;

TcpAccept::TcpAccept(EventLoopPtr loop):event_loop_(loop){
    
}
void TcpAccept::on_message(int32_t events){
    if( LS_LISTING != event_data_.status_ ){
        LOG(ERROR) << "status not right! current:" <<  event_data_.status_;
        return;
    }    
    int32_t fd = 0;
    std::string ip;
    uint16_t port = 0;
    int64_t id = next_id();
    if( false == NetPackage::Accept(event_data_.fd_, fd, ip, port) ){
        LOG(ERROR) << "accept client fail, fd:" << event_data_.fd_;
        return;        
    }
    TcpSocketPtr s = make_shared<TcpSocket>(event_loop_);
    s->attach(id, fd, ip, port);
    LOG(INFO) << "accept ok id:" << id << " fd:" << fd << " remote ip:" << ip << " port:" << port;
    NetErrorCode error;
    on_accept_(id, s, error);
    NET_WORK_STATUS.add_online_count();
    
    
    
}
bool TcpAccept::do_accept(int32_t port, OnAcceptHandler h){
    event_data_.acceptor_ = shared_from_this();
    if ( true == NetPackage::Listen(&event_data_.fd_, "0.0.0.0", port, true)){
        LOG(INFO) << "Listen on port:" << port << " success";
    }else{
        LOG(ERROR) << "Listen on port:" << port << " fail, err:" << strerror(errno);
        return false;
    }
    event_data_.link_type_ = LT_SERVICE;
    event_data_.status_ = LS_LISTING;
    event_data_.epoll_event_.data.ptr = &event_data_;
    if ( false == event_loop_->ev_control_.add_in_event(event_loop_->get_efd(), event_data_)){
        LOG(ERROR) << "add event fail!";
        NetPackage::Close(event_data_.fd_);
        event_data_.status_ = LS_CLOSED;
        return false;
    }
    on_accept_ = std::move(h);
    return true;
}