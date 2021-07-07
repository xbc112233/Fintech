#include "thread_pool.h"
#include <iostream>
#include <unistd.h>
using namespace std;

ThreadPool::Task::Task(const Function &function)
{
    mTask = std::packaged_task<int()>(function);
    mFuture = mTask.get_future();
}

ThreadPool::Task::~Task()
{
}

void ThreadPool::Task::Run()
{
    mTask();
}

void ThreadPool::Task::Wait()
{
    auto& future = GetFuture();
    if (future.valid())
        future.wait();
}

std::future<int> &ThreadPool::Task::GetFuture()
{
    return mFuture;
}

ThreadPool::ThreadPool(int nThreads)
    :mNThreads(nThreads)
{
    for (int i = 0; i < mNThreads; i++)
    {
        std::thread *pThread = new std::thread(&ThreadPool::ConsumeTask, this);
        mThreads.push_back(pThread);
        //cout << "ThreadPool init pThread: " << i <<endl;
    }
}

ThreadPool::~ThreadPool()
{
    for (int i = 0; i < mNThreads; i++)
    {
        mThreads[i]->join();
        delete mThreads[i];
    }
    mTaskBuffer.clear();
}

int ThreadPool::Submit(TaskPtr pTask)
{
    std::unique_lock<std::mutex> ulock(mMutex);
    mTaskBuffer.push_back(pTask);
    //cout << "DEBUG Submit mTaskBuffer size: " << mTaskBuffer.size()<<endl;
    ulock.unlock();
    return 0;
}

void ThreadPool::ConsumeTask()
{
    while (1)
    {
        std::unique_lock<std::mutex> ulock(mMutex);
        if (mTaskBuffer.size() > 0) {
            TaskPtr pTask = mTaskBuffer.front();
            mTaskBuffer.pop_front();
            ulock.unlock();
            pTask->Run();
            //cout << "DEBUG ConsumeTask get mMutex mTaskBuffer size: " << mTaskBuffer.size() << endl;
            pTask.reset();
        }
        else {
            ulock.unlock();
        }
        usleep(20000);
    }
}

