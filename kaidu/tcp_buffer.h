#ifndef TCP_BUFFER_H
#define TCP_BUFFER_H
#include <string>
#include <memory>
#include <list>
#include <utility>
#include <unordered_set>

struct DataInfo
{
    int seqId;
    int size;
    int nextSeqId;
    std::string data;
    DataInfo(int seqId, std::string &data) {
        this->data = data;
        this->seqId = seqId;
        size = data.length();
        nextSeqId = seqId + size;
    
    }
};

class Buffer
{
public:
    Buffer(int maxBufferSize);
    std::string Get();
    void Put(std::shared_ptr<DataInfo> pDataInfo);
    void Print();
    ~Buffer();

private:
    void UpdateOutputList(std::shared_ptr<DataInfo> pDataInfo);
    void UpdateBufferList(std::shared_ptr<DataInfo> pDataInfo);
    void UpdateOutputList();
private:
    std::list<std::shared_ptr<DataInfo>> mDataBufferPtrList;
    std::list<std::shared_ptr<DataInfo>> mDataOutputPtrList;
    //如果当前输入的data的seqId = nextOutputSeqId,首先插入对应的dataBufferList
    //并且从头检查dataBufferList，pop掉可以输出的data(nextSeqId比gapList.front()小)，放入dataOutputList
    int mNextOutputSeqId;
    int mMaxBufferSize; //超过这个size就要丢弃
};

#endif
