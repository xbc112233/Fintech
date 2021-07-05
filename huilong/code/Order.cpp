#include "Order.h"
#include <iostream>
using namespace std;

Order::Order(int maxThreadNum)
{
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
    for (int i = 0; i < data.size(); ++i) {
        cout << data[i] << ",";
    }
    cout <<endl;
    return 0;
}
