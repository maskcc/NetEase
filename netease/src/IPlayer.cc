/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "iplayer.h"
IPlayer::IPlayer(TcpSocketPtr s){
    peer_ = s;
}

IPlayer::~IPlayer(){
    
    
    
}

bool IPlayer::send_msg(IMsgPtr msg){
    if( nullptr == msg ){
        return false;
    }
    void *data = msg->encode();
    return peer_->send_msg(data, msg->size());
}