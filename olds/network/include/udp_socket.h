/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   udp_socket.h
 * Author: admin
 *
 * Created on 2017年3月7日, 下午1:30
 */

#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H

#include "svr_define.h"
namespace easynet{
    class UDPSocket : public IPlayer
    {

    };
    using UDPSocketPtr = std::shared_ptr<UDPSocket>;

}
#endif /* UDP_SOCKET_H */

