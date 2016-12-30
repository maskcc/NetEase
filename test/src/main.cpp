#include <iostream>
#include <string>
#include <sys/select.h>
#include <thread>
#include <chrono>
#include <vector>

#include "Log.h"
#include "EPOLLSvr.h"

using namespace std;

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

int main(int argc, char* argv[])
{
    LOGInfo log;
    log.Init("main", true);
    //chrono_test();
    testVec();








    return 0;
}
