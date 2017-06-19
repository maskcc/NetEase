/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   session.h
 * Author: admin
 *
 * Created on 2017年6月19日, 上午10:15
 */

#ifndef SESSION_H
#define SESSION_H
#include <string>
#include <chrono>
#include <memory>

#include "iplayer.h"
using namespace std::chrono;
enum SessionState{
    SS_NONE,
    SS_CONNECTED,
    SS_DISCONNECTED
    
};
class Session{
public:
    Session();
    ~Session();
    //验证身份之后才能 set player
    void set(IPlayerPtr pl);
private:
    std::string session_; //session 信息
    IPlayerPtr player_;   //和session 绑定的玩家
    
    SessionState session_st_;   //是不是已经断线, 断线后定时删除节点
    time_point<system_clock> dis_conn_time_; //断线时间戳
    time_point<system_clock> conn_time_; //time_point_cast<days_type>(system_clock::now()) //conn_time_ = system+clock::now();
    
    
};//连接信息
using SessionPtr = std::shared_ptr<Session>;
#endif /* SESSION_H */

