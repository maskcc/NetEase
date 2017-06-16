/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   imsg.h
 * Author: admin
 *
 * Created on 2017年6月16日, 下午3:37
 */

#ifndef IMSG_H
#define IMSG_H
#include <stdint.h>
#include <memory>
class IMsg{
public:
    IMsg();
    ~IMsg();
    virtual void* encode(){return nullptr;};
    virtual void decode(){};
    virtual int32_t size(){return 0;};
};
using IMsgPtr = std::shared_ptr<IMsg>;

#endif /* IMSG_H */

