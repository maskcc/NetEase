/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   data_buffer.h
 * Author: admin
 *
 * Created on 2017年3月7日, 上午11:17
 */

#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#include "svr_define.h"
namespace easynet{
//发送的数据包缓冲结构 DataBuffer
    class DataBuffer
        {
            public:
                //sock, 连接的fd
                //sz, 包的总长
                //rsz, real sz, 接收到的包的长度
                //ptr, 数据地址
                DataBuffer(int32_t sock, int32_t sz);
                ~DataBuffer();

                //向buff里添加数据, 返回还不够的长度, 
                //返回值为 -1 表示添加的数据长度比总长度还长, 返回0 表示已经传送完成
                int32_t AddData(int32_t sz, const char* ptr);
                void Resize(int32_t sz);

                //返回还需要的数据长度
                //返回-1 表示错误, pos比sz大, 不存在, 返回0 表示已经好了, 返回其他大于0 的值表示还需要的数据长度
                int32_t NeedData();
                const void* GetBuffPtr();

            private:
                int32_t  sock_;  //文件描述符
                int32_t  sz_;    //数据包大小
                int32_t  pos_;   //当前发送的字节下标
                std::vector<uint8_t> buf_;

        };
    using DataBufferPtr = std::shared_ptr<DataBuffer>;
}

#endif /* DATA_BUFFER_H */

