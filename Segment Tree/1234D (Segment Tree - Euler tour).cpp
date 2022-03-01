// https://codeforces.com/contest/1234/problem/D

#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int ALPHABET_SIZE = 26;

const int mxN = 1e5;
char s[mxN];
int t[2 * mxN], n;

void init() {
    scanf("%s", s);
    n = strlen(s);
}

int ind(char c) {
    return c - 'a';
}

int countBits(int x) {
    int cnt = 0;

    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (x & (1 << i)) {
            ++cnt;
        }
    }

    return cnt;
}

// pre-order traversal (Euler)
void build_seg_tree_euler(int v, int tl, int tr) { // 2n - 1 memory
    if (tl == tr) {
        t[v] = 1 << ind(s[tl]);
    } else {
        int tm = (tl + tr) >> 1;

        build_seg_tree_euler(v + 1, tl, tm);
        build_seg_tree_euler(v + 2 * (tm - tl + 1), tm + 1, tr);
        t[v] = t[v + 1] | t[v + 2 * (tm - tl + 1)];
    }
}

int query_diff_symbols_cnt(int v, int tl, int tr, int l, int r) {
    if (l > r) {
        return 0;
    }

    if (tl == l && tr == r) {
        return t[v];
    }

    int tm = (tl + tr) >> 1;

    return query_diff_symbols_cnt(v + 1, tl, tm, l, min(r, tm)) |
           query_diff_symbols_cnt(v + 2 * (tm - tl + 1), tm + 1, tr, max(l, tm + 1), r);
}

void update(int v, int tl, int tr, int pos, char newVal) {
    if (tl == tr) {
        t[v] = 1 << ind(newVal);
    } else {
        int tm = (tl + tr) >> 1;

        if (pos <= tm) {
            update(v + 1, tl, tm, pos, newVal);
        } else {
            update(v + 2 * (tm - tl + 1), tm + 1, tr, pos, newVal);
        }

        t[v] = t[v + 1] | t[v + 2 * (tm - tl + 1)];
    }
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
            printf("%d\n", countBits(query_diff_symbols_cnt(1, 0, n - 1, l, r)));
        } else {
            scanf("%d %c", &l, &ch);
            update(1, 0, n - 1, l - 1, ch);
        }
    }
}

int main() {
    init();

    build_seg_tree_euler(1, 0, n - 1);

    queries();

    return 0;
}