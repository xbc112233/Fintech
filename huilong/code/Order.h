#include "ThreadPool.h"
#include <vector>
#include <string>

class Order
{
public:
    Order(int maxThreadNum);
    void OnOrder(const std::vector<std::string> &data);

private:
    int OnOrderWorker(const std::vector<std::string> &data);
    ThreadPoolPtr mpThreadPool;
};
