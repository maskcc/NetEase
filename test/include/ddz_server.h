/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ddz_server.h
 * Author: admin
 *
 * Created on 2017年5月22日, 下午4:59
 */

#ifndef DDZ_SERVER_H
#define DDZ_SERVER_H

#include <msgpack.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include "log.h"
#include "epoll_svr.h"
#include "config_loader.h"

namespace DDZ_NET_MSG{    
    
    class LoginReq{
    public:
        string name_;
        string passwd_;
        int32_t plat_;
        MSGPACK_DEFINE(name_, passwd_, plat_);
    };
    //using LoginReq = msgpack::type::tuple<string, string, int32_t>;
    using LoginRsp = msgpack::type::tuple<int32_t, int32_t, int32_t, string>;
    
    
    
}



class DDZServer{
public:
    void start();
    void test();
    
    
private:
    
    
    
    
    
    
    
    
};



#endif /* DDZ_SERVER_H */

