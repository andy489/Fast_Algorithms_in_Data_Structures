// github.com/andy489

// https://codeforces.com/contest/1234/problem/D

#include <stdio.h>
#include <string.h>
#include <algorithm>

using namespace std;

const int mxN = 1e5;
int t[2 * mxN + 1];
int n;
char s[mxN];

void init() {
    scanf("%s", s);
    n = strlen(s);
}

int ind(char c) {
    return c - 'a';
}

int countOnes(int x, short i = 0, short cnt = 0) {
    for (; i < 26; ++i)
        if (x & (1 << i))
            ++cnt;
    return cnt;
}
// pre-order traversal (Euler)
void buildEuler(int v, int tl, int tr) { // 2n - 1 memory
    if (tl == tr)
        t[v] = 1 << ind(s[tl]);
    else {
        int tm = tl + tr >> 1;
        buildEuler(v + 1, tl, tm);
        buildEuler(v + 2 * (tm - tl + 1), tm + 1, tr);
        t[v] = t[v + 1] | t[v + 2 * (tm - tl + 1)];
    }
}

void update(int pos, char newVal, int v, int tl, int tr) {
    if (tr < pos || tl > pos)
        return;
    if (tl == tr) {
        t[v] = (1 << ind(newVal));
    } else {
        int tm = (tl + tr) / 2;
        update(pos, newVal, v+1, tl, tm);
        update(pos, newVal, v + 2 * (tm - tl + 1), tm + 1, tr);
        t[v] = (t[v+1] | t[v + 2 * (tm - tl + 1)]);
    }
}

int querySymbolCount(int v, int tl, int tr, int l, int r) {
    if (l > r)
        return 0;
    if (tl == l && tr == r)
        return t[v];
    int tm = (tl + tr) >> 1;
    return (querySymbolCount(v+1, tl, tm, l, min(r, tm)) |
            querySymbolCount(v + 2 * (tm - tl + 1), tm + 1, tr, max(l, tm + 1), r));
}

void queries() {
    int q, cmd, l, r;
    char ch;
    scanf("%d", &q);
    while (q--) {
        scanf("%d", &cmd);
        if (cmd == 2) {
            scanf("%d%d", &l, &r);
            --l, --r;
            printf("%d\n", countOnes(querySymbolCount(1, 0, n - 1, l, r)));
        } else {
            scanf("%d %c", &l, &ch);
            update(l - 1, ch, 1, 0, n - 1);
        }
    }
}

int main() {
    init();
    buildEuler(1, 0, n - 1);
    queries();
    return 0;
}
