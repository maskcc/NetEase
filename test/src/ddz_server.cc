/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ddz_server.h"

using namespace easynet;
void DDZServer::start(){
    /*
    auto on_data = [](uint64_t id, const void* data, int32_t sz, int32_t type){
        LOG(INFO) << "on_data id[" << id << "] type[" << type <<"] sz[" << sz << "]";
        
        std::string msg((char*)data);
        msgpack::object_handle oh = msgpack::unpack(msg.data(), msg.size());
    
        msgpack::object deserialized = oh.get();

        std::cout << deserialized << std::endl;

        //msgpack::type::tuple<string, string, int32_t> dst;
        DDZ_NET_MSG::LoginReq req;
        deserialized.convert(req);
        LOG(INFO) << "name:" << req.name_ << " passwd:" << req.passwd_ << " platform:" << req.plat_;
        
    };
    auto on_accept = [](uint64_t id)  {
        LOG(INFO) << "on_accept id[" << id << "] ";  
    };
     
    ConfigLoader::ServerInfo *info = ConfigLoader::load_server_config();
        
    EPOLLSvrPtr svr = std::make_shared<EPOLLSvr>();     
    auto server = svr.get();
    server->Init(info->config_.port_, 3000, on_accept, on_data);     
    server->Start();    
    
   
    */
    
}

void DDZServer::test(){
    start();
    /*
    msgpack::type::tuple<string, string, int32_t> data("good", "job", 3);
    
    using msg = msgpack::type::tuple<string, string, int32_t>;
    msg data2("ok", "right", 3);
    std::stringstream buffer;
    msgpack::pack(buffer, data2);
    
    buffer.seekg(0);
    
    std::string str( buffer.str() );
    msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());
    
    msgpack::object deserialized = oh.get();
    
    std::cout << deserialized << std::endl;
    
    msgpack::type::tuple<string, string, int32_t> dst;
    deserialized.convert(dst);
    */
    
    
    
    
    
}