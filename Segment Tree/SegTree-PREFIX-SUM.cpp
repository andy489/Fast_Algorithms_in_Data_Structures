// github.com/andy489
// Searching for an array prefix with a given amount

#include <stdio.h>
#include <algorithm>

using namespace std;

const int mxN = 1e5;
int a[mxN], t[2 * mxN], n;

void init() {
    scanf("%d", &n);
    int x;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &x);
        a[i] = x;
    }
}

void build(int v, int tl, int tr) {
    if (tl == tr)
        t[v] = a[tl];
    else {
        int tm = tl + tr >> 1;
        build(v + 1, tl, tm);
        build(v + 2 * (tm - tl + 1), tm + 1, tr);
        t[v] = t[v + 1] + t[v + 2 * (tm - tl + 1)];
    }
}

int sum(int v, int tl, int tr, int l, int r) {
    if (l > r)
        return 0;
    if (l == tl && r == tr)
        return t[v];
    int tm = tl + tr >> 1;
    return sum(v + 1, tl, tm, l, min(r, tm)) +
           sum(v + 2 * (tm - tl + 1), tm + 1, tr, max(l, tm + 1), r);
}

int update(int v, int tl, int tr, int pos, int newVal) {
    if (tr == tl)
        t[v] = newVal;
    else {
        int tm = tl + tr >> 1;
        if (pos <= tm)
            update(v + 1, tl, tm, pos, newVal);
        else
            update(v + 2 * (tm - tl + 1), tm + 1, tr, pos, newVal);
        t[v] = t[v + 1] + t[v + 2 * (tm - tl + 1)];
    }
}

int prefix(int v, int tl, int tr, int s) {
    if (t[v] < s)
        return -1;
    if (tl == tr)
        return tr;
    int tm = tl + tr >> 1;
    if (t[v + 1] >= s)
        return prefix(v + 1, tl, tm, s);
    else
        return prefix(v + 2 * (tm - tl + 1), tm + 1, tr, s - t[v + 1]);
}

int main() {
    init();
    build(1, 0, n - 1);

    int q, cmd, pos, newVal, s;
    scanf("%d", &q);
    while (q--) {
        scanf("%d", &cmd);
        if (cmd == 1) {
            scanf("%d", &s);
            printf("Index: %d\n", prefix(1, 0, n - 1, s));
        } else if (cmd == 2) {
            scanf("%d%d", &pos, &newVal);
            update(1, 0, n - 1, pos - 1, newVal);
        } else {
            printf("~Bad input\n");
        }
    }
    return 0;
}
