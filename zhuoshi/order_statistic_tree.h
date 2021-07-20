#ifndef ORDER_STATISTIC_TREE_H
#define ORDER_STATISTIC_TREE_H
enum COLOR { RED, BLACK };

struct Node {
    int key;
    COLOR color;
    Node* p;
    Node* left;
    Node* right;
    int size;
    Node(int k, COLOR c = RED, Node* parent = nullptr, Node* l = nullptr, Node* r = nullptr, int s = 1) :key(k), color(c), p(parent), left(l), right(r), size(s) {}
};

class OrderStatisticTree
{
public:
    OrderStatisticTree(Node* r = nullptr);
    void insert(int key);
    bool find(int key) const;
    void remove(int key);
    void clear();
    int index(int i) const;
    int rank(int key) const;
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
    int rank(Node* root, int key, int sum = 0) const;
    

private:
    Node* root;

};

#endif
