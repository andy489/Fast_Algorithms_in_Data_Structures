//  https://www.spoj.com/problems/IITWPC4D/

//  Time complexity: O(T * N * log(N))
//  Space complexity: O(N);

#include <cstdio>
#include <vector>
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

    void split(pnode t, int key, pnode &l, pnode &r) {
        if (!t) {
            l = r = nullptr;
        } else if (t->key < key) {
            split(t->r, key, t->r, r), l = t;
        } else {
            split(t->l, key, l, t->l), r = t;
        }
        updCnt(t);
    }

    void merge(pnode &t, pnode l, pnode r) {
        if (!l || !r) {
            t = l ? l : r;
        } else if (l->prior < r->prior) {
            merge(r->l, l, r->l), t = r;
        } else {
            merge(l->r, l->r, r), t = l;
        }
        updCnt(t);
    }

    void insert(pnode &t, pnode it) {
        if (!t) {
            t = it;
        } else if (t->prior < it->prior) {
            split(t, it->key, it->l, it->r), t = it;
        } else {
            insert(t->key < it->key ? t->r : t->l, it);
        }
        updCnt(t);
    }

    void erase(pnode &t, int key) {
        if (!t) {
            return;
        }
        if (t->key == key) {
            pnode toDel = t;
            merge(t, t->l, t->r);
            delete toDel;
        } else {
            erase(t->key < key ? t->r : t->l, key);
        }
        updCnt(t);
    }

    int kthLargest(pnode t, int k) {
        if (cnt(t) < k) {
            return -1;
        }
        while (t) {
            if (cnt(t->r) == k - 1) {
                return t->key;
            } else if (cnt(t->r) >= k) {
                t = t->r;
            } else {
                k -= 1 + cnt(t->r);
                t = t->l;
            }
        }
        return -1;
    }

public:
    Treap() : root(nullptr) {
        srand(time(nullptr));
    }

    ~Treap() {
        clear(this->root);
    }

    void insert(int key) {
        insert(this->root, new Node(key));
    }

    void erase(int key) {
        erase(this->root, key);
    }

    int kthLargest(int k) {
        return kthLargest(this->root, k);
    }

};

void solve(int &T) {
    printf("Test : %d\n", T);
    int n;
    scanf("%d", &n);
    vector<int> v(n), a(n); // a = answer (we fill it from right to left)
    for (int i = 0; i < n; ++i) {
        scanf("%d", &v[i]);
    }

    Treap t;
    for (int j = 1; j <= n; ++j) {
        t.insert(j);
    }

    for (int i = n - 1; i >= 0; --i) {
        a[i] = t.kthLargest(v[i] + 1);
        if (a[i] == -1) {
            printf("-1\n");
            return;
        }
        t.erase(a[i]);
    }
    for (int x:a) {
        printf("%d ", x);
    }
    printf("\n");
}

int main() {
    int T;
    scanf("%d", &T);

    for (int i = 1; i <= T; ++i) {
        solve(i);
    }

    return 0;
}
