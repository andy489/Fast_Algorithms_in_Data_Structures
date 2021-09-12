// http://codeforces.com/contest/847/problem/D

// Time complexity: O(n * log(n))
// Space complexity: O(n)

#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

typedef struct Node *pnode;

struct Node {
    int key, prior, sub, freq; // sub = subtree, freq = count of curr element
    pnode l, r;

    Node(int key) : key(key), prior(rand()), sub(1), freq(1), l(nullptr), r(nullptr) {}
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


    int less(pnode t, int key) {
        int le = 0;
        while (t) {
            if (t->key < key) {
                le += t->freq + sub(t->l);
                t = t->r;
            } else if (t->key == key) {
                return le + sub(t->l);
            } else {
                t = t->l;
            }
        }
        return le;
    }

public:
    Treap() : root(nullptr) {
        srand(time(nullptr));
    }

    ~Treap() {
        clear(this->root);
    }

    void smartInsert(int key) {
        pnode found = find(this->root, key);
        if (!found) {
            insert(this->root, new Node(key));
        } else {
            ++found->freq;
            updPath(this->root, key);
        }
    }

    int less(int key) {
        return less(this->root, key);
    }
};

int main() {
    int n, T;
    scanf("%d %d", &n, &T);

    Treap t;
    int b, res = 0; // b = bowlsOfFood

    for (int i = 1; i <= n; ++i) {
        scanf("%d", &b);
        //  we enter the time that the dog has to wait when it reaches the bowl of food
        if (T <= i) {
            //  an alternative way to manage the never reachable bowls is
            //  to insert max(b-i , 0) in the Treap
            break;
        }
        t.smartInsert(b - i);
        int le = t.less(T - i);
        res = le > res ? le : res;
    }
    return printf("%d\n", res), 0;
}
