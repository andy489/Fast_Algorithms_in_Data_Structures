// https://www.spoj.com/problems/TWIST/

// Time complexity: O(min(N*log(N),M*log(N)), can be done in O(min(N,M*log(N))
// Space complexity: O(N)

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <utility>

using namespace std;

typedef struct Node *pnode;

struct Node {
    int key, prior, cnt;
    bool rev;
    pnode l, r;

    Node(int key) : key(key), prior(rand()), cnt(1), rev(false), l(nullptr), r(nullptr) {}
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

    void updCnt(pnode t) {
        if (t) {
            t->cnt = 1 + cnt(t->l) + cnt(t->r);
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
        int curKey = cnt(t->l) + add;
        if (key <= curKey) {
            split(t->l, l, t->l, key, add), r = t;
        } else {
            split(t->r, t->r, r, key, curKey + 1), l = t;
        }
        updCnt(t);
    }

    void merge(pnode &t, pnode l, pnode r) {
        pushLazy(l);
        pushLazy(r);
        if (!l || !r) {
            t = l ? l : r;
        } else if (l->prior < r->prior) {
            merge(r->l, l, r->l), t = r;
        } else {
            merge(l->r, l->r, r), t = l;
        }
        updCnt(t);
    }

    void insert(pnode &t, pnode it, int pos) {
        pnode l, r;
        split(t, l, r, pos - 1);
        merge(l, l, it);
        merge(t, l, r);
    }

    void reverse(pnode &t, int l, int r) {
        pnode t1, t2, t3;
        split(t, t1, t2, l - 1);
        split(t2, t2, t3, r - l + 1);
        t2->rev ^= 1;
        merge(t, t1, t2);
        merge(t, t, t3);
    }

    void LDR(pnode &t) {
        if (t) {
            pushLazy(t);
            LDR(t->l);
            printf("%d ", t->key);
            LDR(t->r);
        }
    }

public:
    Treap() : root(nullptr) {
        srand(time(nullptr));
    }

    ~Treap() {
        clear(this->root);
    }

    void insert(int key, int pos) {
        insert(this->root, new Node(key), pos);
    }

    void reverse(int l, int r) {
        reverse(this->root, l, r);
    }

    void LDR() {
        LDR(this->root);
    }
};

int main() {
    int N, M, l, r;
    scanf("%d %d", &N, &M);
    Treap T;
    for (int i = 1; i <= N; ++i) {
        T.insert(i, i);
    }

    while (M--) {
        scanf("%d %d", &l, &r);
        T.reverse(l, r);
    }
    return T.LDR(), 0;
}
