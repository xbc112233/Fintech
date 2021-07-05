#ifndef ORDER_H
#define ORDER_H

#include "thread_pool.h"
#include <vector>
#include <string>
#include <mutex>

class Order
{
public:
    Order(int maxThreadNum, const std::string &locationPath);
    void OnOrder(const std::vector<std::string> &data);

private:
    int OnOrderWorker(const std::vector<std::string> &data);
    ThreadPoolPtr mpThreadPool;
    std::list<int> orderTimeList;
    std::string mDataLocationPath;
    std::mutex mMutex;
};

#endif
