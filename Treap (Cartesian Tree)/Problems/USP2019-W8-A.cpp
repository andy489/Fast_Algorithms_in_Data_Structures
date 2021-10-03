//  https://www.hackerrank.com/contests/usp2019-week-5/challenges/week-8-task-a

//  Time complexity:  O(max(n,m)*log(n))
//  Space:            O(n)

#include <cstdio>
#include <algorithm>
#include <random>

using namespace std;

typedef long long ll;

typedef struct Node *pnode;

struct Node {
    int prior, val, cnt;
    ll sum;
    bool rev;
    pnode l, r;

    // prior = random variable for balancing the tree
    // cnt = count of nodes in subtree with root this node (inclusive)
    // val = value of this node
    // sum = sum of values in nodes of subtree with this node (inclusive)
    // rev = bool flag for indicating a reversal took place
    // l and r = left and right children of this node

    Node(int val, int prior) : prior(prior), val(val), cnt(1), sum(val), rev(false), l(nullptr), r(nullptr) {}
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

    ll sum(pnode t) {
        return t ? t->sum : 0;
    }

    void upd(pnode t) {
        if (t) {
            t->cnt = cnt(t->l) + 1 + cnt(t->r);
            t->sum = sum(t->l) + t->val + sum(t->r);
        }
    }

    void pushLazy(pnode t) {    // propagate
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

    // splits by count into (= key) and (= total - key), in this case key = position
    void split(pnode t, pnode &l, pnode &r, int key, int add = 0) {
        if (!t) {
            return void(l = r = nullptr);
        }
        pushLazy(t);
        int curKey = cnt(t->l) + add;   // implicit key
        if (key <= curKey) {
            split(t->l, l, t->l, key, add), r = t;
        } else {
            split(t->r, t->r, r, key, 1 + curKey), l = t;
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

    void insert(pnode &t, pnode it, int pos) {
        pnode l, r;
        split(t, l, r, pos - 1);
        merge(l, l, it);
        merge(t, l, r);
    }

    ll sum(pnode t, int left, int right) {
        pnode l, m, r;
        split(t, l, r, left - 1);
        split(r, m, r, right - left + 1);

        ll s = sum(m);

        merge(t, l, m);
        merge(t, t, r);
        return s;
    }

    void flip(pnode &t, int left, int right) {
        pnode l, m, r;
        split(t, l, r, left - 1);
        split(r, m, r, right - left + 1);

        m->rev ^= 1;

        merge(t, l, m);
        merge(t, t, r);
    }

public:
    Treap() : root(nullptr) {}

    ~Treap() {
        clear(this->root);
    }

    void insert(int val, int prior, int pos) {
        insert(this->root, new Node(val, prior), pos);
    }

    ll sum(int l, int r) {
        return sum(this->root, l, r);
    }

    void rev(int l, int r) {
        flip(this->root, l, r);
    }
};

int main() {
    Treap T;

    int n, m, x, q, l, r;
    scanf("%d %d", &n, &m);

    int *rand = new int[n];
    for (int i = 0; i < n; ++i) {
        rand[i] = i;
    }

    random_device rd;
    mt19937 g(rd());
    shuffle(rand, rand + n, g);

    for (int i = 1; i <= n; ++i) {
        scanf("%d", &x);
        T.insert(x, rand[i], i + 1);
    }

    while (m--) {
        scanf("%d %d %d", &q, &l, &r);
        switch (q) {
            case 0:
                printf("%lld\n", T.sum(l, r));
                break;
            case 1:
                T.rev(l, r);
                break;
            default:
                printf("Invalid type of query\n");
        }
    }
    return 0;
}
