#include "common_util.h"
#include "Order.h"
#include <iostream>
#include <algorithm>
#include <mutex>
using namespace std;


Order::Order(int maxThreadNum, const string &locationPath, int minSize)
        :mDataLocationPath(locationPath)
{
    mpThreadPool.reset(new ThreadPool(maxThreadNum));
    //onSortedOrder 一次性展示的窗口大小，可以通过配置来扩展，此处hard-code
    mpSortThreadPool.reset(new ThreadPool(maxThreadNum));
    mSortedOutputMinSize = minSize;
}

Order::~Order()
{
    mSeqInfoMap.clear();   
}

void Order::OnOrder(const std::vector<std::string> &data)
{
    ThreadPool::TaskPtr pTask(std::make_shared<ThreadPool::Task>(
            std::bind(
                &Order::OnOrderWorker,
                this, data)));
    mpThreadPool->Submit(pTask);

    int channel = stoi(data[3]);
    unique_lock<mutex> lock(mMutex);
    if (mSeqInfoMap.find(channel) == mSeqInfoMap.end()) {
        mSeqInfoMap[channel] = make_shared<SeqInfo>();
        mSeqInfoMap[channel]->start = 1;
        mSeqInfoMap[channel]->channel = channel;
    }
    int seqId = stoi(data[data.size() - 1]);
    ThreadPool::TaskPtr pSortTask(std::make_shared<ThreadPool::Task>(
            std::bind(
                &Order::OnSortedOrderWorker,
                this, mSeqInfoMap[channel], seqId, data)));

    mpSortThreadPool->Submit(pSortTask);
    lock.unlock();   
}

int Order::OnOrderWorker(const std::vector<std::string> &data)
{
    unique_lock<mutex> lock(mMutex);
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
    //mDataLocationPath = "ORDER_SSE.loc";
//    cout << "DEBUG mDataLocationPath: " << mDataLocationPath << " dataStr: " << dataStr << " rowNum: " << rowNum <<endl;
    CommonUtil::InsertByLineNumber(mDataLocationPath, dataStr, rowNum); 
    lock.unlock();
    return SUCC;
}

void Order::OutputList(std::list<std::vector<std::string>> &buffDataList)
{
    cout << "================output Order Sort begin!==================" << endl;
    for (auto it = buffDataList.begin(); it != buffDataList.end(); ++it) {
        string tmp = "";
        for (int i = 0; i < it->size(); ++i) {
            tmp += (*it)[i];
            tmp += ",";
        }
        cout << tmp << endl;
    }
    cout << "===========output Order Sort end!!!!!=================" <<endl;
    cout << endl;
}

int Order::OnSortedOrderWorker(std::shared_ptr<SeqInfo> pSeqInfo, int seqId, const vector<string> &data)
{
    unique_lock<mutex> lock(pSeqInfo->mSortMutex);
    auto &seqInfo = *pSeqInfo;
    auto sit = seqInfo.gap.find(seqId);
    auto &l = seqInfo.seqList;
    auto &gap = seqInfo.gap;
    auto &buffList = seqInfo.buffDataList;
    bool needUpdateGap = true;
    //第一步检查当前的seqId是否在gap的set里，如果在就删除
    if (sit != gap.end()) {
        needUpdateGap = false;
        gap.erase(sit);
    }

    //第二步要插入新来的数据到对应的位置，seqId和data都要
    auto it = upper_bound(l.begin(), l.end(), seqId);
    auto itt = it;
    int start = seqInfo.start;
    if (itt != l.begin()) {
        --itt;
        start = *(itt) +1;
    }
    l.insert(it, seqId);
    
    int dist = distance(l.begin(), it) - 1;
    auto bit = seqInfo.buffDataList.begin();
    while (dist--) {
        bit++;
    }
    buffList.insert(bit, data);

    //第三步准备输出可以输出的SortedResult
    //可以输出的情况分两种
    //1. gap空了，然后现有的seqList长度大于mSortedOutputMinSize,输出seqList[begin, end]
    //2. gap不空,但*(gap.begin()) - start >= mSortedOutputMinSize，输出seqList[begin, gap.begin())
    if (gap.empty() && l.size() >= mSortedOutputMinSize){
        //输出先把前面N个可以输出的节点splice到l2,并且更新start
        seqInfo.start = l.back() + 1;
        OutputList(buffList);
        l.clear();
        buffList.clear();
        return SUCC;
    }
    else if (!gap.empty() && *(gap.begin()) - seqInfo.start >= mSortedOutputMinSize) {
        seqInfo.start = *(gap.begin());
        std::list<std::vector<std::string>> outputBuffList;
        auto it = upper_bound(l.begin(), l.end(), *(gap.begin()));
        int dist = distance(l.begin(), it);
        auto bit = seqInfo.buffDataList.begin();
        while (dist--) {
            bit++;
        }
        outputBuffList.splice(outputBuffList.begin(), buffList, buffList.begin(), bit);
        OutputList(outputBuffList);
        list<int> outputSeqIdList;
        outputSeqIdList.splice(outputSeqIdList.begin(), l, l.begin(), it);
        return SUCC;
    }

    //新增当前seqId引入变化的gap
    if (needUpdateGap) {
        for (int i = start; i < seqId; ++i) {
            gap.insert(i);
        }
    }
    //lock.unlock();
    return SUCC;    
}
