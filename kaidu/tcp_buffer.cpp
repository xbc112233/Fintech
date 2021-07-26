#include "tcp_buffer.h"
#include <algorithm>
#include <iostream>
using namespace std;


Buffer::Buffer(int maxBufferSize)
            :mMaxBufferSize(maxBufferSize)
{
    mNextOutputSeqId = 0;
}

Buffer::~Buffer()
{
    mDataBufferPtrList.clear();
    mDataOutputPtrList.clear();
    mNextOutputSeqId = 0;
}

string Buffer::Get()
{
    string res = "";
    for (auto it = mDataOutputPtrList.begin(); it != mDataOutputPtrList.end(); ++it) {
        shared_ptr<DataInfo> pData = *it;
        res += pData->data;
    }
    mDataOutputPtrList.clear();
    return res;
}

void Buffer::Put(shared_ptr<DataInfo> pDataInfo)
{
    int seqId = pDataInfo->seqId;
    cout << "DEBUG put seqId: " << seqId << " mNextOutputSeqId: " << mNextOutputSeqId << endl;
    if (seqId < mNextOutputSeqId) {
        return;
    }
    else if (seqId == mNextOutputSeqId) {
        cout << "DEBUG put seqId == mNextOutputSeqId: " << seqId << endl;
        UpdateOutputList(pDataInfo);
    }    
    else {
        UpdateBufferList(pDataInfo);
    }
}

void Buffer::UpdateOutputList(shared_ptr<DataInfo> pDataInfo)
{
    auto it = mDataBufferPtrList.begin();
    while (it != mDataBufferPtrList.end() && (*it)->seqId <= mNextOutputSeqId) {
        cout << "DEBUG UpdateOutputList push seqId: " << (*it)->seqId <<endl;
        mDataOutputPtrList.push_back(*it);
        ++it;
        mDataBufferPtrList.pop_front();
    }

    mDataOutputPtrList.push_back(pDataInfo);
    mNextOutputSeqId = pDataInfo->nextSeqId;
    it = mDataBufferPtrList.begin();
    while (it != mDataBufferPtrList.end() && (*it)->seqId == mNextOutputSeqId) {
        mDataOutputPtrList.push_back(*it);
        mNextOutputSeqId = (*it)->nextSeqId;
        ++it;
        mDataBufferPtrList.pop_front();
    }
}

void Buffer::UpdateOutputList()
{
    auto it = mDataBufferPtrList.begin();
    while (it != mDataBufferPtrList.end() && (*it)->seqId == mNextOutputSeqId) {
        mDataOutputPtrList.push_back(*it);
        mNextOutputSeqId = (*it)->nextSeqId;
        ++it;
        mDataBufferPtrList.pop_front();
    }

    it = mDataOutputPtrList.begin();
    while (it!= mDataOutputPtrList.end()) {
        if ((*it)->seqId < mNextOutputSeqId) {
            ++it;
            mDataOutputPtrList.pop_front();
        }
        else {
            break;
        }
    }
}

void Buffer::Print()
{

    auto it = mDataOutputPtrList.begin();
    while (it != mDataOutputPtrList.end()) {
        cout << "DEBUG output seqId:" <<(*it)->seqId<<" nextSeqId: " <<(*it)->nextSeqId 
            << " mNextOutputSeqId: "<< mNextOutputSeqId << endl;
        ++it;  
    }
    it = mDataBufferPtrList.begin();
    while (it != mDataBufferPtrList.end()) {
        cout << "DEBUG buffer seqId:" <<(*it)->seqId<<" nextSeqId: " <<(*it)->nextSeqId 
            << " mNextOutputSeqId: " << mNextOutputSeqId << endl;
        ++it;
    }

    cout << "=========================" << endl;
}

void Buffer::UpdateBufferList(shared_ptr<DataInfo> pDataInfo)
{
    auto &l = mDataBufferPtrList;
    auto it = lower_bound(l.begin(), l.end(), pDataInfo->seqId, 
        [](const shared_ptr<DataInfo> pDataInfo, int seqId) {
            return pDataInfo->nextSeqId <= seqId;
        });
    cout <<"DEBUG lower_bound it: " << (*it)->seqId <<" input seId: " << pDataInfo->seqId << endl;
    l.insert(it, pDataInfo);

    int lastSeqId = l.back()->nextSeqId;
    if (lastSeqId - pDataInfo->seqId > mMaxBufferSize) {
        cout << "DEBUG UpdateBufferList lastSeqId - pDataInfo->seqId > mMaxBufferSize seqId : " << pDataInfo->seqId << endl;
        mNextOutputSeqId = lastSeqId - mMaxBufferSize;
        return;
    }
    
    mNextOutputSeqId = max(mNextOutputSeqId, lastSeqId - mMaxBufferSize);
    it = mDataBufferPtrList.begin();
    //如果丢包超过阈值就pop掉最开头的
    //简单假设当前包序列是0,1,2,5,6,8，数据大小都为1，如果mMaxBufferSize = 4，那么当8进入buffer队列，需要把0,1,2,3都丢弃
    //并且更新mNextOutputSeqId，下次包3put的时候，直接丢弃,更新mNextOutputSeqId=4，等待SeqId >= 4的包，如果包的seqId为3，大小为2
    //则简单处理直接丢弃
    while (!mDataBufferPtrList.empty() && 
            (*(mDataBufferPtrList.begin()))->seqId < mNextOutputSeqId) {
        mDataBufferPtrList.pop_front();
    }

    UpdateOutputList();
    //mNextOutputSeqId = mNextOutputSeqId + 1;  
}
