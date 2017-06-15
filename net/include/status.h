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
namespace easynet {

    class NetWorkStatus {
    public:

        static void add_send_packet(int32_t c) ;

        static void add_receive_packet(int32_t c);

        static void add_online_count();

        static void del_online_count();
        static void print_status() ;
    private:
        //接收发送流量
        static uint64_t send_packet_;
        static uint64_t receive_packet_;

        //接收发送消息数
        static uint64_t send_msg_;
        static uint64_t receive_msg_;

        static uint64_t online_count_;


    };

   



}


#endif /* STATUS_H */

