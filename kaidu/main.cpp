#include "tcp_buffer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#define TERT_DATA_FILE "data.list"

using namespace std;

int main()
{
    ifstream  ifs(TERT_DATA_FILE);
    string line;
    shared_ptr<Buffer> pBuffer;
    while (line != "EOF" && getline(ifs, line)) {
        if (line[0] == 'P') {
            int idx = 4;
            while (idx < line.length()) {
                string str = "";
                while (line[idx] != '_') {
                    str += line[idx];
                    ++idx;
                }
                ++idx;
                string seqIdStr = "";
                while (line[idx] >= '0' && line[idx] <= '9') {
                    seqIdStr += line[idx];
                    ++idx;
                }
                int seqId = atoi(seqIdStr.c_str());
                shared_ptr<DataInfo> pData(new DataInfo(seqId, str));
                cout<<endl;
                 cout << "OPT Put data:" << str << " seqId: " << seqId  <<endl;
                cout << "~~~~~~~~~~~~~~~~~~" <<endl;
                pBuffer->Put(pData);
                if (line[idx] == ',') {
                    ++idx;
                    continue;
                }

            }
        }
        else if (line[0] == 'I') {
            cout << "Test cmd Init!" << endl;
            int idx = 5;
            string bufSizeStr = "";
            while (line[idx] >= '0' && line[idx] <= '9') {
                bufSizeStr += line[idx];
                ++idx;
            }
            
            pBuffer.reset(new Buffer(atoi(bufSizeStr.c_str())));
        }
        else if (line[0] == 'G') {
            cout << "Test cmd Get" << endl;
            string res = pBuffer->Get();
            cout << "exec get res: " << res << endl;
        }
        else if (line == "end") {
            cout << "OPT destroy success!" << endl;
            cout << "=================================" << endl;
            cout << "" <<endl;
            pBuffer.reset();
        }
        else {
            continue;
        }
    }

    return 0;
    //Buffer buf(5);
    //string str = "2222";
    //std::shared_ptr<DataInfo> pDataInfo(new DataInfo(5, str));
    //buf.Put(pDataInfo);

    //str = "0000";
    //pDataInfo.reset(new DataInfo(0, str));
    //buf.Put(pDataInfo);

    //str = "1";
    //pDataInfo.reset(new DataInfo(4, str));
    //buf.Put(pDataInfo);
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
}
