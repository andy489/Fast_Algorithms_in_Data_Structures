// github.com/andy489

#include <stdio.h>
#include <algorithm>

using namespace std;

const int mxN = 1e5, mxM = 1e1;

int n, a[mxN], t[2 * mxN]; // 2 * mxN - 1, but root is 1

void init() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
        scanf("%d", a + i);
}

void build(int v, int tl, int tr) {
    if (tl == tr)
        t[v] = a[tl];
    else {
        int tm = tl + tr >> 1;
        build(v + 1, tl, tm);
        build(v + 2 * (tm - tl + 1), tm + 1, tr);
        t[v] = min(t[v + 1], t[v + 2 * (tm - tl + 1) + 1 - 1]);
    }
}

int mini(int v, int tl, int tr, int l, int r) {
    if (l > r)
        return 1e9;
    if (tl == tr)
        return t[v];
    int tm = tl + tr >> 1;
    return min(mini(v + 1, tl, tm, l, min(r, tm)),
               mini(v + 2 * (tm - tl + 1), tm + 1, tr, max(l, tm + 1), r));
}

void update(int v, int tl, int tr, int pos, int newVal) {
    if (tl == tr)
        t[v] = newVal;
    else {
        int tm = tl + tr >> 1;
        if (pos <= tm)
            update(v + 1, tl, tm, pos, newVal);
        else
            update(v + 2 * (tm - tl + 1), tm + 1, tr, pos, newVal);
        t[v] = min(t[v + 1], t[v + 2 * (tm - tl + 1)]);
    }
}

int main() {
    init();
    build(1, 0, n - 1);

    int q, l, r;
    char cmd[mxM];
    scanf("%d", &q);
    while (q--) {
        scanf("%s", cmd);
        if (!strcmp(cmd, "u")) { // update
            scanf("%d%d", &l, &r);
            update(1, 0, n - 1, l - 1, r);
        } else if (!strcmp(cmd, "q")) { // query
            scanf("%d%d", &l, &r);
            --l, --r;
            printf("%d\n", mini(1, 0, n - 1, l, r));
        }
    }

    return 0;
}
