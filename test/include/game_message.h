/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   game_message.h
 * Author: admin
 *
 * Created on 2017年5月24日, 下午8:21
 */

#ifndef GAME_MESSAGE_H
#define GAME_MESSAGE_H
#include <stdint.h>
#include <string>
#include <memory>
#include <msgpack.hpp>
class GameMessage{
public:
    //获取消息类型
    int32_t type(){
        return type_;
    }
    //子类必须实现的消息处理
    //消息加密
    virtual void encode() = 0;
    //消息解密
    virtual void decode() = 0;
    //处理消息
    virtual void proc(const void* data, int32_t sz) = 0;
public:
    int32_t type_;
    
};
using GameMessagePtr = std::shared_ptr<GameMessage>;
class LoginReq : public GameMessage{
    public:
        LoginReq(){
            type_ = 0x0001;
        }
        void proc(const void* data, int32_t sz) override
        {
            
        }
        void encode(){
            
        }
        void decode(){
            
        }
    //定义数据结构
    public:
        std::string name_;
        std::string passwd_;
        int32_t plat_;
        MSGPACK_DEFINE(name_, passwd_, plat_);
    };
    class LoginRsp: public GameMessage{
    public:        
        LoginRsp(){
            type_ = 0x0002;
        }
        void encode(){
            
        }
        void decode(){
            
        }
        void proc(const void* data, int32_t sz) override
        {
            
        }
        int32_t err_;
        int32_t id_;
        int32_t gender_;
        std::string sign_;
        MSGPACK_DEFINE(err_, id_, gender_, sign_);
        
    };
    



#endif /* GAME_MESSAGE_H */

