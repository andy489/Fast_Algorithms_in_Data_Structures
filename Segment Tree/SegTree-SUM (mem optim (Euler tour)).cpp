// github.com/andy489

#include <stdio.h>
#include <algorithm>

int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int n = sizeof a / sizeof(int);
int t[2 * n];

using namespace std;

void build(int v, int tl, int tr) {
    if (tl == tr)
        t[v] = a[tl];
    else {
        int tm = tl + tr >> 1;
        build(v + 1, tl, tm);
        build(v + 2 * (tm - tl + 1) - 1 + 1, tm + 1, tr);
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

void update(int v, int tl, int tr, int pos, int newVal) {
    if (tl == tr)
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

int main() {
    build(1, 0, n - 1);
    int q, cmd, l, r;
    scanf("%d", &q);
    while (q--) {
        scanf("%d", &cmd);
        if (cmd == 1) {
            scanf("%d%d", &l, &r);
            --l, --r;
            printf("%d\n", sum(1, 0, n - 1, l, r));
        } else {
            scanf("%d%d", &l, &r); //  l = pos, r = x
            update(1, 0, n - 1, l - 1, r);
        }
    }
    return 0;
}
