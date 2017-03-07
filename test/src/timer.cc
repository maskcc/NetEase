/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "timer.h"
using namespace easynet;
//timer
    Timer::Timer() {

    }

    Timer::~Timer() {

    }

    uint64_t Timer::CreateTimer(uint64_t tick, const _OnTimerHandler& h) {
        return CreateTimer(tick, bind(h));
    }

    uint64_t Timer::CreateTimer(uint64_t tick, _OnTimerHandler &&h) {
        uint64_t t = GetSysTick() - this->start_sys_time_;
        //超时距离 startSysTime 的时间差为 t
        t += tick;
        _OnTimerHandler *pFunc = new _OnTimerHandler(std::move(h));
        timer_map_.insert(make_pair(t, pFunc));
        return t;
    }

    bool Timer::CancelTimer(uint64_t id) {

    }

    void Timer::CheckTimer() {

    }

    uint64_t Timer::GetSysTick() {
        return (uint64_t)(std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch()).count());

    }

    uint32_t Timer::GetNextExpireTime() {
        uint32_t tm = -1;
        if(!timer_map_.empty()) {
            tm = timer_map_.begin()->first - GetSysTick() - start_sys_time_;
        }
        tm = std::min(tm, 100U);
        return tm;

    }