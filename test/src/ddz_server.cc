/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ddz_server.h"

using namespace easynet;
bool DDZServer::timer(){
    LOG(INFO) << "on timer!";
    
    return true;
}

void DDZServer::reg_event(){
    GameMessagePtr login_req = std::make_shared<LoginReq>();
    login_req.get()->set_frame(shared_from_this());
    GameFrame::reg_event(login_req);
    
}

void DDZServer::start(){
    
        this->init(30077, 1000, 1000);
}
