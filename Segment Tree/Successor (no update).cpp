#include <cstdio>
#include <vector>

using namespace std;

const int mxN = 1e5;

int a[mxN], n;
vector<int> t[2 * mxN];

void init() {
    scanf("%d", &n);

    for (int i = 0; i < n; ++i) {
        scanf("%d", a + i);
    }
}

void build(int v, int tl, int tr) {
    if (tl == tr) {
        t[v] = vector<int>(1, a[tl]);
    } else {
        int tm = (tl + tr) >> 1;

        build(v + 1, tl, tm);
        build(v + 2 * (tm - tl + 1), tm + 1, tr);

        merge(t[v + 1].begin(), t[v + 1].end(),
              t[v + 2 * (tm - tl + 1)].begin(), t[v + 2 * (tm - tl + 1)].end(),
              back_inserter(t[v]));
    }
}

int query_succ(int v, int tl, int tr, int l, int r, int x) {
    if (l > r) {
        return 1e9;
    }

    if (l == tl && r == tr) {
        auto pos = lower_bound(t[v].begin(), t[v].end(), x);
        if (pos != t[v].end()) {
            return *pos;
        }

        return 1e9;
    }
    int tm = (tl + tr) >> 1;

    return min(query_succ(v + 1, tl, tm, l, min(r, tm), x),
               query_succ(v + 2 * (tm - tl + 1), tm + 1, tr, max(l, tm + 1), r, x));
}

int main() {
    init();

    build(1, 0, n - 1);

    int q, cmd, l, r, x;

    scanf("%d", &q);
    while (q--) {
        scanf("%d%d%d%d", &cmd, &l, &r, &x);
        if (cmd == 1) {
            --l, --r;
            printf("%d\n", query_succ(1, 0, n - 1, l, r, x));
        } else if (cmd == 2) {
            // no modifications
        } else {
            printf("~Bad input\n");
        }
    }

    return 0;
}
