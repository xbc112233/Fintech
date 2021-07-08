#ifndef TRADER_H
#define TRADER_H

#include "thread_pool.h"
#include <vector>
#include <string>
#include <mutex>
#include <unordered_set>
#include <unordered_map>
#include <set>

class Trader
{
public:
    Trader(int maxThreadNum, const std::string &locationPath, int minSize);
    ~Trader();
    void OnTrader(const std::vector<std::string> &data);
    void OnSortedTrader(const std::vector<std::string> &data);
    struct SeqInfo
    {
        std::set<int> gap;
        std::list<int> seqList;
        int channel;
        int start;
        std::list<std::vector<std::string>> buffDataList; //此处简单拷贝了,数据顺序和seqList对齐
        std::mutex mSortMutex;
    };
private:
    int OnTraderWorker(const std::vector<std::string> &data);
    int OnSortedTraderWorker(std::shared_ptr<SeqInfo> pSeqInfo, int seqId, const std::vector<std::string> &data);
    void OutputList(std::list<std::vector<std::string>> &buffDataList);

    ThreadPoolPtr mpThreadPool;
    ThreadPoolPtr mpSortThreadPool;
    std::list<int> orderTimeList;
    std::string mDataLocationPath;
    std::mutex mMutex;
    std::unordered_map<int, std::shared_ptr<SeqInfo>> mSeqInfoMap;
    int mSortedOutputMinSize;
};

#endif
