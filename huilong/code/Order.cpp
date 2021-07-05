#include "common_util.h"
#include "Order.h"
#include <iostream>
#include <algorithm>
using namespace std;

Order::Order(int maxThreadNum, const string &locationPath)
        :mDataLocationPath(locationPath)
{
    cout << "DEBUG mDataLocationPath: " << mDataLocationPath << endl;
    mpThreadPool.reset(new ThreadPool(maxThreadNum));
}

void Order::OnOrder(const std::vector<std::string> &data)
{
    ThreadPool::TaskPtr pTask(std::make_shared<ThreadPool::Task>(
            std::bind(
                &Order::OnOrderWorker,
                this, data)));
    mpThreadPool->Submit(pTask);
}

int Order::OnOrderWorker(const std::vector<std::string> &data)
{
    //这里不用加锁，因为外面线程池会hang住等这个函数全部执行完，有优化空间
    //unique_lock<mutex> lock(mMutex);
    const string &timeStr = data[0];
    int timeSuffix = stoi(timeStr.substr(10));
    //int timeSuffix = stoi(data[0]);
    auto &l = orderTimeList;
    //cout << "DEBUG orderTimeList size: " << orderTimeList.size()<<endl;
    auto it = lower_bound(l.begin(), l.end(), timeSuffix);
    int rowNum = distance(l.begin(), it) + 1;
    l.insert(it, timeSuffix);
    string dataStr = data[0] ;
    for (int i = 1; i < data.size(); ++i) {
        dataStr += ",";
        dataStr += data[i];
    }
    mDataLocationPath = "ORDER_SSE.loc";
    CommonUtil::InsertByLineNumber(mDataLocationPath, dataStr, rowNum); 
    //lock.unlock();
    return SUCC;
}
