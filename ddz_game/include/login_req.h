/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   login_req.h
 * Author: admin
 *
 * Created on 2017年5月27日, 上午10:42
 */

#ifndef LOGIN_REQ_H
#define LOGIN_REQ_H

#include "game_message.h"

class LoginRsp : public GameMessage {
public:

    MSGPACK_DECODE_DEF
    MSGPACK_ENCODE_DEF
    LoginRsp() {
        type_ = 0x0002;
    }

    void proc(GamePlayerPtr player) override {

    }
public:
    int32_t err_;
    int32_t id_;
    int32_t gender_;
    std::string sign_;

    MSGPACK_DEFINE(err_, id_, gender_, sign_);

};

class LoginReq : public GameMessage {

    MSGPACK_DECODE_DEF
    MSGPACK_ENCODE_DEF
public:
    LoginReq() {
        type_ = 0x0001;
    }
    void proc(GamePlayerPtr player) override;

    //定义数据结构
public:
    std::string name_;
    std::string passwd_;
    int32_t plat_;

    //注册消息
    MSGPACK_DEFINE(name_, passwd_, plat_);

};






#endif /* LOGIN_REQ_H */

