/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   config.h
 * Author: admin
 *
 * Created on 2017年6月6日, 下午7:02
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <stdint.h>
namespace easynet{
    struct Config{
        bool    nodelay;         //不延迟
        bool    reuse;
        int32_t max_reconnect;  // 最大重连次数
        int32_t reconnect_time; //重连间隔
        int32_t disconnect_time;//过多久断开连接
        int32_t max_connections;//最大连接数量
        int32_t port;           //监听端口号        
        int32_t window;         //发送接收窗口
        std::string name;       //服务名称
        
        
        
        
        
        
        
        
    };
    
    
    
}


#endif /* CONFIG_H */

