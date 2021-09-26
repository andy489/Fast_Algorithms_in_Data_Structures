//  https://codeforces.com/contest/899/problem/F

//  Time complexity: O(min(n*log(n), m*log(n))  -   we will delete each position no more than once.
//  Space complexity:   O(n)

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

char *s;

const int BUCKET = 26 + 26 + 10;

typedef struct Node *pnode;

int code(char ch) {
    if ('0' <= ch && ch <= '9') {
        return ch - '0';
    }
    if ('A' <= ch && ch <= 'Z') {
        return ch - 'A' + 10;
    }
    if ('a' <= ch && ch <= 'z') {
        return ch - 'a' + 10 + 26;
    }
}

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

    void upd(pnode &t) {
        if (t) {
            t->cnt = 1 + cnt(t->l) + cnt(t->r);
        }
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
        upd(t);
    }

    void insert(pnode &t, int key) {
        pnode l, r;
        split(t, l, r, key);
        merge(t, l, new Node(key));
        merge(t, t, r);
    }

    int kthRecursive(pnode t, int k) {
        if (t->cnt < k) {
            return -1;
        }
        int lst = cnt(t->l);    // left subtree
        if (k == lst) {
            return t->key;
        } else if (k < lst) {
            return kthRecursive(t->l, k);
        } else {
            return kthRecursive(t->r, k - lst - 1);
        }
    }

    int kthIterative(pnode t, int k) {
        if (t->cnt < k) {
            return -1;
        }
        while (t) {
            int lst = cnt(t->l);
            if (lst == k) {
                return t->key;
            } else if (k < lst) {
                t = t->l;
            } else {
                k -= lst + 1;
                t = t->r;
            }
        }
        return -1;
    }

    void erase(pnode &t, int key) {
        if (!t) {
            return;
        }
        if (t->key == key) {
            pnode toDel = t;
            merge(t, t->l, t->r);
            delete toDel;
            upd(t);
        } else {
            erase(t->key < key ? t->r : t->l, key);
            upd(t);
        }
    }

    void LDR(pnode t) {
        if (t) {
            LDR(t->l);
            printf("%c", s[t->key]);
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

    void insert(int key) {
        insert(this->root, key);
    }

    int kth(int k) {
        //  return kthRecursive(this->root, k);
        return kthIterative(this->root, k);
    }

    void erase(int key) {
        erase(this->root, key);
    }

    void LDR() {
        LDR(this->root);
    }

};

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    vector<set<int>> a(BUCKET);

    s = new char[n + 1];
    scanf("%s", s);
    s[n] = '\0';

    for (int i = 0; i < n; ++i) {
        a[code(s[i])].insert(i);
    }

    Treap T;

    for (int i = 0; i < n; ++i) {
        T.insert(i);
    }

    while (m--) {
        int l, r;
        char ch;
        scanf("%d %d %c", &l, &r, &ch);
        --l;
        --r;

        int i = code(ch);

        l = T.kth(l);
        r = T.kth(r);

        auto lb = lower_bound(a[i].begin(), a[i].end(), l);
        auto ub = upper_bound(a[i].begin(), a[i].end(), r);
        auto start = lb;
        while (lb != ub) {
            T.erase(*lb);
            ++lb;
        }
        a[i].erase(start, lb);
    }

    return T.LDR(), printf("\n"), 0;
}
