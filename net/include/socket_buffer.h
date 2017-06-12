/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   socket_buffer.h
 * Author: admin
 *
 * Created on 2017年6月12日, 下午2:39
 */

#ifndef SOCKET_BUFFER_H
#define SOCKET_BUFFER_H
#include "svr_define.h"
#include <vector>
/*
 *不是线程安全的函数
 *接收数据和处理数据应该是同步进行的
 * 接收到的数据不能异步处理
 * 处理完数据不需要释放内存
 */
class SocketBuffer{
public:
    SocketBuffer(int32_t max_sz);
    ~SocketBuffer();
public:
    bool put_msg(void *msg, int sz);
    void* get_msg(int sz);
    
protected:
    bool check_resize(int32_t sz);
    inline int32_t buffer_size(){
        return cur_step_ * buff_step_;
    }
    inline int32_t free_size(){
        return buffer_size() - (end_pos_ - start_pos_);
    }
private:
    int32_t max_buff_sz_;
    int32_t start_pos_;
    int32_t end_pos_;
    int32_t cur_step_ ;
    int32_t buff_step_;
    uint8_t *buffer_;
    
    
    
};


#endif /* SOCKET_BUFFER_H */

