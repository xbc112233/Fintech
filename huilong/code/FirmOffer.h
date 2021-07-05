#ifndef FIRMOFFER_H
#define FIRMOFFER_H

#include <memory>
#include <string>
#include "Order.h"

class FirmOffer
{
public:
    FirmOffer(const std::string &path, std::shared_ptr<Order> mpOrder);
    void ConstructLocationData();
private:
    void OnOrder();
    std::string inputDataPath;
    std::shared_ptr<Order> mpOrder;
};

#endif
