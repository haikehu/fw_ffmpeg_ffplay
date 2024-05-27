#ifndef DECODETHREAD_H
#define DECODETHREAD_H
#include "thread.h"
#include "avpackequeue.h"
#include "avframequeue.h"
class DecodeThread : public Thread
{
public:
    // 构造函数
    DecodeThread(AVPacketQueue *packet_queue,AVFrameQueue *frame_queue);
    // 栖构函数
    ~DecodeThread();
    // 初始化解码器 根据AVCodeParameters
    int Init(AVCodecParameters *par);
    // 开始运行
    int Start();
    // 停止运行
    int stop();
    // 运行函数
    void Run();
private:
    // 错误信息保存
    char err2str[256] = {0};
    // 上下文
    AVCodecContext *codec_ctx_= NULL;
    // 上游paket queue
    AVPacketQueue *packet_queue_ = NULL;
    // 需要推送的下游的frame queue
    AVFrameQueue *frame_queue_ = NULL;
};

#endif // DECODETHREAD_H
