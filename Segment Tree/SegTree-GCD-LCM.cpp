// github.com/andy489

#include <stdio.h>
#include <algorithm>

using namespace std;

const int mxN = 1e5;
int a[mxN], tGCD[2 * mxN], tLCM[2 * mxN], n;

void init() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
        scanf("%d", a + i);
}

int GCD(int a, int b) {
    return !b ? a : GCD(b, a % b);
}

int LCM(int a, int b) {
    return a * b / GCD(a, b);
}

void buildGCD(int v, int tl, int tr) {
    if (tl == tr)
        tGCD[v] = a[tl];
    else {
        int tm = tl + tr >> 1;
        buildGCD(v + 1, tl, tm);
        buildGCD(v + 2 * (tm - tl + 1), tm + 1, tr);
        tGCD[v] = GCD(tGCD[v + 1], tGCD[v + 2 * (tm - tl + 1)]);
    }
}

void buildLCM(int v, int tl, int tr) {
    if (tl == tr)
        tLCM[v] = a[tl];
    else {
        int tm = tl + tr >> 1;
        buildLCM(v + 1, tl, tm);
        buildLCM(v + 2 * (tm - tl + 1), tm + 1, tr);
        tLCM[v] = LCM(tLCM[v + 1], tLCM[v + 2 * (tm - tl + 1)]);
    }
}

int queryGCD(int v, int tl, int tr, int l, int r) {
    if (l > r)
        return 0;
    if (l == tl && r == tr)
        return tGCD[v];
    int tm = tl + tr >> 1;
    return GCD(queryGCD(v + 1, tl, tm, l, min(tm, r)),
               queryGCD(v + 2 * (tm - tl + 1), tm + 1, tr, max(tm + 1, l), r));
}

int queryLCM(int v, int tl, int tr, int l, int r) {
    if (l > r)
        return 1;
    if (l == tl && r == tr)
        return tLCM[v];
    int tm = tl + tr >> 1;
    return LCM(queryLCM(v + 1, tl, tm, l, min(tm, r)),
               queryLCM(v + 2 * (tm - tl + 1), tm + 1, tr, max(tm + 1, l), r));
}

void updateGCD(int v, int tl, int tr, int pos, int newVal) {
    if (tl == tr)
        tGCD[v] = newVal;
    else {
        int tm = tl + tr >> 1;
        if (pos <= tm)
            updateGCD(v + 1, tl, tm, pos, newVal);
        else
            updateGCD(v + 2 * (tm - tl + 1), tm + 1, tr, pos, newVal);
        tGCD[v] = GCD(tGCD[v + 1], tGCD[v + 2 * (tm - tl + 1)]);
    }
}

void updateLCM(int v, int tl, int tr, int pos, int newVal) {
    if (tl == tr)
        tLCM[v] = newVal;
    else {
        int tm = tl + tr >> 1;
        if (pos <= tm)
            updateLCM(v + 1, tl, tm, pos, newVal);
        else
            updateLCM(v + 2 * (tm - tl + 1), tm + 1, tr, pos, newVal);
        tLCM[v] = LCM(tLCM[v + 1], tLCM[v + 2 * (tm - tl + 1)]);
    }
}

int main() {
    init();
    buildGCD(1, 0, n - 1);
    buildLCM(1,0,n-1);
    int q, cmd, l, r;
    scanf("%d", &q);
    while (q--) {
        scanf("%d%d%d", &cmd, &l, &r);
        if (cmd == 1) {
            --l, --r;
            printf("~GCD: %d\n~LCM: %d\n",
                   queryGCD(1, 0, n - 1, l, r),
                   queryLCM(1, 0, n - 1, l, r));
        } else if (cmd == 2) {
            updateGCD(1, 0, n - 1, l - 1, r);
            updateLCM(1, 0, n - 1, l - 1, r);
        } else
            printf("~Bad input\n");
    }
    return 0;
}
