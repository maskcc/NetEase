#include <iostream>
#include <string>
#include <sys/select.h>
#include <thread>
#include <chrono>
#include <vector>

#include "log.h"
#include "epoll_svr.h"

using namespace std;
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

void test_epoll() {
    EPOLLSvr svr;
    svr.Init(30077, 3000);
    svr.Start();


}


int main(int argc, char* argv[])
{
    LOGInfo log;
    log.Init("main", true);
    //chrono_test();
    //testVec();
    //testQ();
    //test_sock_utils();
    test_epoll();
    //testMap();


    return 0;
}
