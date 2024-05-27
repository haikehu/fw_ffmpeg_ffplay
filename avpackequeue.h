#ifndef AVPACKEQUEUE_H
#define AVPACKEQUEUE_H
#include "queue.h"
#ifdef __cplusplus ///
extern "C"
{
// #include "libavutil/avutil.h"
// #include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}
#endif
class AVPacketQueue
{
public:
    // 构造函数
    AVPacketQueue();
    // 栖构函数
    ~AVPacketQueue();
    // 终止
    void Abort();
    // 大小
    int Size();
    // 推送数据
    int Push(AVPacket *val);
    // 取数据
    AVPacket *Pop(const int timeout);
private:
    // 释放资源
    void release();
    // Q AVPacket
    Queue<AVPacket *> queue_;
};

#endif // AVPACKEQUEUE_H
