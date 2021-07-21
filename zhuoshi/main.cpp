#include "order_statistic_tree.h"
#include <iostream>
using namespace std;

int main()
{
    OrderStatisticTree t;
    t.insert(1);
    t.insert(1);
    t.insert(3);
    t.insert(3);
    t.insert(4);
    t.insert(4);
    t.insert(4);
    t.insert(8);
    t.insert(7);
    t.erase(3);
    cout << "index res: " << t.index(5) << endl;
    cout << "rank res: " <<t.rank(4).first << " " << t.rank(4).second << endl;
    return 0;
}
