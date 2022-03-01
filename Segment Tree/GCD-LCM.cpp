#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

#define pii pair<int, int>
#define f first
#define s second

int n;
vector<int> a;
vector<pii> t;

void init() {
    scanf("%d", &n);

    a.resize(n);
    t.resize(2 * n);

    int x;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &x);
        a[i] = x;
    }
}

int GCD(int a, int b) {
    return !b ? a : GCD(b, a % b);
}

int LCM(int a, int b) {
    return a * b / GCD(a, b);
}

pii combine(pii a, pii b) {
    return {GCD(a.f, b.f), LCM(a.s, b.s)};
}

void build(int v, int tl, int tr) {
    if (tl == tr) {
        t[v] = {a[tl], a[tl]};
    } else {
        int tm = (tl + tr) >> 1;

        build(v + 1, tl, tm);
        build(v + 2 * (tm - tl + 1), tm + 1, tr);

        t[v] = combine(t[v + 1], t[v + 2 * (tm - tl + 1)]);
    }
}

pii query(int v, int tl, int tr, int l, int r) {
    if (l > r) {
        return {0, 1};
    }

    if (l == tl && r == tr) {
        return t[v];
    }

    int tm = (tl + tr) >> 1;

    return combine(query(v + 1, tl, tm, l, min(tm, r)),
                   query(v + 2 * (tm - tl + 1), tm + 1, tr, max(tm + 1, l), r));
}

void update(int v, int tl, int tr, int pos, int new_val) {
    if (tl == tr) {
        t[v] = {new_val, new_val};
    } else {
        int tm = (tl + tr) >> 1;

        if (pos <= tm) {
            update(v + 1, tl, tm, pos, new_val);
        } else {
            update(v + 2 * (tm - tl + 1), tm + 1, tr, pos, new_val);
        }

        t[v] = combine(t[v + 1], t[v + 2 * (tm - tl + 1)]);
    }
}

int main() {
    init();

    build(1, 0, n - 1);

    int q, cmd, l, r;
    scanf("%d", &q);

    while (q--) {
        scanf("%d%d%d", &cmd, &l, &r);

        if (cmd == 1) {
            --l, --r;
            pii res = query(1, 0, n - 1, l, r);
            printf("~GCD: %d\n~LCM: %d\n", res.f, res.s);
        } else if (cmd == 2) {
            update(1, 0, n - 1, l - 1, r);
        } else {
            printf("~Bad input\n");
        }
    }

    return 0;
}
