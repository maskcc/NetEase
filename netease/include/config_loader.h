/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConfigLoader.h
 * Author: admin
 *
 * Created on 2017年5月22日, 下午7:12
 */

#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <string>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include "log.h"


namespace ConfigLoader {
    //服务器的信息, 包括 ip, 端口, 名称, id
    using namespace rapidjson;
    using namespace std;
    class ServerConfig{
        public:
            ServerConfig(){
                name_ = "";
                ip_ = "";
                port_ = -1;
                id_ = -1;
                connected_ = false;
                
            }
        public:
            string name_;
            string ip_;
            int32_t port_;
            int32_t id_;
            bool   connected_;
    };
    class ServerInfo{
        public:
            ServerConfig* find_svr(const string& name);
            ServerInfo(){
                open_reconnect_ = 1;
                time_out_ = 10;
                max_count_ = 10;
            }
            ServerConfig config_;
            vector<ServerConfig> connected_;
            int32_t open_reconnect_;
            int32_t time_out_;
            int32_t max_count_;
            
            
    };
    using ServerInfoPtr = std::unique_ptr<ServerInfo>;
    void parse_server_config(const Value& doc, ServerConfig& conf);
    void parse_net_init(const Value& doc, ServerInfo* info);
    void parse_rec_init(const Value& doc, int32_t &open, int32_t &time, int32_t &max);
    ServerInfoPtr load_server_config() ;
    

}


#endif /* CONFIGLOADER_H */

