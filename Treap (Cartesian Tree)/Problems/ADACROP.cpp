// https://www.spoj.com/problems/ADACROP/

// Idea: Make a Treap data structure for every vegetable type, using it's unique position as key

// Time complexity: O(N+Q*log^2(N)), where N is the number of all vegetables and Q is the number of queries
// Space complexity: O(N)

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;

typedef struct Node* pnode;

struct Node {
    int key, prior, cnt; // cnt nodes in subtree with the curr root (incl. root)
    pnode l, r;

    Node(int key) : key(key), prior(rand()), cnt(1), l(nullptr), r(nullptr) {}
};

struct Treap {
private:
    pnode root;

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
        } else if (t->key <= key) {
            split(t->r, key, t->r, r), l = t;
        } else {
            split(t->l, key, l, t->l), r = t;
        }
        updCnt(t);
    }

    void insert(pnode &t, pnode it) {
        if (!t) {
            t = it;
        } else if (it->prior <= t->prior) {
            insert(t->key < it->key ? t->r : t->l, it);
        } else {
            split(t, it->key, it->l, it->r), t = it;
        }
        updCnt(t);
    }

    void merge(pnode &t, pnode l, pnode r) {
        if (!l || !r) {
            t = l ? l : r;
        } else if (l->prior <= r->prior) {
            merge(r->l, l, r->l), t = r;
        } else {
            merge(l->r, l->r, r), t = l;
        }
        updCnt(t);
    }

    void erase(pnode &t, int key) {
        if (!t) {
            return;
        }
        if (t->key == key) {
            pnode temp = t;
            merge(t, t->l, t->r);
            delete temp;
        } else {
            erase(key <= t->key ? t->l : t->r, key);
        }
        updCnt(t);
    }

    int queryFirstVersion(pnode t, int key) {
        pnode l(nullptr), r(nullptr);
        split(t, key - 1, l, r);
        int res = (l ? l->cnt : 0);
        merge(t, l, r);
        return res;
    }

    int querySecondVersion(pnode t, int key) {
        if (!t) {
            return 0;
        } else if (t->key < key) {
            return 1 + cnt(t->l) + querySecondVersion(t->r, key);
        } else {
            return querySecondVersion(t->l, key);
        }
    }

    int queryThirdVersion(int key){
        int res = 0;
        pnode t = this->root;
        while (t) {
            if (t->key < key) {
                res += 1 + cnt(t->l);
                t = t->r;
            } else {
                t = t->l;
            }
        }
        return res;
    }

    void clear(pnode t) {
        if (!t)
            return;
        clear(t->l);
        clear(t->r);
        delete t;
    }

public:
    Treap() : root(nullptr) {
        // no time
        // srand(time(nullptr));
    };

    ~Treap() {  // always clean your mess
        clear(this->root);
    }

    void insert(int key) {
        insert(this->root, new Node(key));
    }

    void erase(int key) {
        erase(this->root, key);
    }

    int query(int key) {
        //	return queryFirstVersion(this->root, key);
        //	return querySecondVersion(this->root, key);
        return queryThirdVersion(key);
    }
};

int main() {
    int N, Q, pos, vegType;
    scanf("%d %d", &N, &Q);

    vector<int> a(N);
    map<int, Treap> vegTypeTreap;

    for (int i = 0; i < N; ++i) {
        scanf("%d", &a[i]);
        vegTypeTreap[a[i]].insert(i);
    }

    while (Q--) {
        scanf("%d %d", &pos, &vegType);
        Treap *t = &vegTypeTreap[vegType];
        printf("%d\n", t->query(pos));
        if (a[pos] != vegType) {
            vegTypeTreap[a[pos]].erase(pos);
            t->insert(pos);
            a[pos] = vegType;
        }
    }

    return 0;
}
