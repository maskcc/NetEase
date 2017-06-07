/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   event_data.h
 * Author: admin
 *
 * Created on 2017年6月7日, 下午2:59
 */

#ifndef EVENT_DATA_H
#define EVENT_DATA_H
#include "svr_define.h"
namespace easynet{
     enum LINK_STATUS{
        LS_NONE,         //什么都没做
        LS_CONNECTING,   //正在连接
        LS_ESTABLISHED,  //已经连接 
        LS_RECONNECTING, //正在尝试重连
        LS_CLOSING,      //正在关闭
        LS_CLOSED        //已经关闭
    };
    
    enum LINK_TYPE{
        LT_NONE,
        LT_LOCAL_SERVICE,    //服务器主动监听的端口 
        LT_REMOTE_SERVICE,   //服务器主动连接的远程服务
        LT_CLIENT            //连上来的客户端
        
    };
class EventData{
public:
    EventData();
    ~EventData();
    //acceptor
    //tcpsocket
    //buffer
        
public:
    epoll_event epoll_event_;  //事件
    LINK_TYPE   link_type_;    //连接类型
    LINK_STATUS status_;       //连接状态    
    int32_t     fd_;           //连接fd
    std::string name_;          //连接名称, 监听的有名称, 连接的没有名称
        
};
    
    
    
}


#endif /* EVENT_DATA_H */

