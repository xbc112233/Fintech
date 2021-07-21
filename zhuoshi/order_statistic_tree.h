#ifndef ORDER_STATISTIC_TREE_H
#define ORDER_STATISTIC_TREE_H

#include <utility>
#include <unordered_map>

enum COLOR { RED, BLACK };

struct Node {
    int key;
    COLOR color;
    Node* p;
    Node* left;
    Node* right;
    int size;
    int repeatNum;
    Node(int k, COLOR c = RED, Node* parent = nullptr, Node* l = nullptr, Node* r = nullptr, int s = 1) :key(k), color(c), p(parent), left(l), right(r), size(s), repeatNum(1) {}
};

class OrderStatisticTree
{
public:
    OrderStatisticTree(Node* r = nullptr);
    void insert(int key);
    bool find(int key) const;
    void erase(int key);
    void clear();
    int index(int i) const;
    std::pair<int, int> rank(int key);
    ~OrderStatisticTree();
private:
    void leftR(Node* n);
    void rightR(Node* n);
    void keep(Node* tokeep);
    Node* getKey(int key) const;
    Node* getMin(Node* t);
    Node* getNext(Node* t);
    void dkeep(Node* x, Node* px);
    void clear(Node* root);
    int index(Node* root, int i) const;
//    int rank(int key) const;
    int rank(Node* root, int key, int sum, int &num) const;
private:
    Node* root;
    std::unordered_map<int, int> m;

};

#endif
