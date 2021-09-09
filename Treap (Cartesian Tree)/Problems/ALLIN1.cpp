//  https://www.spoj.com/problems/ALLIN1/

// Time complexity: all queries are O(log(N))
// Space complexity: O(N)

#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

typedef struct Node *pnode;

struct Node {
    int key, prior, cnt, sub;
    pnode l, r;

    Node(int key) : key(key), prior(rand()), cnt(1), sub(1), l(nullptr), r(nullptr) {}
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

    void split(pnode t, int key, pnode &l, pnode &r) {
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

    pnode find(pnode t, int key) {
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

    void updPath(pnode t, int key) {
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

    int rank(pnode t, int key) {
        int rank = 1;
        while (t) {
            if (key < t->key) {
                t = t->l;
            } else if (key > t->key) {
                rank += sub(t->l) + t->cnt;
                t = t->r;
            } else {
                return rank + sub(t->l);
            }
        }
        return -1;
    }

    int kthSmallest(pnode t, int k) {
        if (sub(t) < k) {
            return -1;
        }
        while (t) {
            int lst = sub(t->l);

            if (lst >= k) {
                t = t->l;
            } else if (lst + t->cnt >= k) {
                return t->key;
            } else {
                k -= lst + t->cnt;
                t = t->r;
            }
        }
        return -1;  // we will never reach here
    }

public:
    Treap() : root(nullptr) {
        srand(time(nullptr));
    }

    ~Treap() {
        clear(this->root);
    }

    void smartInsert(int key) { // if it exist - increment and update Path, else insert new element
        pnode found = find(this->root, key);
        if (!found) {
            insert(this->root, new Node(key));
        } else {
            ++found->cnt;
            updPath(this->root, key);
        }
    }

    void smartErase(int key) {
        pnode found = find(this->root, key);
        if (found) {
            int freq = --found->cnt;
            if (!freq) {
                erase(this->root, key);
            }
            updPath(this->root, key);
        }
    }

    int rank(int key) {  // rank = index of key if all keys were sorted
        return rank(this->root, key);
    }

    int kthSmallest(int k) {
        return kthSmallest(this->root, k);
    }
};


int main() {
    Treap t;

    int type, x;
    while (scanf("%d", &type) && type != -1) {
        scanf("%d", &x);
        switch (type) {
            case 1: {
                t.smartInsert(x);
                break;
            }

            case 2: {
                t.smartErase(x);
                break;
            }

            case 3: {
                printf("%d\n", t.rank(x));
                break;
            }

            case 4: {
                printf("%d\n", t.kthSmallest(x));
                break;
            }

            default: {
                printf("ivalid query type\n");
            }
        }
    }
    return 0;
}
