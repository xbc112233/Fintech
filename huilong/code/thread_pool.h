#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <thread>
#include <functional>
#include <vector>
#include <list>
#include <mutex>
#include <future>


typedef std::function<int()> Function;

class ThreadPool
{
public:
    ThreadPool(int nThreads);
    ~ThreadPool();
public:
    class Task
    {
    public:
        Task(const Function &function);
        ~Task();

        void Run();

        void Wait();

        std::future<int> &GetFuture();

    private:
        std::future<int> mFuture;
        std::packaged_task<int()> mTask;
    };
    typedef std::shared_ptr<Task> TaskPtr;
public:
    int Submit(TaskPtr pTask);
    void ConsumeTask();
private:
    std::vector<std::thread*> mThreads;
    int mNThreads;
    std::list<TaskPtr> mTaskBuffer;
    std::mutex mMutex;    
};
typedef std::shared_ptr<ThreadPool> ThreadPoolPtr;
#endif
