/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   iplayer.h
 * Author: admin
 *
 * Created on 2017年6月16日, 下午3:22
 */

#ifndef IPLAYER_H
#define IPLAYER_H
#include <memory>
#include "svr_define.h"
#include "event_loop.h"
#include "imsg.h"
using namespace easynet;
//连接上的玩家
class IPlayer{
public: 
    IPlayer(TcpSocketPtr);
    ~IPlayer();    
    bool send_msg(IMsgPtr msg);
    
private:
    TcpSocketPtr peer_;
    
};
using IPlayerPtr = std::shared_ptr<IPlayer>;
#endif /* IPLAYER_H */

