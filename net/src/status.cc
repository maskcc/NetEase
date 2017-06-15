/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "status.h"

using namespace easynet;

void NetWorkStatus::add_send_packet(int32_t c) {
    send_packet_ += c;
    ++send_msg_;
}

void NetWorkStatus::add_receive_packet(int32_t c) {
    receive_packet_ += c;
    ++receive_msg_;
}

void NetWorkStatus::add_online_count() {
    ++online_count_;
}

void NetWorkStatus::del_online_count() {
    --online_count_;
}

void NetWorkStatus::print_status() {
    LOG(INFO) << "发送流量:" << send_packet_ << " 接收流量:" << receive_packet_
            << " 发送消息:" << send_msg_ << " 接收消息:" << receive_msg_
            << " 在线数量:" << online_count_;
}


uint64_t NetWorkStatus::send_packet_ = 0;
uint64_t NetWorkStatus::receive_packet_ = 0;
uint64_t NetWorkStatus::send_msg_ = 0;
uint64_t NetWorkStatus::receive_msg_ = 0;
uint64_t NetWorkStatus::online_count_ = 0;