#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

#define pii pair<int,int>
#define f first
#define s second

const int mxN = 1e5;
int a[mxN], n;
vector<pii > t(2 * mxN);

int init() {
    scanf("%d", &n);

    for (int i = 0; i < n; ++i) {
        scanf("%d", a + i);
    }
}

pii combine(pii l, pii r) {
    if (l.f > r.f) {
        return l;
    }

    if (l.f < r.f) {
        return r;
    }

    return {l.f, l.s + r.s};
}

void build(int v, int tl, int tr) {
    if (tl == tr) {
        t[v] = {a[tl], 1};
    } else {
        int tm = (tl + tr) >> 1;

        build(v + 1, tl, tm);
        build(v + 2 * (tm - tl + 1), tm + 1, tr);

        t[v] = combine(t[v + 1], t[v + 2 * (tm - tl + 1)]);
    }
}

pii getMax(int v, int tl, int tr, int l, int r) {
    if (l > r) {
        return {-1e9, 0};
    }

    if (l == tl && r == tr) {
        return t[v];
    }

    int tm = (tl + tr) >> 1;

    return
            combine(getMax(v + 1, tl, tm, l, min(tm, r)),
                    getMax(v + 2 * (tm - tl + 1), tm + 1, tr, max(tm + 1, l), r));
}

void update(int v, int tl, int tr, int pos, int new_val) {
    if (tl == tr) {
        t[v] = {new_val, 1};
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
        scanf("%d %d %d", &cmd, &l, &r);
        if (cmd == 1) {
            --l, --r;

            pii ans = getMax(1, 0, n - 1, l, r);
            tie(l, r) = ans;

            printf("~MAX element: %d\n~Number of occurrences: %d\n", l, r);
        } else if (cmd == 2) {
            update(1, 0, n - 1, l - 1, r);
        } else {
            printf("~bad cmd\n");
        }
    }
    
    return 0;
}
