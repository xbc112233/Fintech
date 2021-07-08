#ifndef FIRM_OFFER_H
#define FIRM_OFFER_H

#include <memory>
#include <vector>
#include <string>
#include "Order.h"
#include "Trader.h"

#define ORDER 0
#define TRADER 1

class FirmOffer
{
public:
    FirmOffer(const std::string &orderDataPath, std::shared_ptr<Order> pOrder, 
              const std::string &traderDataPath, std::shared_ptr<Trader> pTrader);
    void ConstructLocationData(const int dataType);
private:
    void OnOrder(const std::vector<std::string> &data);
    void OnTrader(const std::vector<std::string> &data);
    std::string mOrderInputDataPath;
    std::string mTraderInputDataPath;
    std::vector<std::string> dataPathList;
    std::shared_ptr<Order> mpOrder;
    std::shared_ptr<Trader> mpTrader;
};

#endif
