/*  

https://www.hackerrank.com/challenges/find-the-running-median/problem

Time complexity:    O(n*log(n))
Space complexity:   O(n)

*/

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

    // splits by key into <= key and > key
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
    
	// splits by count into =cnt and =total-cnt
    void splitCnt(pnode t, pnode &l, pnode &r, int key, int add = 0) {
        if (!t) {
            return void(l = r = nullptr);
        }
        int curKey = add + cnt(t->l);   // implicit key
        if (key <= curKey) {
            splitCnt(t->l, l, t->l, key, add), r = t;
        } else {
            splitCnt(t->r, t->r, r, key, 1 + curKey), l = t;
        }
        updCnt(t);
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

    int getOddMedian(int cnt) {
        pnode le, mid, ri;
        splitCnt(this->root, le, ri, cnt);
        splitCnt(ri, mid, ri, 1);
        int median = mid->key;
        merge(this->root, le, mid);
        merge(this->root, this->root, ri);
        return median;
    }

    int getEvenMedian(int cnt) {
        pnode le, mid1, mid2, ri;
        splitCnt(this->root, le, ri, cnt - 1);
        splitCnt(ri, mid1, ri, 1);
        splitCnt(ri, mid2, ri, 1);
        int median = mid1->key + mid2->key;
        merge(this->root, le, mid1);
        merge(this->root, this->root, mid2);
        merge(this->root, this->root, ri);
        return median;
    }
};

int main() {
    int n, x;
    scanf("%d", &n);

    Treap T;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &x);
        T.insert(x);
        int SIZE = T.size();
        if (SIZE % 2 == 1) {
            printf("%d.0\n", T.getOddMedian(SIZE / 2));
        } else {
            printf("%.1f\n", T.getEvenMedian(SIZE / 2) / 2.0);
        }
    }
    return 0;
}
