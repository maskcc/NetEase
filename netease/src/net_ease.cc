/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "net_ease.h"

NetEase::NetEase(){
    config_ = ConfigLoader::load_server_config();
    
}

NetEase::~NetEase(){
    
}


void NetEase::start(){
    init();
    while(true){
        event_loop_->run_once(2*1000);
    }
}

void NetEase::init(){
    event_loop_ = make_shared<EventLoop>();
    if( false == event_loop_->initialize() ){
        LOG(FATAL) << "init event loop fail!";
        return;
    }
    TcpAcceptPtr acc = make_shared<TcpAccept>(event_loop_);
    auto acc_handler = [this](int32_t fd,  TcpSocketPtr ts, NetErrorCode& code){
        this->on_accept(fd, ts, code);
    };
    acc->do_accept(config_->config_.port_, acc_handler);
}

TcpSocketPtr NetEase::connect_server(const std::string& name){
    ConfigLoader::ServerConfig* conf = config_->find_svr(name);
    if( nullptr == conf) {
        LOG(ERROR) << "load config fail, name:" << name;
        return nullptr;
    }
    if( nullptr == event_loop_ ){
        LOG(FATAL) << "event loop is null ptr, please init before this!";
        return nullptr;
    }
    
    TcpSocketPtr tc = make_shared<TcpSocket>(event_loop_);
    if ( false == tc->do_connect(conf->ip_, conf->port_) ){
        LOG(ERROR) << "connect ip:" << conf->ip_ << " port:" << conf->port_ << " fail";
        return nullptr;
    }
    auto receive = [this](int32_t id,void* buff, int32_t sz,  NetErrorCode& code){
        this->on_receive(id, buff, sz, code);
    };
    tc->do_receive(std::move(receive));
    return tc;
    
    
}
void NetEase::on_accept(int32_t id,  TcpSocketPtr sock, NetErrorCode&){
    if( nullptr == sock ){
        LOG(ERROR) << "sock is nullptr";
    }
    auto receive = [this](int32_t id,void* buff, int32_t sz,  NetErrorCode& code){
        this->on_receive(id, buff, sz, code);
    };    
    
    sock->do_receive(receive);
    IPlayerPtr player = create_player(sock);
    
}
void NetEase::on_receive(int32_t id,void* buff, int32_t sz,  NetErrorCode& code){
    if( code.err_ == NetErrorCode::NE_CLOSED ){
        
        return;
    }
    if( code.err_ == NetErrorCode::NE_ERROR ){
        
        return;
    }
     if( code.err_ == NetErrorCode::NE_DATA ){
        receiver_.on_receive(id, buff, sz);
        return;
    }
    
    
}

IPlayerPtr NetEase::create_player(TcpSocketPtr s){
    return std::make_shared<IPlayer>(s);
}