/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   svr_define.h
 * Author: admin
 *
 * Created on 2017年3月7日, 上午11:27
 */

#ifndef SVR_DEFINE_H
#define SVR_DEFINE_H

//头文件
#include <sys/epoll.h> //epoll
#include <netinet/in.h>//sockaddr_in
#include <sys/socket.h>//socket, listen, bind, accept, connect, shutdown,sockaddr
#include <error.h>//errno, EINTR
#include <arpa/inet.h> //sockaddr_in, in_addr, ntohs, htonl
#include <unistd.h> //read, write, close
#include <stdint.h>
#include <string.h>
#include <sys/fcntl.h>
#include <netinet/tcp.h> //TCP_NODELAY

#include <thread>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <map>
#include <iostream>
#include <memory>
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include <glog/logging.h>



const int32_t MAX_EVENTS = 128;
const int32_t MAX_SOCK_BUFF = 1024 * 64; //网络上最大的包大小为 64K

const uint8_t VERSION = 0x99;
using HEADER = struct tagHEADER {
    uint8_t   version_;   //版本号
    uint8_t   encode_;    
    uint8_t   reserve_;   //是否反序
    uint8_t   state_;     //TODO 序列, 类似tcp 序列字段, 序号不对断开连接?
    int16_t  length_;    //包体大小, 最大 65535     
    int16_t  type_;
    
    
};
//包头大小
const int32_t HEADER_SZ = sizeof(HEADER);
using MSG = struct tagMSG {
    HEADER header; //包头
    void   *body;  //包体
};



using On_Accept_Handler = std::function<void(uint64_t)>;
using On_Socket_Handler = std::function<void(uint64_t, const void*, int32_t, int32_t)>;
using On_Reconnect_Handler = std::function<void()>;
using On_Timer_Handler = std::function<void()>;


//定时器
//定时执行过程, 能够添加过程, 删除过程
//写在主线程中, 能处理主线程数据, 在epoll里处理, epoll超时单位为毫秒
using _OnTimerHandler = std::function<void()>;


//获取下一个id 
    static uint64_t next_id(){
        static uint64_t id = 0;
        ++id;
        return id;
    }
    
    
    enum TCP_STEP  {
        READ_HEAD = 1,
        READ_BODY
    };

#endif /* SVR_DEFINE_H */

