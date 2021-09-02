// https://www.spoj.com/problems/ADAAPHID/

// Time complexity: O(N*log(N)), where N is the number of aphids
// Space complexity: O(N)

#include <cstdio>   // printf()
#include <cstdlib>  // rand()

using namespace std;

typedef long long ll;

typedef struct Node* pnode;

struct Node {
    ll key, val, sum; // key is A, val is pseudoRand V, sum is second int of ID
    int prior;
    pnode l, r;

    Node(ll &key, int prior, ll val) : key(key), prior(prior), val(val), sum(val), l(nullptr), r(nullptr) {}
    // sum is initially set to val (i.e. we have only one node at initialization)
};

struct Treap {
private:
    pnode root;

    ll sum(pnode t) {
        return t ? t->sum : 0;
    }

    void push(pnode t) {
        if (t)
            t->sum = t->val + sum(t->l) + sum(t->r);
    }

    void split(pnode t, ll key, pnode &l, pnode &r) {
        if (!t)
            l = r = nullptr;
        else if (t->key <= key)
            split(t->r, key, t->r, r), l = t;
        else
            split(t->l, key, l, t->l), r = t;
        push(t);
    }

    void insert(pnode &t, pnode it) {
        if (!t)
            t = it;
        else if (it->prior > t->prior)
            split(t, it->key, it->l, it->r), t = it;
        else
            insert(t->key < it->key ? t->r : t->l, it);
        push(t);
    }

    bool find(pnode t, ll key, ll val) {
        if (!t) // if we reach nullptr, that means we haven't find node n with n->key == key
            return false;
        if (t->key == key) {    // there is already such node n with n->key == key
            t->val += val;      // actualize val of that node
            t->sum += val;       // or equivalently we can call push(t)
            return true;        // no need to add new node
        }
        bool f = find(t->key < key ? t->r : t->l, key, val); // traverse to left or right sub-treaps
        push(t);
        return f;
    }

    ll query(pnode t, ll key) {
        if (!t)
            return 0;
        if (t->key > key)
            return query(t->l, key);
        return t->val + sum(t->l) + query(t->r, key);
    }

    void clear(pnode t) {
        if (!t)
            return;
        clear(t->l);
        clear(t->r);
        delete t;
    }

public:
    Treap() : root(nullptr) {}

    ~Treap() {  // always clean your mess
        clear(this->root);
    }

    void insert(ll key, ll val) {
        if (!find(this->root, key, val)) { // first check if there is node n with n->key == key
            // if there isn't, then insert it
            pnode n = new Node(key, rand(), val);
            insert(this->root, n);
        } // else actualize it's val (pseudoRand), which we already did in find function
    }

    ll query(ll x) {
        return query(this->root, x);
    }
};

int main() {
    Treap T;
    int Q;
    scanf("%d", &Q);
    ll L(0);
    while (Q--) {
        ll A, V;
        scanf("%lld %lld", &A, &V);
        A ^= L;
        V ^= L;
        T.insert(A, V);
        L = T.query(A);
        printf("%lld %lld\n", A, L);
    }
}
