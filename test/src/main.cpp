#include <iostream>
#include <string>
#include <sys/select.h>

#include "Log.h"

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

int main(int argc, char* argv[])
{
        Log log;
        log.Init("main");

    do
    {
        LOG(INFO) << "time out";
        char str[20] = "hello log!";
        LOG(INFO) << str;
        string cStr = "hello google!";
        LOG(INFO) << cStr;
        LOG(INFO) << "info test" << "hello log!";  //输出一个Info日志
        LOG(WARNING) << "warning test";  //输出一个Warning日志
        LOG(ERROR) << "error test\n\n";  //输出一个Error日志
    }while(timer());




    test();
    google::ShutdownGoogleLogging();


    return 0;
}
