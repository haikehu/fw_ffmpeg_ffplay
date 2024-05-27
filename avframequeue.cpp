#include "avframequeue.h"

#include "log.h"

AVFrameQueue::AVFrameQueue()
{

}

AVFrameQueue::~AVFrameQueue()
{

}

void AVFrameQueue::Abort()
{
    release();
    queue_.abort();
}

// 推送消息
int AVFrameQueue::Push(AVFrame *val)
{
    AVFrame *tmp_frame = av_frame_alloc();
    av_frame_move_ref(tmp_frame,val);
    return queue_.Push(tmp_frame);
}

// 读取消息
AVFrame *AVFrameQueue::Pop(const int timeout)
{

    // 分配av 容器
    AVFrame *tmp_frame = av_frame_alloc();
    // 取出
    int ret = queue_.Pop(tmp_frame,timeout);
    if(ret < 0){
        if(ret == -1){
            LogError("AVPacketQueue::Pop failed");
        }
    }
    return tmp_frame;
}

// 判断是否存在数据
AVFrame *AVFrameQueue::Front()
{

    AVFrame *tmp_frame = av_frame_alloc();
    int ret = queue_.Front(tmp_frame);
    if(ret < 0){
        if(ret == -1){
            LogError("AVPacketQueue::Pop failed");
        }
    }
    return tmp_frame;
}

// 判断Q剩下的大小
int AVFrameQueue::Size()
{
    return queue_.size();
}


// 清空内容
void AVFrameQueue::release()
{
    while (true){
        AVFrame *frame = NULL;
        int ret = queue_.Pop(frame, 1);
        if(ret < 0){
            break;
        } else {
            av_frame_free(&frame);
            continue ;
        }
    }
}
