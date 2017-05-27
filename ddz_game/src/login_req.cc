/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "login_req.h"
void LoginReq::proc(GamePlayerPtr player) {
    LOG(INFO) << "name:" << this->name_;
    LOG(INFO) << "passwd_:" << this->passwd_;
    LOG(INFO) << "plat_:" << this->plat_;
    
    static_rsp_.get()->err_ = 0;
    static_rsp_.get()->gender_ = 1;
    static_rsp_.get()->id_ = 99l;
    static_rsp_.get()->sign_ = "ggpp";
    frame_.get()->send_msg(player.get()->id_, static_rsp_);

}
