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
    auto onacc = [](int32_t id, TcpSocketPtr sock, NetErrorCode& e){
        LOG(INFO) << "connected";
    
    };
    accept->do_accept(10011, onacc);
    TcpAcceptPtr accept2 = make_shared<TcpAccept>(loop);
    accept2->do_accept(10012, onacc);
    while(true)
        loop->run_once(5 * 1000);
    
}
int main(int argc, char** argv) {
    test1();
    return 0;
}

