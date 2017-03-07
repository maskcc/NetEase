/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   monitor.h
 * Author: admin
 *
 * Created on 2017年3月7日, 下午1:28
 */

#ifndef MONITOR_H
#define MONITOR_H
#include "svr_define.h"
namespace easynet{
//一些统计信息 MONITOR
    class MONITOR
    {
        public:
            MONITOR(){recv_pack_count_ = 0; send_pack_count_ = 0;}
            void AddRecvPack(){++recv_pack_count_;}
            void AddSendPack(){++send_pack_count_;}
            uint64_t GetRecvPack(){return recv_pack_count_;}
            uint64_t GetSendPack(){return send_pack_count_;}
        private:
            std::atomic<uint64_t> recv_pack_count_; //收到的包数量
            std::atomic<uint64_t> send_pack_count_; //收到的包数量

    };
}

#endif /* MONITOR_H */

