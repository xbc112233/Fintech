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
    //t.remove(8);
    t.insert(7);
    //cout << t << endl;
//    t.printsize(cout);
    cout << "th\t:\t" << t.index(5) << endl;
    cout << "\t:\t" << t.rank(8) << endl;
    return 0;
}
