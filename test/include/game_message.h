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

#include "game_frame.h"
#include "log.h"
#include "game_player.h"

class GameFrame;
using GameFramePtr = std::shared_ptr<GameFrame>;


class GameMessage{
public:
    //每个继承此类的子类在类中都要添加两个个宏定义来
    // MSGPACK_DECODE_DEF   和 MSGPACK_ENCODE_DEF
    // 用来定义消息解析方法
    //获取消息类型
    void set_frame(GameFramePtr ptr){
        frame_ = ptr;
    }
    int32_t type(){
        return type_;
    }
    //子类必须实现的消息处理
    //消息加密
    
    
    //消息解密
    void decode(const void* data, int32_t sz){
        std::string str( (char*)data, sz );
        msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());
        msgpack::object deserialized = oh.get();     
        decode(deserialized);
        
    }
    //会产生一次拷贝
    std::string encode(){
        std::stringstream buffer;
        encode(buffer);
        return std::string( buffer.str() );
    }
    virtual void decode(const msgpack::object& parse) = 0;
    virtual void encode(std::stringstream& buf) = 0;
    //处理消息
    virtual void proc(GamePlayerPtr player) = 0;
protected:
    int32_t type_;
    GameFramePtr frame_;
    
};

#define MSGPACK_DECODE_DEF void decode(const msgpack::object& parse) override{\
            parse.convert(*this);\
        }
#define MSGPACK_ENCODE_DEF void encode(std::stringstream& buf) override{\
            msgpack::pack(buf, *this);\
            buf.seekg(0);\
        }

using GameMessagePtr = std::shared_ptr<GameMessage>;


#endif /* GAME_MESSAGE_H */

