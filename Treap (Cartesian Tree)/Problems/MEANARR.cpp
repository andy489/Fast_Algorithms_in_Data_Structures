// https://www.spoj.com/problems/MEANARR/

// Idea: an subinterval [l,r] has mean >= k if and only if Sr-Sl>=0,
// where Sr and Sl are the partial sums ending at l and r respectively after we substract k from all of the elements

// Time complexity: O(n*log(n))
// SPace complexity: O(n)

#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

typedef struct Node *pnode;
typedef long long ll;

struct Node {
    ll key, prior, freq, sub;
    pnode l, r;

    Node(ll key) : key(key), prior(rand()), freq(1), sub(1), l(nullptr), r(nullptr) {}
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
            t->sub = t->freq + sub(t->l) + sub(t->r);
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
        } else if (l->prior < r->prior) {
            merge(r->l, l, r->l), t = r;
        } else {
            merge(l->r, l->r, r), t = l;
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

    void updPath(pnode t, ll key) {
        if (t->key == key) {}
        else if (key < t->key) {
            updPath(t->l, key);
        } else {
            updPath(t->r, key);
        }
        upd(t);
    }

    ll leq(pnode t, ll key) {
        ll res = 0;
        while (t) {
            if (t->key > key) {
                t = t->l;
            } else {
                res += sub(t->l) + t->freq;
                t = t->r;
            }
        }
        return res;
    }

public:
    Treap() : root(nullptr) {
        srand(time(nullptr));
    }

    ~Treap() {
        clear(this->root);
    }

    void smartInsert(ll key) { // if it exist - increment and update Path, else insert new element
        pnode found = find(this->root, key);
        if (!found) {
            insert(this->root, new Node(key));
        } else {
            ++found->freq;
            updPath(this->root, key);
        }
    }

    ll leq(ll key) {
        return leq(this->root, key);
    }
};

int main() {
    int n, k, x;
    ll pref = 0,res = 0;
    scanf("%d %d", &n, &k);
    Treap T;
    T.smartInsert(0LL);
    while (n--) {
        scanf("%d", &x);
        x -= k;
        pref += x;
        res += T.leq(pref);
        T.smartInsert(pref);
    }
    printf("%lld\n", res);
    return 0;
}
