#include "firm_offer.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

FirmOffer::FirmOffer(const std::string &orderPath, std::shared_ptr<Order> pOrder, 
                     const string &traderPath, shared_ptr<Trader> pTrader)
        :mOrderInputDataPath(orderPath), mpOrder(pOrder), mTraderInputDataPath(traderPath), mpTrader(pTrader)
{
    dataPathList.resize(2);
    dataPathList[ORDER] = mOrderInputDataPath;
    dataPathList[TRADER] = mTraderInputDataPath;

}

void FirmOffer::ConstructLocationData(const int dataType)
{
    cout << "DEBUG dataType: " << dataType << " " << dataPathList[dataType]<<endl;
    ifstream infile(dataPathList[dataType]);
    string line;
    while (getline(infile, line)) {
        vector<string> data;
        istringstream input(line);
        string tmp;
        while (getline(input, tmp, ',')) {
            data.push_back(tmp);
        }
        if (dataType == ORDER)
            OnOrder(data);
        else if (dataType == TRADER) {
            OnTrader(data);
        }
    }   
}

void FirmOffer::OnOrder(const vector<string> &data)
{
    mpOrder->OnOrder(data);
}

void FirmOffer::OnTrader(const vector<string> &data)
{
    mpTrader->OnTrader(data);
}
