#ifndef DEMUXTHREAD_H
#define DEMUXTHREAD_H
#include "thread.h"
#include "avpackequeue.h"
#ifdef __cplusplus ///
extern "C"
{
#include "libavutil/avutil.h"
#include "libavformat/avformat.h"
}
#endif
// 解复用器 主要是把视频文件解析出来 分为audio 和 video 分别处理
class DemuxThread : public Thread
{
public:
    // 构造函数
    DemuxThread(AVPacketQueue *audio_queue_,AVPacketQueue *video_queue_);
    // 栖构函数
    ~DemuxThread();
    // 复用器初始化
    int Init(const char *url);
    // 开始运行
    int Start();
    // 停止
    int Stop();
    // 运行
    void Run();
    // 声音的参数
    AVCodecParameters *AudioCodeParameters();
    // 视频的参数
    AVCodecParameters *VideoCodeParameters();

    //
    AVRational AudioStreamTimeBase();
    AVRational VideoStreamTimeBase();
private:
    // 错误信息
    char err2str[256] = {0};
    // 文件地址
    std::string url_; // 文件名
    // 音频Q
    AVPacketQueue *audio_queue_ = NULL;
    // 视频Q
    AVPacketQueue *video_queue_ = NULL;
    // 全局上下文
    AVFormatContext *ifmt_ctx_= NULL;
    // 判断音频值
    int audio_index_ = -1;
    // 判断视频值
    int video_index_ = -1;
};

#endif // DEMUXTHREAD_H
