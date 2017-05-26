#include <iostream>
#include <string>
#include <sys/select.h>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdio>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

#include "log.h"
#include "epoll_svr.h"
#include "book.pb.h"
#include "safe_queue.h"
#include "ddz_server.h"
#include "game_frame.h"
//#include "ConfigLoader.h"
using namespace std;
using namespace easynet;
//TODO:  还没做完
//FIXME: 你说呢
//XXX: WHAT?

void test()
{
    LOG(INFO) << "we are all bugs" << endl;

}
bool timer()
{
    timeval tm = {50, 0};
    select(0 , nullptr, nullptr, nullptr, &tm);
    return true;

}
void chrono_test()
{
    using namespace chrono;
    //准确的时钟
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();


    //系统时钟,纳秒, 离epoch 的时间间隔 
    chrono::system_clock::time_point t3 = chrono::system_clock::now();


    chrono::time_point<system_clock, chrono::seconds> t5 = chrono::time_point_cast<seconds> (chrono::system_clock::now());
    //  this_thread::sleep_for(chrono::seconds(2));
    chrono::time_point<system_clock, chrono::seconds> t6 = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::now());
    cout << t5.time_since_epoch().count() << endl;
    cout << t6.time_since_epoch().count() << endl;


    chrono::steady_clock::time_point t2= chrono::steady_clock::now();
    chrono::system_clock::time_point t4= chrono::system_clock::now();
    auto ret1 = t1.time_since_epoch().count() ;
    auto ret2 = t2.time_since_epoch().count() ;
    cout << "t2 - t1 = " << ret2 - ret1 << endl;


    auto ret3 = t3.time_since_epoch().count() ;
    auto ret4 =  t4.time_since_epoch().count() ;
    cout << "t4 - t3 = " << ret4 - ret3 << endl;

    cout << "t1 = " << ret1 << " t2 = " << ret2  << " t3 = " << ret3 << " t4 = " << ret4 << endl;
}

void testVec()
{
    vector<char> vc;
    vc.resize(12);
    memcpy(&vc[0], "wo nimswqds", 12);
    vc.resize(12);
    for(auto v : vc)
        cout << v ;
    cout << endl;

}

void testQ()
{
    SafeQueue<DataBufferPtr> m;
    m.Init(3000000);
    auto f1 = [&m]{
        for(;;){
            auto data = make_shared<DataBuffer>(7, 12);
            data.get()->AddData(12, "abcsw 321jj");
            m.Put(data);
            
            //this_thread::sleep_for(std::chrono::seconds(3));
            //LOG(INFO) << "after  sleep for 3 seconds ";

        }

    };
    auto f2 = [&m]{
        for(;;){
            vector<DataBufferPtr> tc;
            if(m.IsFull()){
                LOG(INFO) << "before poure 3000w msg ";
                m.Pour(tc);
                LOG(WARNING) << "get 3000w char success" << endl;
              //  for(auto c:tc)
               //     LOG(INFO) << c ;

            }
        }

    };
    thread t1(f1);
    thread t2(f2);
    t1.join();
    t2.join();

}
void testMap()
{
    class Data{
        public:
            Data(int32_t d){d_ = d; LOG(INFO) << "d is consturcted:" << d_;}
            ~Data(){LOG(INFO) << "d is desturcted:" << d_;}
        private:
            int32_t d_;
    };
    using DataPtr = std::shared_ptr<Data>;

    std::map<int32_t, DataPtr> data_map_;
    data_map_.insert({1, make_shared<Data>(1)});
    data_map_.insert({2, make_shared<Data>(2)});
    data_map_.insert({3, make_shared<Data>(3)});

    auto it = data_map_.find(1);
    data_map_.erase(it);

    getchar();


}
void test_sock_utils()
{
    auto svr = [](){
        int32_t fd = NetPackage::kINVALID_FD;
        bool ret = NetPackage::Listen(&fd, "0.0.0.0", 19977, true);
        int32_t acc_fd = -1;
        uint16_t acc_port = -1;
        std::string ip = "\0";
        for(;;){
            ret = NetPackage::Accept(fd, &acc_fd, &ip, &acc_port);
            LOG(INFO) << strerror(errno) << "accept success, fd:" << acc_fd << " ip:" << ip << " port:" << acc_port ; 
            NetPackage::Write(acc_fd, "Good Job!", 10);
            NetPackage::Close(acc_fd);
        }
    };
    auto client = [](){
        int32_t count = 0;
        char buff[64] = {0};
        for(;;){
            memset(buff, 0, 64);
            bool ret;
            int32_t fd;
            ret = NetPackage::Connect(&fd, "127.0.0.1", 19977);
            if(ret){
                LOG(INFO) << count << ". connect succeed, fd:" << fd << endl;
                this_thread::sleep_for(std::chrono::seconds(1));
                NetPackage::Read(fd, buff, 64);
                LOG(INFO) << "svr said:" << buff;
                NetPackage::Close(fd);
                ++count;
            }else{
                LOG(FATAL) << "faild!" << strerror(errno);
            }
        }
    };

    std::thread th1(svr);
    std::thread th2(client);
    th1.join();
    th2.join();

}


void test_epoll_function()
{
    auto svr = [](){
        int32_t fd = NetPackage::kINVALID_FD;
        bool ret = NetPackage::Listen(&fd, "0.0.0.0", 30077, true);
        int32_t acc_fd = -1;
        uint16_t acc_port = -1;
        std::string ip = "\0";
        
        int32_t efd = epoll_create(1024);
        EPOLL_EV ev = {0};
        ev.events = EPOLLIN;
        ev.data.fd = fd;               
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev);
        
        for(;;){
            EPOLL_EV events_[MAX_EVENTS] = {0};
            int32_t count = 0;
            count = epoll_wait(efd, events_, MAX_EVENTS, -1);            
            
            for(int c = 0; c < count; ++c){
                if(events_[c].data.fd == fd){
                    ret = NetPackage::Accept(fd, &acc_fd, &ip, &acc_port);
                    LOG(INFO) << strerror(errno) << "accept success, fd:" << acc_fd << " ip:" << ip << " port:" << acc_port ; 
                    //NetPackage::Write(acc_fd, "Good Job!", 10);
                    //NetPackage::Close(acc_fd);
                    EPOLL_EV *cli_ev = new EPOLL_EV;
                    cli_ev->events = EPOLLIN;
                    cli_ev->data.fd = acc_fd;
                    epoll_ctl(efd, EPOLL_CTL_ADD, acc_fd, cli_ev);
                }else{
                    int32_t cfd = events_[c].data.fd;
                    //NetPackage::Write(acc_fd, "Good Job!", 10);
                    char buff[1024] = {0};
                    read(acc_fd, buff, 1024); 

                    
                }
            }
        }
    };
    

    std::thread th1(svr);    
    th1.join();

}

auto count = make_shared<int32_t>(0);
auto on_accept = [](uint64_t id)  {
    LOG(INFO) << "on_accept id[" << id << "] "  ;    

};
auto on_data = [](uint64_t id, const void* data, int32_t sz, int32_t type){
     //LOG(INFO) << "on_data id[" << id << "] sz [" << sz << "]" ;  
     string msg((char*)data, sz);
     Person person;
     person.ParseFromString(msg);
     if(7 != person.id()){
         LOG(ERROR) << "parse id fail id[" << person.id() << "]";
     }
     LOG(INFO) << "name:" << person.name();
     LOG(INFO) << "id:" << person.id();
     LOG(INFO) << "result_per_page:" << person.result_per_page();
     LOG(INFO) << "score:" << person.score();
     LOG(INFO) << "phone:" << person.phone();
  
};

void test_epoll() {
    
    auto svr_proc = []{
        ConfigLoader::ServerInfo *info = ConfigLoader::load_server_config();
        
        EPOLLSvrPtr svr = std::make_shared<EPOLLSvr>();
        auto m = on_accept;
        auto server = svr.get();
        server->Init(info->config_.port_, 3000, m, on_data);
     
        server->Start();
    };
    
    auto cli_proc = []{
        this_thread::sleep_for(std::chrono::seconds(1));
        int32_t conn = NetPackage::kINVALID_FD;
        bool ret = false;
        
        ret = NetPackage::Connect(&conn, std::string("127.0.0.1"), 30077);
        
        char buff_[4096];
        Person person;
        person.set_name("Jack");
        person.set_id(7);
        person.set_result_per_page(9);
        person.set_score(Person::GOOD);
        person.set_phone("18221888856");
        string data;
        person.SerializeToString(&data);       
    
        int32_t sz = data.size() + sizeof(HEADER);        
        
        MSG *pMsg = (MSG* )buff_;
        pMsg->header.version_ = VERSION;
        pMsg->header.length_ = data.size();
        pMsg->header.state_ = 0;
        pMsg->header.reserve_ = 0;
        char *p = (char*)pMsg;
        p += sizeof(HEADER);
        memcpy(p, data.c_str(), data.size());
        //memcpy(&pMsg->body, data.c_str(), data.size());
    

        int32_t sendcnt = 0;        
        int32_t leftcnt = sz; 
        int32_t wr = 0;
        
        for(auto c = 0; c < 1000000; ++ c){
            char *sendbuf = buff_;
            sendcnt = 0;  
            leftcnt = sz; 
            //this_thread::sleep_for(std::chrono::seconds(1));
            leftcnt = sz;
            while(leftcnt > 0){
                //wr = NetPackage::Write(conn, sendbuf + sendcnt, sz); //这个是错误的
                wr = NetPackage::Write(conn, sendbuf + sendcnt, leftcnt);
                if(wr < 0){
                    if( EAGAIN == errno || EWOULDBLOCK == errno ){
                        //LOG(INFO) << "EAGAIN happen";
                        continue;
                    }else{
                        return false;
                    }
                }     
                sendcnt += wr;
                leftcnt -= wr;       
            }
        }    
        
        close(conn);
    };
    std::thread th1(svr_proc);    
    
    //std::thread th2(cli_proc); 
    //std::thread th3(cli_proc);
    th1.join();
    //th2.join();    
    //th3.join();

}

void test_client(){
    
    
}
 
int main(int argc, char* argv[])
{
    
    //LOGInfo log;
    //log.Init("main", true);
    //chrono_test();
    //testVec();
    //testQ();
    //test_sock_utils();    
    //test_epoll_function();
    //testMap();
    //Test_Rapid_Json::load_server_config();
    //test_epoll();
    //DDZServer d;
    //d.test();
    
    GameFramePtr f = std::make_shared<GameFrame>();
    f.get()->init(30077, 1000);
   
    
    //if( 2 != argc){
    //    LOG(ERROR) << "usage:";
    //    LOG(ERROR) << "please type \"main.run s\" for run server or \"main.run c\" for client:";
    //    exit(0);
   // }
    //string arg = argv[1];
    //if(arg == "s") {
     //   LOG(INFO) << "Server Running";
    //    test_epoll();
        
    //}else if(arg == "c"){
     //   LOG(INFO) << "Client Running";
    //}else{
    //    LOG(ERROR) << "usage:";
   //     LOG(ERROR) << "please type \"main.run s\" for run server or \"main.run c\" for client:";
    //    test_client();
   // }

    return 0;
}
