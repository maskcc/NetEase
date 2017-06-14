/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "tcp_accept.h"
using namespace easynet;

//TCPAccept
    TCPAccept::TCPAccept(int32_t port, EPOLLSvrPtr s) {
        peer_.port_ = port;
        accept_handler_ = nullptr;
        svr_ = s;
    }
    void TCPAccept::SetHandler(On_Accept_Handler h){
        accept_handler_ = h;
        
    }
    bool TCPAccept::Listen(bool reuse) {
        if(!NetPackage::Listen(&peer_.fd_, "0.0.0.0", peer_.port_, reuse)){
            LOG(FATAL) << "Listen failed on port " << peer_.port_ ;
            return false;
        }
        LOG(INFO) << "Listen success on port [" << peer_.port_ << "] fd[" << peer_.fd_ << "]";
        return true;
    }
    TCPAccept::~TCPAccept() {
    }
    void TCPAccept::OnNetMessage() {
        if( nullptr == accept_handler_){
            LOG(FATAL) << "tcpsocket handler is nullptr!";
            return;
        }  
        int32_t cliFD = NetPackage::kINVALID_FD;
        std::string ip = "";
        uint16_t port = 0;
        bool ret = false;
        //do{ //ET 模式
            ret = NetPackage::Accept( peer_.fd_, &cliFD, &ip, &port);
            if(false == ret){
                // 可能会有问题, 当只是发生了 eintr, EAGAIN, EWOULDBLOCK 或其他错误时, 还有没接收的连接
                //LOG(INFO) << "accept fail" ;
                return;
            }             

            TCPSocketPtr sock = std::make_shared<TCPSocket>(this->svr_.get()->shared_from_this());
            auto client = sock.get();
            client->peer_.addr_ = std::move(ip);
            client->peer_.fd_ = cliFD;
            client->peer_.port_ = port;   

            NetPackage::SetNonBlock(cliFD);
            NetPackage::SetNoDelay(cliFD);
            //这个设置很危险, (http://coolshell.cn/articles/11564.html)
            //NetPackage::SetReuse(cliFD);
       
            svr_.get()->AddConnection(sock);   
            
            auto f = accept_handler_;
            try{
                f(client->getID());
            }catch(...){
                LOG(ERROR) << "tcpsocket handler run fail!";
            }
            
        //}while(ret);
        
    }