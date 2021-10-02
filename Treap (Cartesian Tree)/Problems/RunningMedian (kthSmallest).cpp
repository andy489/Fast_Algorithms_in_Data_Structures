//  https://www.hackerrank.com/challenges/find-the-running-median/problem

//  Time complexity:    O(n*log(n))
//  Space complexity:   O(n)

#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

typedef struct Node *pnode;

struct Node {
    int key, prior, cnt;
    pnode l, r;

    Node(int key) : key(key), prior(rand()), cnt(1), l(nullptr), r(nullptr) {}
};

struct Treap {
private:
    pnode root;
    int sz;

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

    // splits into <= key and > key
    void split(pnode t, pnode &l, pnode &r, int key) {
        if (!t) {
            return void(l = r = nullptr);
        }
        if (t->key <= key) {
            split(t->r, t->r, r, key), l = t;
        } else {
            split(t->l, l, t->l, key), r = t;
        }
        updCnt(t);
    }

    void merge(pnode &t, pnode l, pnode r) {
        if (!l || !r) {
            t = l ? l : r;
        } else if (l->prior <= r->prior) {
            merge(l->r, l->r, r), t = l;
        } else {
            merge(r->l, l, r->l), t = r;
        }
        updCnt(t);
    }

    void insert(pnode &t, int key) {
        pnode l, r;
        split(t, l, r, key);
        merge(t, l, new Node(key));
        merge(t, t, r);
    }

    int kthSmallest(pnode t, int k) {
        if (cnt(t) < k) {
            return -1;
        }
        while (t) {
            int lst = cnt(t->l);
            if (lst >= k) {
                t = t->l;
            } else if (lst + 1 >= k) {
                return t->key;
            } else {
                k -= lst + 1;
                t = t->r;
            }
        }
        return -1;
    }

public:
    Treap() : root(nullptr), sz(0) {
        srand(time(nullptr));
    }

    ~Treap() {
        clear(this->root);
    }

    int size() {
        return this->sz;
    }

    void insert(int key) {
        ++this->sz;
        insert(this->root, key);
    }

    void median(int n) {
        if (n % 2 == 1) {
            printf("%d.0\n", kthSmallest(this->root, n / 2 + 1));
        } else {
            int mi1 = kthSmallest(this->root, n / 2);
            int mi2 = kthSmallest(this->root, n / 2 + 1);
            printf("%.1f\n", (mi1 + mi2) / 2.0);
        }
    }
};

int main() {
    int n, x;
    scanf("%d", &n);

    Treap T;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &x);
        T.insert(x);
        T.median(T.size());
    }
    return 0;
}
