/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   iplayer.h
 * Author: admin
 *
 * Created on 2017年3月7日, 上午11:23
 */

#ifndef IPLAYER_H
#define IPLAYER_H
#include "svr_define.h"
#include "socket.h"
namespace easynet{
//代表一个连接对象 基类 IPlayer
    class IPlayer
    {
        public:
            IPlayer();
            virtual ~IPlayer();
            virtual void OnNetMessage();
            virtual bool Send();
            virtual uint64_t getID();
            virtual void KickOut();
            virtual void SetHandler(On_Accept_Handler h);
            virtual void SetHandler(On_Socket_Handler h);
            
        public:
            Socket   peer_;    //连接信息
        private:
            uint64_t identify_;//用来标志自己的身份

    };
    //连接信息的智能指针
    using IPlayerPtr = std::shared_ptr<IPlayer>;


}
#endif /* IPLAYER_H */

