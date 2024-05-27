#ifndef AVFRAMEQUEUE_H
#define AVFRAMEQUEUE_H
#include "queue.h"
#ifdef __cplusplus ///
extern "C"
{
// #include "libavutil/avutil.h"
// #include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}
#endif

class AVFrameQueue
{
public:
    // 构造函数
    AVFrameQueue();
    // 栖构函数
    ~AVFrameQueue();
    // 停止函数
    void Abort();
    // 推送函数
    int Push(AVFrame *Val);
    // 读取函数
    AVFrame *Pop(const int timeout);
    // 判断是否存在
    AVFrame *Front();
    // queue 的大小
    int Size();
private:
    // 清空资源
    void release();
    // AVFrame q
    Queue<AVFrame *> queue_;
};

#endif // AVFRAMEQUEUE_H
