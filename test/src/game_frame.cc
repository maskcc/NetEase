/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */



#include <bits/stl_map.h>

#include "game_frame.h"

GameFrame::GameFrame() {
    epoll_svr_ = std::make_shared<EPOLLSvr>();
}

bool GameFrame::init(uint16_t port, int32_t max_connection, On_Accept_Handler h1, On_Socket_Handler h2,
        int32_t timeout, int32_t window, bool nodelay) {

    auto svr = epoll_svr_.get();

    auto on_accept = [this](uint64_t id) {
        //加入玩家应该在登录消息中处理
        auto pl = this->find_player(id);
        if (nullptr == pl) {
            //没找到玩家
            pl = std::make_shared<GamePlayer>();
            this->add_player(id, pl);
            return;
        }
        //一般不会走到这来, 每连接一个客户端id会增加
        //每个客户端应该有个独立于连接id的用户id
        pl->on_reconnect();
    };
    auto on_data = [this](uint64_t id, const void* data, int32_t sz, int32_t type) {




    };

    svr->Init(port, max_connection, on_accept, on_data, timeout, window, nodelay);
}


//连接服务器

bool GameFrame::connect_to() {
    return true;
}
//发送消息

bool GameFrame::send_msg() {
    return true;
}
//定时器

bool GameFrame::timer() {
    return true;
}

GamePlayerPtr GameFrame::find_player(uint64_t id) {
    auto pl = player_map_.find(id);
    if (pl != player_map_.end()) {
        return pl->second;
    }
    //没有找到玩家
    return nullptr;
}

void GameFrame::add_player(uint64_t id, GamePlayerPtr player) {
    player_map_.insert(make_pair(id, player));
}

bool GameFrame::reg_event(GameMessagePtr ptr) {
    auto msg = ptr.get();
    auto mm = message_map_.find(msg->type());
    if (mm != message_map_.end()) {
        //LOG WARN 有消息重复
        message_map_.erase(mm, std::next(mm));
    }
    //注册消息成功
    message_map_.insert(make_pair(msg->type(), ptr));
    
    return true;
}

void GameFrame::proc_message(int32_t type, const void* data, int32_t sz) {

}