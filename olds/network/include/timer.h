/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   timer.h
 * Author: admin
 *
 * Created on 2017年3月7日, 下午1:26
 */

#ifndef TIMER_H
#define TIMER_H
#include "svr_define.h"
namespace easynet{
class Timer
    {
        //epoll_wait 是毫秒级别的timeout
        public:
            Timer();
            ~Timer();
        public:
            //创建timer, 返回timerID
            //tick 超时时间, 单位毫秒, handler 处理的函数
            uint64_t CreateTimer(uint64_t tick, const _OnTimerHandler& handler);
            uint64_t CreateTimer(uint64_t tick, _OnTimerHandler &&handler);
            //删除定时器
            bool CancelTimer(uint64_t id);
            //检查定时器
            void CheckTimer();
            uint32_t GetNextExpireTime();
        private: 
            //毫秒为单位
            uint64_t GetSysTick();

        private:
            int64_t MakeTimerID();
            std::map<uint64_t, _OnTimerHandler*> timer_map_;
            const uint64_t start_sys_time_ = GetSysTick();

    };
}


#endif /* TIMER_H */

