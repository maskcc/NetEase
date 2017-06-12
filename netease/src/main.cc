/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cc
 * Author: admin
 *
 * Created on 2017年6月9日, 下午3:00
 */

#include <memory>
#include <cstdlib>
#include "event_loop.h"
#include "socket_buffer.h"
using namespace std;

using namespace easynet;
/*
 * 
 */
void test1(){
    LOGInfo log;
    log.Init("logic");
    EventLoopPtr loop = make_shared<EventLoop>();
    loop->initialize();
    TcpAcceptPtr accept = make_shared<TcpAccept>(loop);
    auto onreceive = [](int32_t /*fd*/ id,void* data, int32_t sz,  NetErrorCode e){
        LOG(INFO) << "id:" << id << " sz:" << sz << " code:" << e.err_;
    };
    auto onacc = [&onreceive](int32_t id, TcpSocketPtr sock, NetErrorCode e){
        LOG(INFO) << "connected";
        sock->do_receive(onreceive);
    
    };
    accept->do_accept(10011, onacc);
    TcpAcceptPtr accept2 = make_shared<TcpAccept>(loop);
    accept2->do_accept(10012, onacc);
    while(true)
        loop->run_once(5 * 1000);
    
}

using DATA = struct{
    int32_t id;
    int32_t age;
    int32_t real;
    int32_t del;
    
};
using INT = struct{
    int32_t a;
};
void test_buffer(){
    INT *a = new INT();
    a->a = -1;
    uint8_t *m = (uint8_t*)a;
    for(int c = 0; c < 4; ++c){
        LOG(INFO) << (int32_t)m[c] ;
    }
    
   // return;
    DATA data = { 0, 33, 77, -1 };
    SocketBuffer buff(512);
    int32_t max = 65535;
    for(int32_t c = 0; c <max; ++c ){
        data.id = c;
        buff.put_msg(&data, sizeof(DATA));
    }
    for(int32_t c = 0; c <max; ++c ){        
        DATA *d = nullptr;
        d = (DATA*) buff.get_msg(sizeof(DATA));
        if( d->id != c){
            LOG(ERROR) << "id:" << d->id << " c:" << c;
        }
    }
    
    
    
}
int main(int argc, char** argv) {
    test1();
    //test_buffer();
    return 0;
}

