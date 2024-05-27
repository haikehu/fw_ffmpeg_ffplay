#include "decodethread.h"

#include "log.h"

DecodeThread::DecodeThread(AVPacketQueue *packet_queue, AVFrameQueue *frame_queue)
    :packet_queue_(packet_queue),frame_queue_(frame_queue)
{

}

DecodeThread::~DecodeThread()
{

    if(thread_){
        Stop();
    }
    if(codec_ctx_){
        avcodec_close1(codec_ctx_);

    }
}

// 初始化解码器
int DecodeThread::Init(AVCodecParameters *par)
{
    if(!par) {
        LogError("Init par is null") ;
        return -1;
    }

    // 创建全局上下文
    codec_ctx_ = avcodec_alloc_context3(NULL);

    // 把参数和全局上下文绑定
    int ret = avcodec_parameters_to_context(codec_ctx_, par);
    if(ret < 0){
        av_strerror(ret, err2str,sizeof(err2str));
        LogError("avcodec_parameters_to_context failed, ret;%d, err2str:%s", ret, err2str);
        return -1;
    }
    const AVCodec *codec;
    //    if (codec_ctx_->codec_id == AV_CODEC_ID_H264){
    //     codec = avcodec_find_decoder_by_name("h264_qsv");
    // } else {
           // 根据codec_id 获取解码器 这里可以指定其它的解码器
        codec = avcodec_find_decoder(codec_ctx_->codec_id);
   // }
    if(!codec) {
        LogError("avcodec_find_decoder failed");
        return -1;
    }

    // 初始化解密器 和全局上下文进行绑定
    ret = avcodec_open2(codec_ctx_, codec ,NULL);
    if(ret < 0){
        av_strerror(ret, err2str,sizeof(err2str));
        LogError("avcodec _open2 failed, ret:%d, err2str:%s", ret, err2str);
        return -1;
    }

    LogInfo("Init finish");
    return 0;
}

int DecodeThread::Start()
{

    // 创建一个新的线程
    thread_= new std::thread(&DecodeThread::Run,this);
    if(!thread_) {
        LogError("new std::thread (&DecodeThread::Run, this) failed");
        return -1;
    }
    return 9;
}

int DecodeThread::stop()
{
    Thread::Stop();
    return 0;
}

void DecodeThread::Run()
{
    // 分配一个avframe 的容器
    AVFrame *frame = av_frame_alloc();
    LogInfo("DecodeThread: Run Info");
    while (abort_ !=1 ){
        // 如果frame queue 的大小大于了10 停顿 休息一会
        if (frame_queue_->Size() >100){
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            continue;
        }
        // 从packet queue 中取出数据 但是设置超时时间为1秒
        AVPacket *pkt = packet_queue_->Pop(1000);
        LogError("avcodec_send_packet packet_queue_ size %d", packet_queue_->Size());
        if(pkt) {
            // 如果成功取出 则发送到全局上下文中
            int ret = avcodec_send_packet(codec_ctx_, pkt);
            if (pkt->dts != 0){
                av_packet_free(&pkt);
            }
            LogInfo("ret = %d",ret);
            if(ret < 0){
                av_strerror(ret, err2str, sizeof(err2str));
                LogError("avcodec_send_packet failed, ret:%d, err2str;%s", ret, err2str);
                break;
            }
            // 读取解码后的frame
            while (true){
                // 把codec_ctx_ 中的信息设置到frame 中
                int ret2 = avcodec_receive_frame(codec_ctx_, frame);
                // 如果设置成功
                if(ret2 == 0) {
                    LogInfo("ret value is %d",ret2);
                    // 调用frame_queue_ 的push 函数 把 frame 推送的frame queue 中
                    frame_queue_->Push(frame);
                    LogInfo("%s frame queue size %d",codec_ctx_->codec->name, frame_queue_->Size());
                    continue;
                }else if(AVERROR(EAGAIN) == ret2) {
                 //   LogInfo("%s frame queue size %d",codec_ctx_->codec->name, frame_queue_->Size());
                    // IO 异常 表示资源暂时不可用
                    break;
                } else {
                    abort_ = 1;
                    LogError("avcodec_receive_frame failed, ret:%d, err2str;%s", ret2, err2str);
                    break;
                }
            }
        } else {
            //   LogInfo("not got packet");
        }
    }
    LogInfo("DecodeThread: Run Finsh");

}
