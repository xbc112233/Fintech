#include <iostream>
#include "Order.h"
#include "Trader.h"
#include <vector>
#include <string>
#include <unistd.h>
#include "firm_offer.h"
using namespace std;

void worker(FirmOffer &fo, int type)
{
    fo.ConstructLocationData(type);
}

//命令行参数，每次输出多少行
int main(int argc,char *argv[])
{
    std::shared_ptr<Order> pOrder;
    int outputMinSize = atoi(argv[1]);
    pOrder.reset(new Order(20, "ORDER_SSE.loc", outputMinSize));   

    std::shared_ptr<Trader> pTrader;
    pTrader.reset(new Trader(20, "TRADER_SSE.loc", outputMinSize));
    FirmOffer fo("../data/ORDER_SSE", pOrder, "../data/TRADE_SSE", pTrader);
    thread t1(worker, ref(fo), ORDER);
    thread t2(worker, ref(fo), TRADER);    
    t1.join();
    t2.join();
    return 0;
}
