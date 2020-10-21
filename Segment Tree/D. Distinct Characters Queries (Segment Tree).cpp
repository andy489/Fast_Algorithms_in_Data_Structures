// https://codeforces.com/contest/1234/problem/D

#include <cstdio>
#include <vector>

using namespace std;

const int mxN = 1e5+5;
int n;
char s[mxN];
int t[4 * mxN];

void init() {
    scanf("%s", s);
}

int ind(char c) {
    return (int)(c - 'a');
}

int countBits(int x, short i = 0, int cnt = 0) {
    for (; i < 26; ++i)
        if (x & (1 << i))
            ++cnt;
    return cnt;
}

void build(int v, int tl, int tr) {
    if (tl == tr)
        t[v] = (1 << ind(s[tl]));
    else {
        int tm = ((tl + tr) >> 1);
        build(v * 2, tl, tm);
        build(v * 2 + 1, tm + 1, tr);
        t[v] = (t[v * 2] | t[v * 2 + 1]);
    }
}

int querySymbolsCount(int v, int tl, int tr, int l, int r) {
    if (l > r)
        return 0;
    if (l == tl && r == tr)
        return t[v];
    int tm = ((tl + tr) >> 1);
    return querySymbolsCount(v * 2, tl, tm, l, min(r, tm)) |
           querySymbolsCount(v * 2 + 1, tm + 1, tr, max(1, tm + 1), r);
}

int update(int v, int tl, int tr, int pos, char newVal) {
    if (tl == tr)
        t[v] = (1<<ind(newVal));
    else {
        int tm = (tl + tr) >> 1;
        if (pos <= tm)
            update(v * 2, tl, tm, pos, newVal);
        else
            update(v * 2 + 1, tm + 1, tr, pos, newVal);
        t[v] = (t[v * 2] | t[v * 2 + 1]);
    }
}

void queries() {
    int q, l, r, cmd;
    char c;
    scanf("%d", &q);
    while (q--) {
        scanf("%d",&cmd);
        if(cmd==2) {
            scanf("%d%d", &l, &r);
            if (l < 1 || r > n)
                continue;
            --l, --r;
            printf("%d\n", countBits(querySymbolsCount(1, 0, n - 1, l, r)));
        } else{
            scanf("%d %c", &l, &c);
            --l;
            update(1,0,n-1,l,c);
        }
    }
}

int main() {
    init();
    n=strlen(s);
    build(1, 0, n-1);
    queries();
    return 0;
}
