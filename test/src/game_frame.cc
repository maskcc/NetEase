/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "game_frame.h"

GameFrame::GameFrame() {
    epoll_svr_ = std::make_shared<EPOLLSvr>();
}

bool GameFrame::init(uint16_t port, int32_t max_connection, int32_t timeout, int32_t window, bool nodelay) {

    auto svr = epoll_svr_.get();

    auto on_accept = [this](uint64_t id) {
        //加入玩家应该在登录消息中处理
        LOG(INFO) << "client connection come, id:" << id;
        auto pl = this->find_player(id);
        if (nullptr == pl) {
            //没找到玩家
            pl = std::make_shared<GamePlayer>();
            pl.get()->id_ = id;
            this->add_player(id, pl);
            return;
        }
        //一般不会走到这来, 每连接一个客户端id会增加
        //每个客户端应该有个独立于连接id的用户id
        pl->on_reconnect();
    };
    auto on_data = [this](uint64_t id, const void* data, int32_t sz, int32_t type) {
        auto player = this->find_player(id);
        if( nullptr == player ){
            return;
        }
        
        this->proc_message(player, type, data, sz);
    };
    GameMessagePtr login_req = make_shared<LoginReq>();
    login_req.get()->set_frame(shared_from_this());
    this->reg_event(login_req);
    svr->Init(port, max_connection, on_accept, on_data, timeout, window, nodelay);
    svr->Start();
    
}


//连接服务器

bool GameFrame::connect_to() {
    return true;
}
//发送消息

bool GameFrame::send_msg(uint64_t id, GameMessagePtr msg) {
    auto svr = epoll_svr_.get();
    std:;string data = msg.get()->encode();
    return svr->SendMessage(id, data.c_str(), data.size(), msg.get()->type());
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
        message_map_.erase(mm);
    }
    //注册消息成功
    message_map_.insert(make_pair(msg->type(), ptr));
    
    return true;
}

void GameFrame::proc_message(GamePlayerPtr player, int32_t type, const void* data, int32_t sz) {
    auto procptr = message_map_.find(type);
    if (procptr == message_map_.end()) {
        //LOG ERROR unkonw msg type
        return;
    }
    auto  proc = procptr->second.get();
    proc->decode(data, sz);
    proc->proc(player);
    
}