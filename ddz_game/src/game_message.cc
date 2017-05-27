/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "game_message.h"

void GameMessage::decode(const void* data, int32_t sz){
        std::string str( (char*)data, sz );
        msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());
        msgpack::object deserialized = oh.get();     
        decode(deserialized);
        
    }

std::string GameMessage::encode(){
        std::stringstream buffer;
        encode(buffer);
        return std::string( buffer.str() );
    }