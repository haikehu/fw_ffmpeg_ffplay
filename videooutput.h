#ifndef VIDEOOUTPUT_H
#define VIDEOOUTPUT_H
#include "avframequeue.h"
#include "avsync.h"
// #include <SDL2/SDL.h>
#ifdef __cplusplus ///
extern "C"
{
// #include "libavutil/avutil.h"
// #include "libavformat/avformat.h"
#include <SDL2/SDL.h>
// #include "libswresample/swresample.h"
}
#endif


class VideoOutput
{
public:
    VideoOutput(AVSync *avsync_, AVRational time_base, AVFrameQueue *frame_queue, int video_width, int video_height);
   // VideoOutput(AVFrameQueue *frame_queue, int video_width, int video_height);
    int Init();
    int MainLoop();
    void RefreshLoopWaitEvent(SDL_Event *event);
private:
    void videoRefresh(double *remaining_time);
    AVFrameQueue *frame_queue_= NULL;
    SDL_Event event_; // 事件
    SDL_Rect rect_;
    SDL_Window *win_=NULL;
    SDL_Renderer *renderer_ = NULL;
    SDL_Texture *texture_= NULL;
    int video_width_= 0;
    int video_height_= 0;
    uint8_t yuv_buf_= NULL;
    int yuv_buf_size_ = 0;
    AVSync *avsync_ = NULL;
    AVRational time_base_;
};

#endif // VIDEOOUTPUT_H
