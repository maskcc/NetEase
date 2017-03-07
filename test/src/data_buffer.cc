/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "data_buffer.h"

using namespace easynet;
//DataBuffer
    DataBuffer::DataBuffer(int32_t s, int32_t sz) {
        buf_.resize(sz);
        sz_    = sz;
        sock_  = s;
        pos_   = 0;
    }
    DataBuffer::~DataBuffer(){
      //  LOG(INFO) << "DataBuffer Destructed! sz:" << sz_ << " sock_:" << sock_ << "pos:" << pos_;
    }
    int32_t DataBuffer::AddData(int32_t sz, const char * ptr) {
        //至少要先把长度发过来
        if(pos_ + sz > sz_){
            //错误, 数据长度不对
            return -1;
        }
        memcpy(&buf_[pos_], ptr, sz);
        pos_ += sz;
        return sz_ - pos_;
    }
    int32_t DataBuffer::NeedData() {
       // LOG(INFO)<< "sz_:" << sz_ << " pos_:" << pos_;
        int32_t len = sz_ - pos_;
        if(len < 0){
            return -1;
        }
        return len;

    }
    void DataBuffer::Resize(int32_t sz){
        buf_.resize(sz);
        sz_ = sz;
        
    }
    const void* DataBuffer::GetBuffPtr(){
        return static_cast<const void*>(&buf_[0]);        
    }
