/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "receiver.h"
Receiver::Receiver(){
    step_ = RD_HEAD;

}

Receiver::~Receiver(){
    
}

void Receiver::on_receive(int32_t id, void* buff, int32_t sz){
    buffer_.put_msg(buff, sz);
    if( step_ == RD_HEAD){
        header_ = (HEAD*)buffer_.get_msg(HEAD_SZ);
        if( nullptr == header_ ){
            return;
        }
        step_ = RD_BODY;
    }
    if( step_ == RD_BODY ){
        void *data = buffer_.get_msg(header_->length);        
  
    }
}