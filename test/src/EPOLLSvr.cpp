#include "EPOLLSvr.h"

//timer
    Timer::Timer()
    {

    }

    Timer::~Timer()
    {

    }

    uint64_t Timer::createTimer(uint64_t tick, const _OnTimerHandler& h)
    {
        return createTimer(tick, bind(h));
    }

    uint64_t Timer::createTimer(uint64_t tick, _OnTimerHandler &&h)
    {
        uint64_t t = getSysTick() - this->startSysTime;
        //超时距离 startSysTime 的时间差为 t
        t += tick;
        _OnTimerHandler *pFunc = new _OnTimerHandler(std::move(h));
        timerMap.insert(make_pair(t, pFunc));
        

        return t;
    }

    bool Timer::cancelTimer(uint64_t id)
    {

    }

    void Timer::checkTimer()
    {

    }

    uint64_t Timer::getSysTick()
    {
        return (uint64_t)(std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch()).count());

    }

    uint32_t Timer::getNextExpireTime()
    {
        uint32_t _tm = -1;
        if(!timerMap.empty())
        {
            _tm = timerMap.begin()->first - getSysTick() - startSysTime;
        }
        _tm = std::min(_tm, 100U);

        return _tm;

    }

//DataBuffer
DataBuffer::DataBuffer(int32_t _sz, char* ptr)
{
    buf.resize(_sz);
    memcpy(&buf[0], ptr, _sz);


}
