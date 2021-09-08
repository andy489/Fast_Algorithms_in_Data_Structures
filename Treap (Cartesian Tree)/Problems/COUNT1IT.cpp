// https://www.spoj.com/problems/COUNT1IT/

//  All operations cost O(log(N)) time complexity, where N is the current count of different elements in Treap
//  Space compleity: O(N);

#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

typedef struct Node *pnode;

struct Node {
    int key, prior, cnt, sub;   // cnt = count of curr element, sub = elements of subtree with curr root
    pnode l, r;

    Node(int key) : key(key), prior(rand()), cnt(1), sub(1), l(nullptr), r(nullptr) {}
};

struct Treap {
private:
    pnode root;
    
    void clear(pnode t){
        if(t){
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

    int leq(pnode t, int key) { // less than or equal to
        int res = 0;
        while (t) {
            if (t->key > key) {
                t = t->l;
            } else {
                res += sub(t->l) + t->cnt;
                t = t->r;
            }
        }
        return res;
    }

    int kthSmallest(pnode t, int k) {
        if (!t) {
            return -1;
        } else {
            int lst = sub(t->l);
            if (lst >= k) {
                return kthSmallest(t->l, k);
            } else if (lst + t->cnt >= k) {
                return t->key;
            } else {
                return kthSmallest(t->r, k - lst - t->cnt);
            }
        }
    }

public:
    Treap() : root(nullptr) {
        srand(time(nullptr));
    }

    ~Treap(){
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

    void firstQuery(int key) {
        int a = leq(this->root, key);
        smartInsert(key + a);
    }

    int secondQuery(int key) {
        return leq(this->root, key);
    }

    int kthSmallest(int k) {
        return kthSmallest(this->root, k);
    }
};

int main() {
    int n, q, type, x;
    scanf("%d %d", &n, &q);
    Treap t;
    while (n--) {
        scanf("%d", &x);
        t.smartInsert(x);
    }
    while (q--) {
        scanf("%d %d", &type, &x);
        switch (type) {
            case 1: {
                t.firstQuery(x);
                break;
            }
            case 2: {
                printf("%d\n", t.secondQuery(x));
                break;
            }
            case 3: {
                int kth = t.kthSmallest(x);
                kth == -1 ? printf("invalid\n") : printf("%d\n", kth);
                break;
            }
            default: {
                printf("invalid type of query\n");
            }
        }
    }
    return 0;
}
