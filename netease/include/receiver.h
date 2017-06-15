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

#include "svr_define.h"
using namespace easynet;
class Receiver{
public:
    Receiver();
    ~Receiver();
public:
    void on_receive(int32_t fd, void* buff, int32_t sz, NetErrorCode&);
    
    
};

#endif /* RECEIVER_H */

