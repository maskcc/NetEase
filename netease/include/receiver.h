/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   receiver.h
 * Author: admin
 *
 * Created on 2017年6月15日, 下午4:46
 */

#ifndef RECEIVER_H
#define RECEIVER_H

#include "socket_buffer.h"
#include "svr_define.h"
using namespace easynet;
struct HEAD{
    int32_t id;
    int32_t encode;
    int32_t type;
    int32_t length;
};
class Receiver{
public:
    enum RD_MSG_STEP{
        RD_HEAD,
        RD_BODY
    };
    Receiver();
    ~Receiver();
public:
    void on_receive(int32_t id, void* buff, int32_t sz);
    HEAD *header_;
    SocketBuffer buffer_; 
    RD_MSG_STEP step_;
    static const int32_t HEAD_SZ = 8;
};

#endif /* RECEIVER_H */

