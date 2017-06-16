/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   net_ease.h
 * Author: admin
 *
 * Created on 2017年6月15日, 上午11:16
 */

#ifndef NET_EASE_H
#define NET_EASE_H

#include <memory>
#include <cstdlib>
#include "event_loop.h"
#include "socket_buffer.h"
#include "config_loader.h"
#include "receiver.h"

#include "iplayer.h"

using namespace easynet;
class NetEase{
public:
    NetEase();
    ~NetEase();
public:
    
public:
    void init();    
    void start();
    TcpSocketPtr connect_server(const std::string& name);
    void on_accept(int32_t id,  TcpSocketPtr ts, NetErrorCode& code);
    void on_receive(int32_t id,void* buff, int32_t sz,  NetErrorCode& code);
    
protected:
    //使用者的连接用户
    virtual IPlayerPtr create_player(TcpSocketPtr s);
private:
    ConfigLoader::ServerInfoPtr config_;
    EventLoopPtr event_loop_;
    Receiver receiver_;
    
    
};


#endif /* NET_EASE_H */

