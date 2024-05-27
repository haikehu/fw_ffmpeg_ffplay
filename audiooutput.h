#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H
#include "avframequeue.h"
#include "avsync.h"
#ifdef __cplusplus ///
extern "C"
{
#include <SDL2/SDL.h>
#include "libswresample/swresample.h"
}
#endif

typedef struct AudioParams{
    int freq;    //采样率
    int channels; // 通道
    AVChannelLayout *channel_layout; //
    enum AVSampleFormat fmt; //
    int frame_size; // 当前输入的Q 的大小
}AudioParams;

class AudioOutput
{
public:
    // 构造函数 输入为Frame Q 和 output 的参数
    AudioOutput(AVSync *avsync, AVRational time_base,const AudioParams &audio_params,AVFrameQueue *frame_queue);AudioOutput();
 //   AudioOutput(const AudioParams &audio_params,AVFrameQueue *frame_queue);AudioOutput();
    // 栖构函数
    ~AudioOutput();
    // 初始化
    int Init();
    // 关闭
    int DeInit();
public:
    AudioParams src_tgt_; // 解码后的参数
    AudioParams dst_tgt_; // SDL实际输出的格式
    AVFrameQueue *frame_queue_= NULL; // 输入数据的Q
    AVRational time_base_;

    struct SwrContext *swr_ctx_= NULL; // 全局上下文
    uint8_t *audio_buf_=NULL;  // 视频需要的读取的缓存
    uint8_t *audio_buf1_= NULL; // 输出需要的缓存
    uint32_t audio_buf_size = 0;  // 记录当前缓存的大小
    uint32_t audio_buf1_size = 0; // 记录需要输出缓存的大小
    uint32_t audio_buf_index = 0; // 缓存的索引
    AVSync *avsync_ = NULL;
    int64_t pts_ = AV_NOPTS_VALUE;
};
#endif // AUDIOOUTPUT_H
