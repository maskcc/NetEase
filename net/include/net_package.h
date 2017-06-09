/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   net_package.h
 * Author: admin
 *
 * Created on 2017年3月7日, 下午1:31
 */

#ifndef NET_PACKAGE_H
#define NET_PACKAGE_H
#include "svr_define.h"
//网络包裹函数 NetPackage
//统计信息
namespace easynet{
    class NetPackage
    {
        public:
            static const int32_t kMAX_BACK_LOG = 1024;
            static const int32_t kINVALID_FD = -1;
            static bool IsIPV6(const std::string& ip);
            static bool SetNonBlock(int32_t fd);
            static bool SetNoDelay(int32_t fd);
            static bool SetReuse(int32_t fd);
            static bool SetIPV6Only(int32_t fd, bool only);
            //端口号要注意是uint16_t 类型, 如果不是unsigned, 会有负数端口号
            static bool Listen(int32_t *fd, const std::string& listenIP, uint16_t port, bool reuse);
            static bool Accept6(int32_t listen_fd, int32_t *fd, std::string *ip, uint16_t *port);
            static bool Accept(int32_t listen_fd, int32_t& fd, std::string& ip, uint16_t& port);
            static bool Close(int32_t fd);
            static int32_t Read(int32_t fd, void *buf, int32_t sz);
            static int32_t Write(int32_t fd, const void *buf, int32_t sz);
            static bool Connect(int *connFD, const std::string& remoteIP, uint16_t remotePort);
    };

}
#endif /* NET_PACKAGE_H */

