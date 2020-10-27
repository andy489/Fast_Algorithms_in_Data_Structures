// github.com/andy489

#include <cstdio>
#include <algorithm>

using namespace std;

const int mxN = 1e5;
int a[mxN], t[2 * mxN - 1], lazy[2 * mxN - 1], n;

void init() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
        scanf("%d", a + i);
}

void build(int v, int tl, int tr) {
    if (tl == tr)
        t[v] = a[tl];
    else {
        int tm = (tl + tr) >> 1;
        build(v + 1, tl, tm);
        build(v + 2 * (tm - tl + 1), tm + 1, tr);
        t[v] = max(t[v], t[v + 2 * (tm - tl + 1)]);
    }
}

void push(int v, int tl, int tm) {
    int c1 = v + 1, c2 = v + 2 * (tl - tm + 1);
    t[c1] += lazy[v];
    t[c2] += lazy[v];
    lazy[c1] += lazy[v];
    lazy[c2] += lazy[v];
    lazy[v] = 0;
}

int query(int v, int tl, int tr, int l, int r) {
    if (l > r)
        return -1e9;
    if (l <= tl && tr <= r)
        return t[v];
    int tm = (tl + tr) >> 1;
    push(v, tl, tm);
    return max(query(v + 1, tl, tm, l, min(r, tm)),
               query(v + 2 * (tm - tl + 1), tm + 1, tr, max(l, tm + 1), r));
}

void update(int v, int tl, int tr, int l, int r, int addend) {
    if (l > r)
        return;
    if (l == tl && r == tr) {
        t[v] += addend;
        lazy[v] += addend;
    } else {
        int tm = (tl + tr) >> 1;
        push(v, tl, tm);
        update(v + 1, tl, tm, l, min(r, tm), addend);
        update(v + 2 * (tm - tl + 1), tm + 1, tr, max(l, tm + 1), r, addend);
        t[v] = max(t[v + 1], t[v + 2 * (tm - tl + 1)]);
    }
}

int main() {
    init();
    build(0, 0, n - 1);

    int q, cmd, l, r, addend;
    scanf("%d", &q);
    while (q--) {
        scanf("%d%d%d", &cmd, &l, &r);
        --l, --r;
        if (cmd == 1)
            printf("%d\n", query(0, 0, n - 1, l, r));
        else if (cmd == 2) {
            scanf("%d", &addend);
            update(0, 0, n - 1, l, r, addend);
        } else
            printf("~Bad input!\n");

    }
    return 0;
}
