/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   safe_queue.h
 * Author: admin
 *
 * Created on 2017年3月7日, 上午11:20
 */

#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H
#include "svr_define.h"
namespace easynet{
//线程安全的消息队列 SafeQueue
//放入的参数应该是智能指针
template <class T>
class SafeQueue
    {
        public:
            SafeQueue()
            {
                stop_ = false;
            }
            ~SafeQueue()
            {

            }
            void init(int32_t sz)
            {
                max_sz_ = sz;

            }

            bool full()
            {
                std::unique_lock<std::mutex> locker(mt_);
                return msg_queue_.size() >= max_sz_;

            }
            bool empty()
            {
                std::unique_lock<std::mutex> locker(mt_);
                return msg_queue_.empty();

            }
            bool put(const T &msg)
            {   
                std::unique_lock<std::mutex> locker(mt_);
                auto cb = [this]{//服务没停, 空间没满就能一直put
                    return stop_ || not_full() ;
                };  
                not_full_.wait(locker, cb);
                if(stop_){
                    return false; //已经停止
                }   

                msg_queue_.push(msg);
                not_empty_.notify_one();

                return true;
            }

            bool pour(std::vector<T> &vec)
            {
                std::unique_lock<std::mutex> locker(mt_);
                auto cb = [this]{//服务没停, 空间没空就能一直获取消息
                    return stop_ || not_empty() ;
                };
                not_empty_.wait(locker, cb);
                if(stop_){
                    return false; //已经停止
                }

                //msg = std::move(msgQ);
                while(!msg_queue_.empty()){ 
                    vec.push_back(msg_queue_.back());
                    msg_queue_.pop(); 
                }
                not_full_.notify_one();

                return true;
            }

        private:
            bool not_full()
            {
                return msg_queue_.size() < max_sz_;
            }

            bool not_empty()
            {
                return !msg_queue_.empty();
            }

        private:
            bool stop_;         //是否已经停止
            uint32_t max_sz_;     //队列最大消息数量
            std::queue<T> msg_queue_; //消息队列
            std::mutex mt_;      //互斥变量
            std::condition_variable not_empty_;
            std::condition_variable not_full_;

    };

}

#endif /* SAFE_QUEUE_H */

