/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "tcp_connector.h"
using namespace easynet;

//TCPConnector 连接别的服务器
     TCPConnector::TCPConnector(EPOLLSvrPtr svr, bool do_re, int32_t time, int32_t max_count) {
         do_reconn_ = do_re;
         reconn_time_ = time;
         max_reconn_count_ = max_count;
         reconn_count_ = 0;
         connected_ = false;
         svr_ = svr;
         
    }
    bool TCPConnector::Connect(){
        bool retCode = false;
        auto sock = std::make_shared<TCPSocket>(svr_);
        auto s = sock.get();
        if(connected_){
            //失败了sock自动释放
            LOG(WARNING) << "已经在连接状态但是还是重连了! ip[" << s->peer_.addr_ << "] port[" << s->peer_.port_ << "]";
            return true;
        }
        retCode = NetPackage::Connect(&s->peer_.fd_, s->peer_.addr_, s->peer_.port_);
        if(!retCode){
            //失败了sock自动释放
            LOG(ERROR) << "connect server fail ip[" << s->peer_.addr_ << "] port[" << s->peer_.port_ << "]";
            return false;
        }
        connected_ = true;  
        socket_ = sock;
        return true;
    }
    void TCPConnector::OnReconnect(){
        if( nullptr == handler_){
            LOG(FATAL) << "reconnect handler is nullptr!";
            return;
        }
        auto f = handler_;
        try{
            f();
        }catch(...){
            LOG(ERROR) << "reconnect handler run fail!";
        }
    }
    
