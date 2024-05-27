#ifndef THREAD_H
#define THREAD_H

// 抽出线程 方便后面创建线程
#include <thread>
class Thread
{
public:
    // 构造函数
     Thread() {}
    // 栖构函数 释放线程数据
    ~Thread() {
        if(thread_){
            Thread::Stop();
        }
    }
    // 开始
    int Start() {}
    // 停止
    int Stop() {
        abort_= 1;
        if(thread_) {
            thread_->join();
            delete thread_;
            thread_= NULL;
        }
    }
    // 运行线程
    virtual void Run() = 0;
    protected:
    // 停止状态
      int abort_= 0;
    // 线程信息
       std::thread *thread_= NULL;

};
#endif // THREAD_H
