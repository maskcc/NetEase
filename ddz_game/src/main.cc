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


using namespace std;
using namespace easynet;
//TODO:  还没做完
//FIXME: 你说呢
//XXX: WHAT?

int main(int argc, char* argv[])
{   
    GameFramePtr f = std::make_shared<DDZServer>();
    f.get()->start();

    return 0;
}
