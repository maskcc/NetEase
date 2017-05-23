#ifndef LOG_H
#define LOG_H

#include <stdint.h>
#include <string>
#include "glog/logging.h"
#include <stdio.h>
using namespace std;

class LOGInfo
{
    public:
        //_proc 程序名, _save 存储位置, _sz 日志文件最大多少M, _ctlShow 是否只显示在控制台
        void Init(std::string _proc, bool _ctlShow = false, std::string _save = "./log", int32_t _sz = 2048)
        {
            _proc = string("LOG_") + _proc;
            FLAGS_log_dir = _save.c_str();
            google::InitGoogleLogging(_proc.c_str());
            google::SetLogDestination(google::GLOG_INFO, (_save + "/").c_str());
            google::SetStderrLogging(google::GLOG_INFO );
            //为了git忽略
            google::SetLogFilenameExtension("LOG_");

            FLAGS_colorlogtostderr = true;  // Set log color
            FLAGS_logtostderr = false; //设置为只在控制台显示
            FLAGS_logbufsecs = 0;  // Set log output speed(s)
            FLAGS_max_log_size = _sz;  // Set max log file size
            FLAGS_stop_logging_if_full_disk = true;  // If disk is full

        }
        ~LOGInfo()
        {
            //关闭日志
            google::ShutdownGoogleLogging();

        }

};

#endif
