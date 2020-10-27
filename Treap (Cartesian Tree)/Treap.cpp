// github.com/andy489

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

struct item {
    int key, prior;
    item *l, *r;
    int cnt = 0;

    item(int key, int prior) : key(key), prior(prior), l(nullptr), r(nullptr) {};
};

typedef item *pitem;

int cnt(pitem t) {
    return t ? t->cnt : 0;
}

void upd_cnt(pitem t) {
    if (t)
        t->cnt = 1 + cnt(t->l) + cnt(t->r);
}

void heapify(pitem t) {
    if (!t) return;
    pitem max = t;
    if (t->l != nullptr && t->l->prior > max->prior)
        max = t->l;
    if (t->r != nullptr && t->r->prior > max->prior)
        max = t->r;
    if (max != t) {
        swap(t->prior, max->prior);
        heapify(max);
    }
}

pitem build(int *a, int n) {
    /// Construct a treap on values {a[0], a[1], ..., a[n-1]}
    if (n == 0) return nullptr;
    int mid = n >> 1;
    pitem t = new item(a[mid], rand() % 300);
    t->l = build(a, mid);
    t->r = build(a + mid + 1, n - mid - 1);
    heapify(t);
    upd_cnt(t);
    return t;
}

void dfs(pitem t) {
    if (!t)
        return;
    dfs(t->l);
    printf("%d %d, ", t->key, t->prior);
    dfs(t->r);
}

#include <queue>
void bfs(pitem t) {
    queue<pitem> Q;
    Q.push(t);
    int SZ;
    while (!Q.empty()) {
        SZ = Q.size();
        while (SZ--) {
            pitem curr = Q.front();
            Q.pop();
            printf("%d %d, ", curr->key, curr->prior);
            if (curr->l)
                Q.push(curr->l);
            if (curr->r)
                Q.push(curr->r);
        }
        printf("\n");
    }
}

int main() {
    srand(time(nullptr));
    int a[] = {2, 4, 8, 3, 6, 22, 9, 1};
    int n = sizeof a / sizeof(int);
    pitem treap = build(a, n);
    dfs(treap);
    printf("\n\n");
    bfs(treap);
    return 0;
}
