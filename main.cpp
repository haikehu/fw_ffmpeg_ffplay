#include <iostream>
#include "log.h"
#include <QApplication>
#include "demuxthread.h"
#include "avframequeue.h"
#include "decodethread.h"
#include "audiooutput.h"
#include "videooutput.h"
#include "avsync.h"
using namespace std;


// start init log file
// new thread
// create demux
// create queue
#undef main
int main(int argc, char *argv[])
{
    cout << "Hello word:" << endl;
    LogInit();
    //1. 解复用
    int ret = 0;

    AVPacketQueue audio_packet_queue;
    AVPacketQueue video_packet_queue;


    AVFrameQueue audio_frame_queue;
    AVFrameQueue video_frame_queue;

    AVSync avasync;
    avasync.Initclock();
    // 1. 解复用
    DemuxThread *demux_thread = new DemuxThread(&audio_packet_queue,&video_packet_queue);
    // 初始化解复用器
    ret =  demux_thread->Init(argv[1]);
    if (ret < 0){
        LogError("demux_thread init failed");
        return -1;
    }
    // 开始运行解复用器
    ret =  demux_thread->Start();
    if (ret < 0){
        LogError("demux_thread.Start() failed");
        return -1;
    }


    // 2. 解码 线程初始化
    // 音频
    DecodeThread *audio_decode_thread = new DecodeThread(&audio_packet_queue, &audio_frame_queue);
    ret = audio_decode_thread->Init(demux_thread->AudioCodeParameters());
    if(ret < 0) {
        LogError("audio_ decode_thread->Init() failed");
        return -1;
    }

    ret = audio_decode_thread->Start();
    if(ret < 0){
        LogError("audio_decode _thread->Start() failed");
        return -1;
    }


    // 视频
    DecodeThread *video_decode_thread = new DecodeThread(&video_packet_queue, &video_frame_queue);
    ret = video_decode_thread->Init(demux_thread->VideoCodeParameters());
    if(ret < 0) {
        LogError("video_ decode_thread->Init() failed");
        return -1;
    }

    ret = video_decode_thread->Start();
    if(ret < 0){
        LogError("video_decode _thread->Start() failed");
        return -1;
    }

  //  // // 初始化 audio 输出
    AudioParams audio_params = {0};
    memset(&audio_params,0,sizeof(AudioParams));
    audio_params.channels = demux_thread->AudioCodeParameters()->ch_layout.nb_channels;
    audio_params.channel_layout = &demux_thread->AudioCodeParameters()->ch_layout;
    audio_params.fmt = (enum AVSampleFormat)demux_thread->AudioCodeParameters()->format;
    audio_params.freq = demux_thread->AudioCodeParameters()->sample_rate;
    audio_params.frame_size =demux_thread->AudioCodeParameters()->frame_size;
   AudioOutput *audio_output = new AudioOutput(&avasync,demux_thread->AudioStreamTimeBase(), audio_params, &audio_frame_queue);
  //  AudioOutput *audio_output = new AudioOutput(audio_params, &audio_frame_queue);
    ret = audio_output->Init();
    if(ret < 0){
        LogError("audio_output->Init() failed");
        return -1;
    }


    // // 初始化 video 输出
    VideoOutput *video_outout = new VideoOutput(&avasync,
                                                demux_thread->VideoStreamTimeBase(),
                                                &video_frame_queue,
                                                demux_thread->VideoCodeParameters()->width,
                                                demux_thread->VideoCodeParameters()->height);
    // // 初始化 video 输出
    // VideoOutput *video_outout = new VideoOutput(&video_frame_queue,
    //                                             demux_thread->VideoCodeParameters()->width,
    //                                             demux_thread->VideoCodeParameters()->height);
    ret = video_outout->Init() ;
    if(ret < 0){
        LogError("video output->Init() failed");
        return -1;
    }
    video_outout->MainLoop();

    // 等待120秒
    std::this_thread::sleep_for(std::chrono::milliseconds(1200*1000));
    LogDebug("demux_thread->Stop");
    demux_thread->Stop();
    delete demux_thread;
    LogDebug("audio_decode_thread->stop");
    audio_decode_thread->stop();
    LogDebug("audio_decode_thread");
    delete audio_decode_thread;

    LogDebug("video_decode_thread->stop");
    video_decode_thread->stop();
    LogDebug("video_decode_thread");
    delete video_decode_thread;
    LogInfo("main finish");
    return 0;
}
