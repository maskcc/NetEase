/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "socket_buffer.h"

SocketBuffer::SocketBuffer(int32_t buff_step) {
    //一个连接到来时先不要为其分配缓冲区, 当需要时按照 buff_step 分配
    //最大缓冲为 1m, 超过肯定是错误, 暂时未做处理内存可以为任意大
    max_buff_sz_ = 1024 * 1024;
    start_pos_ = 0;
    end_pos_ = 0;
    cur_step_ = 0;
    buff_step_ = buff_step;
    buffer_ = (uint8_t *) malloc(buffer_size());

}

SocketBuffer::~SocketBuffer() {
    free(buffer_);
}

bool SocketBuffer::put_msg(void *msg, int32_t sz) {
    if (!check_resize(sz)) {
        LOG(ERROR) << "resize buffer over 10MB!";
        return false;
    }
    memcpy(buffer_ + end_pos_, msg, sz);
    end_pos_ += sz;
    //测试数据用
    //int32_t *age = (int32_t*)&buffer_[end_pos_ - sz + 4];

    return true;
}

void* SocketBuffer::get_msg(int32_t sz) {
    int total = end_pos_ - start_pos_;
    if (total < sz) {
        //数据不够
        return nullptr;
    }
    void *data = &buffer_[start_pos_];
    start_pos_ += sz;
    if (start_pos_ == end_pos_) {
        start_pos_ = end_pos_ = 0;
    }
    return data;

}

bool SocketBuffer::check_resize(int32_t sz) {
    if (free_size() >= sz) {
        return true;
    }
    while (free_size() < sz) {
        ++cur_step_;
    }
    LOG(INFO) << "cur step is:" << cur_step_;
    int32_t data_sz = end_pos_ - start_pos_;
    uint8_t *swp = (uint8_t *) malloc(buffer_size());

    memcpy(swp, &buffer_[start_pos_], data_sz);
    free(buffer_);
    buffer_ = swp;
    start_pos_ = 0;
    end_pos_ = data_sz;
    return true;

}

