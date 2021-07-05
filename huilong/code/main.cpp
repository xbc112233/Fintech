#include <iostream>
#include "Order.h"
#include <vector>
#include <string>
#include <unistd.h>
using namespace std;

int main()
{
    vector<string> data = {"1","2","3"};
    Order order(20); 
    int i = 0;
    while (i < 100000) {
        order.OnOrder(data);
        usleep(20000);
    }   
    return 0;
}
