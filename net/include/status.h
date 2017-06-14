/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   status.h
 * Author: admin
 *
 * Created on 2017年6月14日, 下午5:51
 */

#ifndef STATUS_H
#define STATUS_H

#include <stdint.h>
#include "log.h"
namespace easynet{
class NetWorkStatus{
public:
    NetWorkStatus(){
        send_packet_ = 0;
        receive_packet_ = 0;
        send_msg_ = 0;
        receive_msg_ = 0;
        online_count_ = 0;
    }
    void add_send_packet(int32_t c){
        send_packet_ += c;
        ++send_msg_;
    }
    void add_receive_packet(int32_t c){
        receive_packet_ += c;
        ++receive_msg_;
    }
    void add_online_count(){
        ++online_count_;
    }
    void del_online_count(){
        --online_count_;
    }
    void print_status(){
        LOG(INFO) << "发送流量:" << send_packet_ << " 接收流量:" << receive_packet_
                << " 发送消息:" << send_msg_ << " 接收消息:" << receive_msg_
                << " 在线数量:" << online_count_;
    }
private:
    //接收发送流量
    uint64_t send_packet_;
    uint64_t receive_packet_;
    
    //接收发送消息数
    uint64_t send_msg_;
    uint64_t receive_msg_;
    
    uint64_t online_count_;
    
    
};
}


#endif /* STATUS_H */

