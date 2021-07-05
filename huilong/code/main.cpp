#include <iostream>
#include "Order.h"
#include <vector>
#include <string>
#include <unistd.h>
#include "firm_offer.h"
using namespace std;

int main()
{
    std::shared_ptr<Order> pOrder;
    pOrder.reset(new Order(20, "ORDER_SSE.loc"));   
    FirmOffer fo("../data/ORDER_SSE", pOrder);
    fo.ConstructLocationData();
    return 0;
}
