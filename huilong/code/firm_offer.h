#ifndef FIRM_OFFER_H
#define FIRM_OFFER_H

#include <memory>
#include <vector>
#include <string>
#include "Order.h"

class FirmOffer
{
public:
    FirmOffer(const std::string &path, std::shared_ptr<Order> pOrder);
    void ConstructLocationData();
private:
    void OnOrder(const std::vector<std::string> &data);
    std::string inputDataPath;
    std::shared_ptr<Order> mpOrder;
};

#endif
