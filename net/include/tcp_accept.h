/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TcpAccept.h
 * Author: admin
 *
 * Created on 2017年6月7日, 下午5:40
 */

#ifndef TCPACCEPT_H
#define TCPACCEPT_H
#include "svr_define.h"
#include "event_data.h"
namespace easynet{
class TcpAccept : public enable_shared_from_this<TcpAccept>{
public:
    TcpAccept(EventLoopPtr);
    bool do_accept(int32_t port, OnAcceptHandler h);
    void on_message(int32_t events);

private:
    EventLoopPtr event_loop_;
    EventData    event_data_;
    OnAcceptHandler on_accept_;
    
    
};


}


#endif /* TCPACCEPT_H */

