#ifndef AVSYNC_H
#define AVSYNC_H
// #include "chrono"
#include <chrono>
#include <ctime>
#include <math.h>

using namespace std::chrono;
class AVSync
{
public:
    AVSync(){};
    void Initclock(){
        SetClock(NAN);// 数学对比是一个无效值
    }
    void SetClockAt(double pts, double time){
        pts_ = pts;
        pts_drift_= pts_ - time;
    }
    double GetClock(){
        double time = GetMicroseconds() /1000000.0;
        return pts_drift_ + time;
    }
    void SetClock(double pts) {
        double time = GetMicroseconds()/1000000.0; // us -> s
        SetClockAt(pts,time);
    }
    time_t GetMicroseconds(){
        system_clock::time_point time_point_new = system_clock::now(); // 获取当前时间
        system_clock::duration duration = time_point_new.time_since_epoch();
        time_t us = duration_cast<microseconds>(duration).count(); // 获取微妙
        return us;
    }
    double pts_= 0;
    double pts_drift_= 0; //相对时间
};

#endif // AVSYNC_H
