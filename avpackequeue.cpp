#include "avpackequeue.h"
#include "log.h"

AVPacketQueue::AVPacketQueue()
{

}

AVPacketQueue::~AVPacketQueue()
{

}

void AVPacketQueue::Abort()
{
    release();
    queue_.abort();
}

// void AVPacketQueue::Release()
// {
//     // fix me
//     while (true){
//         AVPacket *pkt = NULL;
//         int ret = queue_.Pop(pkt,1);
//         if(ret >= 0) {
//             av_packet_free(&pkt);
//             continue;

//         }else{
//             break;
//         }
//     }

// }

int AVPacketQueue::Size()
{
    return queue_.size();
}

int AVPacketQueue::Push(AVPacket *val)
{
    // 分配一个av packet 包
    AVPacket *tmp_pkt = av_packet_alloc();
    // 把传递进来的数据丢给tmp_pkt
    av_packet_move_ref(tmp_pkt,val);
    // 把数据写入队列中
    return queue_.Push(tmp_pkt);
}

AVPacket *AVPacketQueue::Pop(const int timeout)
{
    AVPacket *tmp_pkt = NULL;
    // 把数据取出来写入tmp_pkt
    int ret = queue_.Pop(tmp_pkt, timeout);
    // 0 表示成功
    if(ret < 0){
        if(ret == -1)
            LogError("AVPacketQueue::Pop failed");
    }
    return tmp_pkt;
}

void AVPacketQueue::release()
{
    while (true){
        AVPacket *packet = NULL;
        int ret = queue_.Pop(packet, 1);
        if(ret < 0){
            break;
        } else {
            // 清空包中的数据
            av_packet_free(&packet);
            continue ;
        }
    }
}
