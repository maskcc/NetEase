/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tcp_connector.h
 * Author: admin
 *
 * Created on 2017年3月7日, 下午1:23
 */

#ifndef TCP_CONNECTOR_H
#define TCP_CONNECTOR_H

#include "svr_define.h"
#include "tcp_socket.h"
namespace easynet{ 
    class TCPSocket;
    using TCPSocketPtr = std::shared_ptr<TCPSocket>;
 class TCPConnector 
    {
        public:
            //自动重连 
            //do_re 是否自动重连
            //time 断开后重连间隔 10秒
            // max_count 最多重连次数
            TCPConnector(EPOLLSvrPtr svr, bool do_re = true, int32_t time = 10, int32_t max_count = 10);              
            void OnReconnect();
            
            bool Connect();
        public:
            TCPSocketPtr socket_;
        private:
            //重连方法
            On_Reconnect_Handler handler_;
            //是不是已经连接 
            bool connected_; 
            //是否进行重连
            bool do_reconn_; 
            
            //重连时间
            int32_t reconn_time_; 
            //重连次数
            int32_t reconn_count_; 
            //最多重连次数
            int32_t max_reconn_count_; 
            
            EPOLLSvrPtr svr_;

    };
    using TCPConnectorPtr = std::shared_ptr<TCPConnector>;

}
#endif /* TCP_CONNECTOR_H */

