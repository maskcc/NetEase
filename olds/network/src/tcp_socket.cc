/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "tcp_socket.h"
using namespace easynet;
//TCPSocket 已连接的服务器
    TCPSocket::TCPSocket(EPOLLSvrPtr s) {
        socket_handler_ = nullptr;
        svr_ = s;
        msg_ = nullptr;
        step_ = READ_HEAD;
    }
    void TCPSocket::SetHandler(On_Socket_Handler h){
        socket_handler_ = h;
        
    }
   
    void TCPSocket::OnNetMessage(){    
        if( nullptr == socket_handler_){                        
            LOG(FATAL) << "tcpsocket handler is nullptr!";
            return;
        }        
        //char *buff = svr_.get()->buff_;  //对数组操作一定要注意, 会有越界的可能
        for(;;){
            char *buff = svr_.get()->buff_;
            //一直读, 直到出错           
            int32_t ret = NetPackage::Read(peer_.fd_, buff, MAX_SOCK_BUFF); 
            //LOG(INFO) << "read ok ret[" << ret << "]";
               
                                                                                 
            if(0 == ret ){     
                LOG(INFO) << "connection closed by peer fd[" << peer_.fd_ << "]";
                this->KickOut();
                return;
            }

            if( ret < 0 ){
                if( EAGAIN  == errno || EWOULDBLOCK == errno ){
                    //再次read, buff将从头开始填充
                    //buff = svr_.get()->buff_;  
                    //continue;
                    return;

                }else{   
                    LOG(INFO) << "read fail! fd[" << peer_.fd_ << "] errno[" << errno << "] msg[" << strerror(errno) << "]";
                    this->KickOut();  
                    return;
                }
            }

            int32_t more_data = ret;
            
            while( more_data > 0){
                if( nullptr == peer_.buff_ ){                   
                    peer_.buff_ = std::make_shared<DataBuffer>(peer_.fd_, HEADER_SZ); 
                }

                auto data_buffer = peer_.buff_.get();               
                int32_t need_data = data_buffer->NeedData();

                //读取包头
                if( READ_HEAD == step_ ){
                    if( more_data < need_data ) {
                        //包头没有读完整
                        data_buffer->AddData(more_data, buff);
                        return;
                    }
                    data_buffer->AddData(need_data, buff);  
                    //指向body的头指针, 向前添加已经读过的内存
                    buff += need_data;
                    more_data = (more_data - need_data) < 0 ? 0:(more_data - need_data);
                    msg_ = (MSG* )data_buffer->GetBuffPtr();
                    
                    
                    if(VERSION != msg_->header.version_ || IDENTIFY != msg_->header.identify_){
                        LOG(ERROR) << "version or identify is not fit! kick out client fd[" << peer_.fd_ << "] version[" 
                                  << (int)msg_->header.version_ << "] current version[" << (int)VERSION<<"]" << "identify[" 
                                << (int)msg_->header.identify_ << "] current identify[" << (int)IDENTIFY << "]";
                        this->KickOut();
                        LOG(INFO) << "receive msg count[" << m.GetRecvPack() << "]";
                        return;
                    }
                    msg_->header.length_ = ntohs(msg_->header.length_);
                    msg_->header.type_ = ntohs(msg_->header.type_);

                    //为body 申请内存
                    data_buffer->Resize(msg_->header.length_ + HEADER_SZ);  
                    //重新申请内存后, 以前的msg_指向的内容不能再使用了
                    msg_ = (MSG* )data_buffer->GetBuffPtr();
                    need_data = data_buffer->NeedData();                

                    step_ = READ_BODY;            
                }

                //现在的step 肯定是 READ_BODY
                if( more_data > 0 ) {
                    //读取body
                    if(more_data < need_data) {
                        data_buffer->AddData(more_data, buff);
                        return;
                    }

                    data_buffer->AddData(need_data, buff);
                    more_data = (more_data - need_data) < 0 ? 0:(more_data - need_data);
                    //buff读取后指针后移
                    buff += need_data;
                    
                    m.AddRecvPack();
                                
                    //客户程序只需要截获到数据信息就行, 不用关心包头   
                    char *pMsg = (char* )(data_buffer->GetBuffPtr());
                    pMsg +=  sizeof(HEADER);
                    
                    auto f = socket_handler_;
                    try{
                        f(this->getID(), pMsg,msg_->header.length_, msg_->header.type_);
                    }catch(...){
                        LOG(ERROR) << "tcpsocket handler run fail!";                        
                    }                    
                    //自动删除已经用过的packet
                    auto tmp = std::move(peer_.buff_);     
                    tmp = nullptr;
                    peer_.buff_ = nullptr;//是不是多此一举, 就是多此一举, move 后peer_buff_ 会为nullptr
                    //读取新的包头
                    step_ = READ_HEAD;

                }
            }   
        }
    }
    void TCPSocket::KickOut() {
        IPlayer::KickOut();
        //TODO 需要再 EPOLLSvr 的map 中删除事件events_map_ 和连接信息 player_map_
    }
