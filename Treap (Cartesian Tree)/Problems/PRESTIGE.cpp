//  https://www.codechef.com/problems/PRESTIGE

//  Time complexity:  O(N*log(N))
//  Space complexity: O(N)

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

typedef struct Node *pnode;
typedef long long ll;

struct Node {
    int prior, val, cnt;
    ll sum;
    bool rev;
    pnode l, r;

    Node(int val) : val(val), sum(val), prior(rand()), cnt(1), rev(false), l(nullptr), r(nullptr) {}
};

struct Treap {
private:
    pnode upper, lower;

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

    ll sum(pnode t) {
        return t ? t->sum : 0;
    }

    void upd(pnode t) {
        if (t) {
            t->cnt = 1 + cnt(t->l) + cnt(t->r);
            t->sum = t->val + sum(t->l) + sum(t->r);
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

    void split(pnode t, pnode &l, pnode &r, int key, int add = 0) { // key = pos
        if (!t) {
            return void(l = r = nullptr);
        }
        pushLazy(t);
        int curKey = add + cnt(t->l);
        if (key < curKey) {
            split(t->l, l, t->l, key, add), r = t;
        } else {
            split(t->r, t->r, r, key, curKey + 1), l = t;
        }
        upd(t);
    }

    void merge(pnode &t, pnode l, pnode r) {
        if (!l || !r) {
            return void(t = l ? l : r);
        }
        pushLazy(l);
        pushLazy(r);
        if (l->prior < r->prior) {
            merge(r->l, l, r->l), t = r;
        } else {
            merge(l->r, l->r, r), t = l;
        }
        upd(t);
    }

    void flipSecDeck(pnode &t1, pnode &t2, int l, int r) {
        pnode l1, m1, r1;
        split(t1, l1, r1, l - 1);
        split(r1, m1, r1, r - l);

        m1->rev ^= 1;

        pnode l2, m2, r2;
        split(t2, l2, r2, l - 1);
        split(r2, m2, r2, r - l);

        m2->rev ^= 1;

        merge(t1, l1, m2);
        merge(t1, t1, r1);

        merge(t2, l2, m1);
        merge(t2, t2, r2);
    }

    ll sumQuery(pnode t, int left, int right) {
        pnode l, m, r;
        split(t, l, r, left - 1);
        split(r, m, r, right - left);

        ll s = sum(m);

        merge(t, l, m);
        merge(t, t, r);
        return s;
    }

public:
    Treap() : upper(nullptr), lower(nullptr) {
        srand(time(nullptr));
    }

    ~Treap() {
        clear(this->upper);
        clear(this->lower);
    }

    void insert(int val, bool upper = 0) {
        if (upper) {
            merge(this->upper, this->upper, new Node(val));
        } else {
            merge(this->lower, this->lower, new Node(val));
        }
    }

    void flipSecDeck(int l, int r) {
        flipSecDeck(this->upper, this->lower, l, r);
    }

    ll sumQuery(int left, int right) {
        if (right < left) {
            return 0LL;
        }
        return sumQuery(this->upper, left, right);
    }

};


signed main() {
    int N, M, x, type;
    scanf("%d%d", &N, &M);

    Treap T;

    for (int i = 0; i < N; ++i) {
        scanf("%d", &x);
        T.insert(x, 1);
    }
    for (int i = 0; i < N; ++i) {
        scanf("%d", &x);
        T.insert(x);
    }

    int negLim = 0; // negative (-1) limit
    while (M--) {
        scanf("%d", &type);
        switch (type) {
            case 1: {
                int l, r;
                scanf("%d %d", &l, &r);
                --l, --r;
                T.flipSecDeck(l, r);
                break;
            }
            case 2: {
                int k;
                scanf("%d", &k);
                negLim = k - negLim;
                break;
            }
            case 3: {
                int A, B, C, D;
                scanf("%d %d %d %d", &A, &B, &C, &D);
                --A, --B;
                // first deck of cards
                // [C, D]
                // [1, negLim][negLim + 1, N] -> [-1 ... -1][1 ... 1]
                int negPart = 0;
                if (C <= negLim) {
                    negPart = negLim - C + 1;
                }
                negPart = min(B - A + 1, negPart);
                ll sumNeg = -1LL * T.sumQuery(A, A + negPart - 1);
                ll sumPos = T.sumQuery(A + negPart, B);
                printf("%lld\n", sumNeg + sumPos);
                break;
            }
            default: {
                printf("Invalid type of query\n");
                break;
            }
        }
    }
    return 0;
}
