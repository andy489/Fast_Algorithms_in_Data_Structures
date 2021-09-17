// https://www.spoj.com/problems/KOILINE/

// Time complexity:  O(log(N))
// Space complexity: O(N)

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

typedef struct Node *pnode;
typedef long long ll;

struct Node {
    ll key, prior, cnt, sub;
    pnode l, r;

    Node(ll key) : key(key), prior(rand()), cnt(1), sub(1), l(nullptr), r(nullptr) {}
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

    int sub(pnode t) {
        return t ? t->sub : 0;
    }

    void upd(pnode t) {
        if (t) {
            t->sub = t->cnt + sub(t->l) + sub(t->r);
        }
    }

    void split(pnode t, ll key, pnode &l, pnode &r) {
        if (!t) {
            l = r = nullptr;
        } else if (key < t->key) {
            split(t->l, key, l, t->l), r = t;
        } else {
            split(t->r, key, t->r, r), l = t;
        }
        upd(t);
    }

    void merge(pnode &t, pnode l, pnode r) {
        if (!l || !r) {
            t = l ? l : r;
        } else if (l->prior > r->prior) {
            merge(l->r, l->r, r), t = l;
        } else {
            merge(r->l, l, r->l), t = r;
        }
        upd(t);
    }

    pnode find(pnode t, ll key) {
        if (!t) {
            return nullptr;
        }
        if (t->key == key) {
            return t;
        }
        if (key < t->key) {
            return find(t->l, key);
        } else {
            return find(t->r, key);
        }
    }

    void updPath(pnode t, ll key) {
        if (!t) {
            return;
        }
        if (t->key == key) {}
        else if (key < t->key) {
            updPath(t->l, key);
        } else {
            updPath(t->r, key);
        }
        upd(t);
    }

    void insert(pnode &t, pnode it) {
        if (!t) {
            t = it;
        } else if (t->prior > it->prior) {
            insert(t->key <= it->key ? t->r : t->l, it);
        } else {
            split(t, it->key, it->l, it->r), t = it;
        }
        upd(t);
    }

    void erase(pnode &t, ll key) {
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

    int kthSmallest(pnode t, ll k) {
        while (t) {
            ll lst = sub(t->l);

            if (lst >= k) {
                t = t->l;
            } else if (lst + t->cnt >= k) {
                return t->key;
            } else {
                k -= lst + t->cnt;
                t = t->r;
            }
        }
    }

public:
    Treap() : root(nullptr) {
        srand(time(nullptr));
    }

    ~Treap() {
        clear(this->root);
    }

    void smartInsert(ll key) {
        pnode found = find(this->root, key);
        if (!found) {
            insert(this->root, new Node(key));
        } else {
            ++found->cnt;
            updPath(this->root, key);
        }
    }

    void smartErase(ll key) {
        pnode found = find(this->root, key);
        if (found) {
            ll freq = --found->cnt;
            if (!freq) {
                erase(this->root, key);
            }
            updPath(this->root, key);
        }
    }

    int kthSmallest(ll k) {
        if (sub(this->root) < k) {
            return -1;
        }
        return kthSmallest(this->root, k);
    }
};

int main() {
    ll N, h;
    scanf("%lld", &N);

    Treap T;
    vector<ll> prevHigher(N), res(N);

    for (int i = 0; i < N; ++i) {
        scanf("%lld", &h);
        T.smartInsert(h);
    }

    for (int i = 0; i < N; ++i) {
        scanf("%lld", &prevHigher[i]);
    }

    for (int i = N - 1; i >= 0; --i) {
        res[i] = T.kthSmallest(prevHigher[i] + 1);
        T.smartErase(res[i]);
    }

    for (int j = 0; j < N; ++j) {
        printf("%lld\n", res[j]);
    }
    return 0;
}
