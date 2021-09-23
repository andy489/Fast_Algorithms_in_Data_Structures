//  https://codeforces.com/contest/295/problem/E

//  Time complexity:  O(max(n*log(n), m*log(n))
//  Space complexity: O(n)

#include <cstdio>   //  scanf(...), printf(...)
#include <cstdlib>  //  srand(...), rand(...)
#include <ctime>    //  time(...)
#include <vector>
#include <cstdint>  //  int64_t

using namespace std;

typedef struct Node *pnode;

struct Node {
    int key, prior, cnt;
    int64_t sum, res;
    pnode l, r;

    Node(int64_t key) : key(key), prior(rand()), cnt(1), sum(key), res(0), l(nullptr), r(nullptr) {}
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

    int cnt(pnode t) {
        return t ? t->cnt : 0;
    }

    int64_t sum(pnode t) {
        return t ? t->sum : 0;
    }

    int64_t res(pnode t) {
        return t ? t->res : 0;
    }

    void upd(pnode t) {
        if (t) {
            t->cnt = 1 + cnt(t->l) + cnt(t->r);
            t->sum = sum(t->l) + sum(t->r) + t->key;
            t->res = res(t->l) + res(t->r) + cnt(t->l) * 1LL * sum(t->r) - cnt(t->r) * 1LL * sum(t->l) +
                     t->key * 1LL * (cnt(t->l) - cnt(t->r)) + sum(t->r) - sum(t->l);
        }
    }

    void merge(pnode &t, pnode l, pnode r) {
        if (!l || !r) {
            t = l ? l : r;
        } else if (l->prior < r->prior) {
            merge(r->l, l, r->l), t = r;
        } else {
            merge(l->r, l->r, r), t = l;
        }
        upd(t);
    }

    // splits into <= key and > key
    void split(pnode t, pnode &l, pnode &r, int key) {
        if (!t) {
            l = r = nullptr;
        } else if (t->key <= key) {
            split(t->r, t->r, r, key), l = t;
        } else {
            split(t->l, l, t->l, key), r = t;
        }
        upd(t);
    }

    void insert(pnode &t, int key) {
        pnode l, r;
        split(t, l, r, key);
        merge(t, l, new Node(key));
        merge(t, t, r);
    }

    void erase(pnode &t, int key) {
        pnode l, toErase, r;
        split(t, l, r, key);
        split(l, l, toErase, key - 1);
        delete toErase;
        merge(t, l, r);
    }

    int64_t sumDistances(pnode t, int l, int r) {
        pnode left, center, right;

        split(t, left, right, l - 1);
        split(right, center, right, r);

        int64_t toReturn = res(center);
        merge(t, left, center);
        merge(t, t, right);
        return toReturn;
    }

public:

    Treap() : root(nullptr) {
        srand(time(nullptr));
    }

    ~Treap() {
        clear(this->root);
    }

    void insert(int key) {
        insert(this->root, key);
    }

    void erase(int key) {
        erase(this->root, key);
    }

    int64_t sumDistances(int l, int r) {
        return sumDistances(this->root, l, r);
    }
};


int main() {
    int n, m, type;
    scanf("%d", &n);

    vector<int> coord(n);

    Treap T;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &coord[i]);
        T.insert(coord[i]);
    }

    scanf("%d", &m);

    while (m--) {
        scanf("%d", &type);
        switch (type) {
            case 1 :
                int p, d;
                scanf("%d %d", &p, &d);
                --p;
                T.erase(coord[p]);
                coord[p] += d;
                T.insert(coord[p]);
                break;
            case 2:
                int l, r;
                scanf("%d %d", &l, &r);
                printf("%lld\n", T.sumDistances(l, r));
        }
    }
    return 0;
}
