/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GameFrame.h
 * Author: admin
 *
 * Created on 2017年5月24日, 下午7:23
 */

#ifndef GAMEFRAME_H
#define GAMEFRAME_H

#include <map>
#include <memory>
#include <iterator> 
#include "epoll_svr.h"
#include "config_loader.h"
#include "game_player.h"
#include "game_message.h"
#include "log.h"
using namespace easynet;
//游戏框架

class GameMessage;
using GameMessagePtr = std::shared_ptr<GameMessage>;
 
class GamePlayer;
using GamePlayerPtr = std::shared_ptr<GamePlayer>;

using PLAYER_MAP = std::map<uint64_t, GamePlayerPtr>;
using PLAYER_MAP_ITER = std::map<uint64_t, GamePlayerPtr>::iterator;
using MESSAGE_MAP = std::map<int32_t, GameMessagePtr>;
using MESSAGE_MAP_ITER = std::map<int32_t, GameMessagePtr>::iterator;


class GameFrame
: public std::enable_shared_from_this<GameFrame>{
public:
    
    GameFrame();
    /*
        using On_Accept_Handler = std::function<void(uint64_t)>;
        using On_Socket_Handler = std::function<void(uint64_t, const void*, int32_t, int32_t)>;
        using On_Reconnect_Handler = std::function<void()>;    
     */
    //初始化服务器
    //port 端口号, max_connection 最大连接数, 当连接到来时的绑定函数, 
    //timeout 超时多少秒删除连接, window 接收发送窗口, 默认2048, nodelay 不延迟, 默认true
    bool init(uint16_t port, int32_t max_connection, 
              int32_t timeout = 30,int32_t window = 2048,  bool nodelay = true);  
    
    
    //连接服务器
    bool connect_to();
    //发送消息
    bool send_msg(uint64_t id, GameMessagePtr msg);
    //定时器
    bool timer();
    
    //应该另外建一个类管理玩家
    GamePlayerPtr find_player(uint64_t id);
    void add_player(uint64_t id, GamePlayerPtr player);
    
    
    //注册事件, 客户端消息到来时处理
    bool reg_event(GameMessagePtr ptr);    
    void proc_message(GamePlayerPtr player, int32_t type, const void* data, int32_t sz);
private:
    
private:
    EPOLLSvrPtr epoll_svr_;    
    //所有玩家
    PLAYER_MAP  player_map_;
    //消息处理接口
    MESSAGE_MAP message_map_;
    
    
    
    
    
    
    
    
    
};
using GameFramePtr = shared_ptr<GameFrame>;

#endif /* GAMEFRAME_H */

