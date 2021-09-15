// http://codeforces.com/contest/863/problem/D

// Time complexity: O(n*log(n) + Q*log(n))
// Space complexity: O(n)

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <utility>

using namespace std;

typedef struct Node *pnode;

struct Node {
    int prior, cnt, val;
    bool rev;
    pnode l, r;

    Node(int val) : prior(rand()), cnt(1), val(val), rev(false), l(nullptr), r(nullptr) {}
};

struct Treap {

private :
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

    void updCnt(pnode t) {
        if (t) {
            t->cnt = cnt(t->l) + cnt(t->r) + 1;
        }
    }

    void pushLazy(pnode t) {
        if (t && t->rev) {
            t->rev = false;
            swap(t->l, t->r);
            if (t->l) {
                t->l->rev ^= 1;
            }
            if (t->r) {
                t->r->rev ^= 1;
            }
        }
    }

    void split(pnode t, pnode &l, pnode &r, int key, int add = 0) {
        if (!t) {
            return void(l = r = nullptr);
        }
        pushLazy(t);
        int curKey = add + cnt(t->l);
        if (key <= curKey) {
            split(t->l, l, t->l, key, add), r = t;
        } else {
            split(t->r, t->r, r, key, add + 1 + cnt(t->l)), l = t;
        }
        updCnt(t);
    }

    void merge(pnode &t, pnode l, pnode r) {
        pushLazy(l);
        pushLazy(r);
        if (!l || !r) {
            return void(t = l ? l : r);
        }
        if (l->prior > r->prior) {
            merge(l->r, l->r, r), t = l;
        } else {
            merge(r->l, l, r->l), t = r;
        }
        updCnt(t);
    }

    void insert(pnode &t, int val) {
        merge(t, t, new Node(val));
    }

    void cyclicShiftRight(pnode &t, int l, int r) {
        pnode t1, t2, t3, t4;
        split(t, t3, t4, r);
        split(t3, t2, t3, r - 1);
        split(t2, t1, t2, l - 1);
        merge(t1, t1, t3);
        merge(t2, t1, t2);
        merge(t, t2, t4);
    }

    void reverse(pnode &t, int l, int r) {
        pnode t1, t2, t3;
        split(t, t1, t2, l);
        split(t2, t2, t3, r - l + 1);
        t2->rev ^= true;
        merge(t, t1, t2);
        merge(t, t, t3);
    }

    int kth(pnode t, int k) {
        pnode t1, t2, t3;
        split(t, t2, t3, k);
        split(t2, t1, t2, k - 1);
        int res = t2->val;
        merge(t2, t1, t2);
        merge(t, t2, t3);
        return res;
    }

public:
    Treap() : root(nullptr) {
        srand(time(nullptr));
    }

    ~Treap() {
        clear(this->root);
    }

    void insert(int val) {
        insert(this->root, val);
    }

    void cyclicShiftRight(int l, int r) {
        cyclicShiftRight(this->root, l, r);
    }

    void reverse(int l, int r) {
        return reverse(this->root, l - 1, r - 1);
    }

    int kth(int k) {
        return kth(this->root, k);
    }

};

int main() {
    int n, q, m, i, x, type, l, r, k;
    scanf("%d %d %d", &n, &q, &m);
    Treap T;

    for (i = 0; i < n; ++i) {
        scanf("%d", &x);
        T.insert(x);
    }

    while (q--) {
        scanf("%d %d %d", &type, &l, &r);
        if (l == r) {
            continue;
        }
        if (type == 1) {
            T.cyclicShiftRight(l, r);
        } else {
            T.reverse(l, r);
        }
    }

    while (m--) {
        scanf("%d", &k);
        printf("%d ", T.kth(k));
    }
    return printf("\n"), 0;
}
