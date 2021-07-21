#include <iostream>
#include <string>
#include <fstream>
#include <functional>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <vector>
#include "order_statistic_tree.h"

using namespace std;

OrderStatisticTree::OrderStatisticTree(Node* r)
                    :root(r)
{
    
}

void OrderStatisticTree::insert(int key)
{
    Node* tr = root;
    Node* ti = nullptr;
    m[key] += 1;
    while (tr) {
        ++(tr->size);
        ti = tr;
        if (tr->key < key)
            tr = tr->right;
        else {
            if (tr->key == key) {
                tr->repeatNum++;
            }
            tr = tr->left;
        }
    }
    if (!ti)
        root = new Node(key, BLACK);
    else {
        Node* tokeep = new Node(key, RED, ti);
        if (ti->key < key)
            ti->right = tokeep;
        else
            ti->left = tokeep;
        keep(tokeep);
    }
}

bool OrderStatisticTree::find(int key) const
{
    return getKey(key) != nullptr;
}

void OrderStatisticTree::clear()
{
     if (!root)
         return;
     clear(root->right);
     clear(root->left);
     delete root;
}

void OrderStatisticTree::erase(int key)
{
    Node* r = getKey(key);
    int color;
    Node* x = nullptr;
    Node* px = nullptr;
    if (!r)
        return;
    color = r->color;
    if (!r->left && !r->right) {
        x = nullptr;
        px = r->p;
        if (!px) {
            root = nullptr;
            delete r;
            return;
        }
        else {
            if (px->left == r) {
                px->left = x;
            }
            else {
                px->right = x;
            }
        }
    }
    else if (!r->left) {
        x = r->right;
        px = r->p;
        if (!px) {
            root = x;
        }
        else {
            if (px->right == r) {
                px->right = x;
            }
            else {
                px->left = x;
            }
        }
        x->p = px;
    }
    else if (!r->right) {
        x = r->left;
        px = r->p;
        if (!px) {
            root = x;
        }
        else {
            if (px->right == r) {
                px->right = x;
            }
            else {
                px->left = x;
            }
        }
        x->p = px;
    }
    else {
        Node* nr = getMin(r->right);    //nr->left==nullptr
        color = nr->color;              // nr->p != nullptr

        nr->size = r->size;

        x = nr->right;
        px = nr->p;
        if (px->left == nr) {
            px->left = x;
        }
        else {
            px->right = x;
        }
        if (x) {
            x->p = px;
        }
        if (px == r)
            px = nr;
        if (!r->p) {
            root = nr;
        }
        else if (r->p->left == r) {
            r->p->left = nr;
        }
        else {
            r->p->right = nr;
        }
        nr->p = r->p;
        nr->left = r->left;
        nr->left->p = nr;
        nr->right = r->right;
        if (nr->right)
            nr->right->p = nr;
    }
    delete r;

    Node* tmppx = px;
    while (tmppx) {
        --(tmppx->size);
        tmppx = tmppx->p;
    }

    if (color == BLACK) {
        dkeep(x, px);
    }
}

int OrderStatisticTree::index(int i) const
{
    int l = root->left ? root->left->size : 0;
    int s = l + 1;
    if (s == i)
        return root->key;
    else if (s > i)
        return index(root->left, i);
    else
        return index(root->right, i - s);
}

int OrderStatisticTree::rank(Node* root, int key, int sum, int &repeatNum) const {
    if (!root)
        return sum + 1;
    int l = (root->left ? root->left->size : 0) + 1;
    if (root->key == key) {
        auto it = m.find(key);
        repeatNum = it->second; 
        return l + sum;
    }
    if (root->key < key)
        return rank(root->right, key, sum + l, repeatNum);
    else
        return rank(root->left, key, sum, repeatNum);
}

void OrderStatisticTree::leftR(Node* n)
{
    if (!n || !n->right) {
        return;
    }
    Node* r = n->right;
    Node* p = n->p;
    if (p) {
        if (p->right == n)
            p->right = r;
        else
            p->left = r;
    }
    else
        root = r;
    r->p = p;
    n->right = r->left;
    if (n->right)
        n->right->p = n;
    r->left = n;
    n->p = r;
    r->size = n->size;
    n->size = (n->left ? n->left->size : 0) + (n->right ? n->right->size : 0) + 1;
}

void OrderStatisticTree::rightR(Node* n)
{
    if (!n || !n->left) {
        return;
    }
    Node* l = n->left;
    Node* p = n->p;
    if (p) {
        if (p->left == n)
            p->left = l;
        else
            p->right = l;
    }
    else
        root = l;
    l->p = p;
    n->left = l->right;
    if (n->left)
        n->left->p = n;
    l->right = n;
    n->p = l;
    l->size = n->size;
    n->size = (n->left ? n->left->size : 0) + (n->right ? n->right->size : 0) + 1;
}


void OrderStatisticTree::keep(Node* tokeep)
{
    while (tokeep->p && tokeep->p->color == RED) {
        if (tokeep->p->p->left == tokeep->p) {
            Node* father = tokeep->p;
            Node* uncle = father->p->right;
            if (uncle && uncle->color == RED) {
                father->color = BLACK;
                uncle->color = BLACK;
                father->p->color = RED;
                tokeep = father->p;
            }
            else {
                if (tokeep == father->right) {
                    leftR(father);
                    tokeep = father;
                    father = father->p;
                }
                father->color = BLACK;
                father->p->color = RED;
                rightR(father->p);
            }
        }
        else {
            Node* father = tokeep->p;
            Node* uncle = father->p->left;
            if (uncle && uncle->color == RED) {
                uncle->color = BLACK;
                father->color = BLACK;
                father->p->color = RED;
                tokeep = father->p;
            }
            else {
                if (tokeep == father->left) {
                    rightR(father);
                    tokeep = father;
                    father = father->p;
                }
                father->color = BLACK;
                father->p->color = RED;
                leftR(father->p);
            }
        }
    }
    root->color = BLACK;    
}

Node* OrderStatisticTree::getKey(int key) const
{
    Node* r = root;
    while (r) {
        if (r->key == key)
            break;
        if (r->key < key)
            r = r->right;
        else
            r = r->left;
    }
    if (!r)
        return nullptr;
    return r;
}


Node* OrderStatisticTree::getMin(Node* t)
{
    Node* res = nullptr;
    while (t) {
        res = t;
        t = t->left;
    }
    return res;
}

Node* OrderStatisticTree::getNext(Node* t)
{
    if (t &&t->right) {
        return getMin(t->right);
    }
    else {
        while (t && t->p && t->p->left == t) {
            t = t->p;
        }
        if (t && t->p)
            return t->p;
        else
            return nullptr;
    }
}

void OrderStatisticTree::dkeep(Node* x, Node* px)
{
    while (x != root && (!x || x->color == BLACK)) {
        if (x == px->left) {
            Node* w = px->right;
            if (w->color == RED) {
                w->color = BLACK;
                px->color = RED;
                leftR(px);
                w = px->right;
            }
            if ((!w->left || w->left->color == BLACK) && (!w->right || w->right->color == BLACK)) {
                w->color = RED;
                x = px;
                px = px->p;
            }
            else {
                if (!w->right || w->right->color == BLACK) {
                    w->color = RED;
                    w->left->color = BLACK;
                    rightR(w);
                    w = px->right;
                }
                w->color = px->color;
                px->color = BLACK;
                w->right->color = BLACK;
                leftR(px);
                x = root;
            }
        }
        else {
            Node* w = px->left;
            if (w->color == RED) {
                w->color = BLACK;
                px->color = RED;
                rightR(px);
                w = px->left;
            }
            if ((!w->left || w->left->color == BLACK) && (!w->right || w->right->color == BLACK)) {
                w->color = RED;
                x = px;
                px = px->p;
            }
            else {
                if (!w->left || w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftR(w);
                    w = px->left;
                }
                w->color = px->color;
                px->color = BLACK;
                w->left->color = BLACK;
                rightR(px);
                x = root;
            }

        }
    }
    x->color = BLACK;
}

void OrderStatisticTree::clear(Node* root)
{
    if (!root)
        return;
    clear(root->right);
    clear(root->left);
    delete root;
}

int OrderStatisticTree::index(Node* root, int i) const
{
    int l = root->left ? root->left->size : 0;
    int s = l + 1;
    if (s == i)
        return root->key;
    else if (s > i)
        return index(root->left, i);
    else
        return index(root->right, i - s);
}


pair<int,int> OrderStatisticTree::rank(int key)
{
    int repeatNum = 0;
    int sum = 0;
    int upper = rank(root, key, sum, repeatNum);

    int lower = upper - repeatNum + 1;
    return make_pair<int,int>((int)lower, (int)upper);
}

OrderStatisticTree::~OrderStatisticTree()
{
    clear();
}
