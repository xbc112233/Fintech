#include "tcp_buffer.h"
#include <iostream>
using namespace std;

int main()
{
    Buffer buf(5);
    string str = "2222";
    std::shared_ptr<DataInfo> pDataInfo(new DataInfo(5, str));
    buf.Put(pDataInfo);

    str = "0000";
    pDataInfo.reset(new DataInfo(0, str));
    buf.Put(pDataInfo);

    str = "1";
    pDataInfo.reset(new DataInfo(4, str));
    buf.Put(pDataInfo);
//
    //str = "0";
    //pDataInfo.reset(new DataInfo(0, str));
    //buf.Put(pDataInfo);

    //
    //str = "6";
    //pDataInfo.reset(new DataInfo(6, str));
    //buf.Put(pDataInfo);
    //buf.Print();

    //str = "3";
    //pDataInfo.reset(new DataInfo(3, str));
    //buf.Put(pDataInfo);

    //buf.Print();
    //str = "4";
    //pDataInfo.reset(new DataInfo(4, str));
    //buf.Put(pDataInfo);
    //buf.Print();

    //str = "5";
    //pDataInfo.reset(new DataInfo(5, str));
    //buf.Put(pDataInfo);
    //buf.Print();


    //str = "1";
    //pDataInfo.reset(new DataInfo(1, str));
    //buf.Put(pDataInfo);

    string res = buf.Get();
    cout << "exec get res: " << res << endl;
    return 0;
}
