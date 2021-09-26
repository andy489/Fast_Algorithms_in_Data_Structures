//  https://codeforces.com/contest/167/problem/D

//  Consider the points as a graph and consider more closely the point with the largest y.
//  1) It is connected with the highest points on the left and right of it.
//  2) It is not connected to all other points.
//  3) It is inside all corners on points from different sides of it.
//  So,
//  1) Building roads on the left and right sides are independent.
//  2) The graph is a tree.
//  3) The tree is a Cartesian tree (Treap)

//  Time complexity:    O(n*log(n)) 
//  Space complexity:   O(n)   

#include <cstdio>
#include <algorithm>

using namespace std;

const int MOD = 1e9 + 9;

typedef struct Node *pnode;

struct Node {
    int key, prior;
    pnode l, r;

    int candidateForRes, res;

    Node(int key, int prior) : key(key), prior(prior), candidateForRes(0), res(0), l(nullptr), r(nullptr) {}
};

struct Treap {
private:
    pnode root;

    void clear(pnode t) {
        if (t) {
            clear(t->l);
            clear(t->r);
            delete t;
        }
    }

    int candidate(pnode t) {
        return t ? t->candidateForRes : -1;
    }

    int res(pnode t) {
        return t ? t->res : 0;
    }

    void upd(pnode &t) {
        if (t) {
            t->candidateForRes = res(t->l) + res(t->r);
            t->res = max(t->candidateForRes,
                         max(candidate(t->l) + res(t->r), res(t->l) + candidate(t->r)) + 1);
        }
    }

    // splits into <= key and >key
    void split(pnode t, pnode &l, pnode &r, int key) {
        if (!t) {
            return void(l = r = nullptr);
        }
        if (t->key <= key) {    // search for right ones (leave lef ones, which are <= key)
            split(t->r, t->r, r, key), l = t;
        } else {
            split(t->l, l, t->l, key), r = t;
        }
        upd(t);
    }

    void merge(pnode &t, pnode l, pnode r) {
        if (!l || !r) {
            return void(t = l ? l : r);
        }
        if (l->prior < r->prior) {
            merge(r->l, l, r->l), t = r;
        } else {
            merge(l->r, l->r, r), t = l;
        }
        upd(t);
    }

    void insert(pnode &t, pnode it) {
        if (!t) {
            t = it;
        } else if (t->prior < it->prior) {
            split(t, it->l, it->r, it->key), t = it;
        } else {
            insert(it->key < t->key ? t->l : t->r, it);
        }
        upd(t);
    }

    void insertAlternative(pnode &t, pnode it) {
        pnode l, r;
        split(t, l, r, it->key);
        merge(t, l, it);
        merge(t, t, r);
    }

    int query(pnode t, int l, int r) {
        pnode le, mi, ri;
        split(t, le, ri, l - 1);
        split(ri, mi, ri, r);
        int toReturn = 0;
        if (mi) {
            toReturn = mi->res;
        }
        merge(t, le, mi);
        merge(t, t, ri);
        return toReturn;
    }

public:
    Treap() : root(nullptr) {}

    ~Treap() {
        clear(this->root);
    }

    void insert(int key, int prior) {
        insert(this->root, new Node(key, prior));
    }

    int query(int l, int r) {
        return query(this->root, l, r);
    }
};

int main() {
    int n, k, x, y, m;
    scanf("%d %d", &n, &k);

    Treap T;

    for (int i = 0; i < k; ++i) {
        scanf("%d %d", &x, &y);
        T.insert(x, y);
    }

    int a, b, c, d;
    scanf("%d %d %d %d", &a, &b, &c, &d);

    for (int i = k; i < n; ++i) {
        x = (a * 1ll * x + b) % MOD;
        y = (c * 1ll * y + d) % MOD;
        T.insert(x, y);
    }

    scanf("%d", &m);

    while (m--) {
        int l, r;
        scanf("%d %d", &l, &r);
        printf("%d\n", T.query(l, r));
    }

    return 0;
}
