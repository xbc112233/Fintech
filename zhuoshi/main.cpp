#include "order_statistic_tree.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

#define TERT_DATA_FILE "data.list"

int main()
{
    ifstream  ifs(TERT_DATA_FILE);
    string line;
    shared_ptr<OrderStatisticTree> pOrderStatisticTree;
    pOrderStatisticTree.reset(new OrderStatisticTree());
    while (line != "EOF" && getline(ifs, line)) {
        cout << "test cmd: "<< line << endl;
        if (line[0] == 'i' && line[2] == 's') {
            for (int i = 0; i < line.length(); ++i) {
                if (line[i] >= '0' && line[i] <= '9') {
                    int num = line[i] - '0';
                    pOrderStatisticTree->insert(num);
                }
            }
            cout << "OPT insert succuss!" <<endl;
        }
        else if (line[0] == 'i' && line[2] == 'd') {
            std::size_t pos = line.find("index:");
            string numStr = line.substr(pos + 6);
            int res = pOrderStatisticTree->index(atoi(numStr.c_str()));   
            cout << "RESULT index(" << numStr << "): " << res << endl;
        }
        else if (line[0] == 'e' && line[1] == 'r') {
            std::size_t pos = line.find("erase:");
            string numStr = line.substr(pos + 6);
            pOrderStatisticTree->erase(atoi(numStr.c_str()));
            cout << "OPT erase(" << numStr << ") Success" <<endl; 
        }
        else if (line[0] == 'r') {
            std::size_t pos = line.find("rank:");
            string numStr = line.substr(pos + 5);
            auto res = pOrderStatisticTree->rank(atoi(numStr.c_str()));
            cout << "RESULT rank(" << numStr << ") lower: " << res.first << " upper: " << res.second << endl;
        }
        else if (line == "end") {
            cout << "OPT destroy success!" << endl;
            cout << "=================================" << endl;
            cout << "" <<endl;
            pOrderStatisticTree.reset(new OrderStatisticTree());
        }
        else {
            continue;
        }
    }

    return 0;
}
