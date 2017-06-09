/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TcpSocket.h
 * Author: admin
 *
 * Created on 2017年6月7日, 下午5:40
 */

#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "event_loop.h"
#include "event_data.h"
namespace easynet{
class TcpSocket  : public enable_shared_from_this<TcpSocket>{
public:
    TcpSocket(EventLoopPtr loop);
    ~TcpSocket();
    
public:
    void on_message(int32_t events){};
    bool do_connect(){}
    bool do_receive(OnReceiveHandler h);
    void attach(uint64_t id, int32_t fd, std::string ip, int32_t port);
    
private:
    std::string ip_;
    int32_t port_;
    uint64_t identify_;
protected:
    EventData event_data_;
    EventLoopPtr event_loop_;
    OnReceiveHandler on_receive_;
    
};


}

#endif /* TCPSOCKET_H */

