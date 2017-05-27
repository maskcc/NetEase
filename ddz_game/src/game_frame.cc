/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "game_frame.h"
#include "ddz_server.h"

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
    auto on_timeout = [this](){
      this->timer();
    };
    this->reg_event();
    svr->Init(port, max_connection, on_accept, on_data, on_timeout, timeout, window, nodelay);
    svr->Start();
    
}


//连接服务器
//TODO, 应该返回的是一个GamePlayer, 还需要修改
TCPConnectorPtr GameFrame::connect_to(std::string dest, int32_t port, bool reconnect ) {
    TCPConnectorPtr conn =  epoll_svr_.get()->Connect(dest, port, reconnect); //是否重连
    
    return conn;
}
//发送消息

bool GameFrame::send_msg(uint64_t id, GameMessagePtr msg) {
    auto svr = epoll_svr_.get();
    std:;string data = msg.get()->encode();
    return svr->SendMessage(id, data.c_str(), data.size(), msg.get()->type());
}
//定时器



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