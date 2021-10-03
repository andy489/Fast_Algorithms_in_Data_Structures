//	https://www.hackerrank.com/challenges/find-the-running-median/problem

//	Time complexity:    O(n*log(n))
//	Space complexity:   O(n)

#include <cstdio>
#include <cstdlib>
#include <algorithm>

using namespace std;

typedef struct Node *pnode;

struct Node {
    int val;
    pnode l, r;

    Node(int val) : val(val), l(nullptr), r(nullptr) {}
};

struct ProbabilisticMinHeap {
private:
    pnode root;
    int sz;

    void clear(pnode &t) {
        if (t) {
            clear(t->l);
            clear(t->r);
            delete t;
        }
    }

    void merge(pnode &t, pnode l, pnode r) {
        if (!l || !r) {
            return void(t = l ? l : r);
        }
        if (l->val > r->val) {
            swap(l, r);
        }
        if (rand() & 1) {
            swap(l->l, l->r);
        }
        merge(l->r, l->r, r), t = l;
    }

    void pop(pnode &t) {
        pnode toDel = t;
        merge(t, t->l, t->r);
        delete (toDel);
    }

public:

    ProbabilisticMinHeap() : root(nullptr), sz(int()) {}

    ~ProbabilisticMinHeap() {
        clear(this->root);
    }

    int size() {
        return this->sz;
    }

    void push(int el) {
        ++this->sz;
        merge(this->root, this->root, new Node(el));
    }

    int top() {

        return this->root->val;
    }

    void pop() {
        --this->sz;
        pop(this->root);
    }
};

struct ProbabilisticMaxHeap {
private:
    pnode root;
    int sz;

    void clear(pnode &t) {
        if (t) {
            clear(t->l);
            clear(t->r);
            delete t;
        }
    }

    void merge(pnode &t, pnode l, pnode r) {
        if (!l || !r) {
            return void(t = l ? l : r);
        }
        if (l->val < r->val) {
            swap(l, r);
        }
        if (rand() & 1) {
            swap(l->l, l->r);
        }
        merge(l->r, l->r, r), t = l;
    }

    void pop(pnode &t) {
        pnode toDel = t;
        merge(t, t->l, t->r);
        delete (toDel);
    }

public:

    ProbabilisticMaxHeap() : root(nullptr), sz(int()) {}

    ~ProbabilisticMaxHeap() {
        clear(this->root);
    }

    int size() {
        return this->sz;
    }

    void push(int el) {
        ++this->sz;
        merge(this->root, this->root, new Node(el));
    }

    int top() {

        return this->root->val;
    }

    void pop() {
        --this->sz;
        pop(this->root);
    }
};

int main() {
    ProbabilisticMinHeap r;
    ProbabilisticMaxHeap l;

    int n, x;
    scanf("%d", &n);
    while (n--) {
        scanf("%d", &x);
        if (!l.size()) {
            l.push(x);
        } else if (x > l.top()) {
            r.push(x);
        } else {
            l.push(x);
        }
        // balance
        if (l.size() > r.size() + 1) {
            r.push(l.top()), l.pop();
        } else if (r.size() > l.size()) {
            l.push(r.top()), r.pop();
        }
        if (l.size() > r.size()) {
            printf("%d.0\n", l.top());
        } else {
            printf("%.1f\n", (l.top() + r.top()) / 2.0);
        }
    }
    return 0;
}
