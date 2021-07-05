#include "firm_offer.h"
#include <fstream>
#include <sstream>
using namespace std;

FirmOffer::FirmOffer(const std::string &path, std::shared_ptr<Order> pOrder)
        :inputDataPath(path), mpOrder(pOrder)
{

}


void FirmOffer::ConstructLocationData()
{
    ifstream infile(inputDataPath);
    string line;
    while (getline(infile, line)) {
        vector<string> data;
        istringstream input(line);
        string tmp;
        while (getline(input, tmp, ',')) {
            data.push_back(tmp);
        }
        OnOrder(data);
    }   
}

void FirmOffer::OnOrder(const vector<string> &data)
{
    mpOrder->OnOrder(data);
}
