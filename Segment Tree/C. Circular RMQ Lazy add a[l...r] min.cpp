// https://codeforces.com/contest/52/problem/C

#include <cstdio>
#include <algorithm>

using namespace std;

typedef long long ll;

const ll mxN = 1e6 + 5;
int n, arr[mxN];
ll t[2 * mxN], lazy[2 * mxN];

void push(int v, int c1, int c2) {
    t[c1] += lazy[v];
    lazy[c1] += lazy[v];
    t[c2] += lazy[v];
    lazy[c2] += lazy[v];
    lazy[v] = 0;
}

ll combine(ll a, ll b) {
    return min(a, b);
}

void update(int v, int tl, int tr, int l, int r, int addend) {
    if (l > r) {
        return;
    }
    if (tl == l && tr == r) {
        t[v] += addend;
        lazy[v] += addend;
    } else {
        int tm = (tl + tr) >> 1;
        
        int c1 = v + 1;
        int c2 = v + 2 * (tm - tl + 1);

        push(v, c1, c2);
        update(c1, tl, tm, l, min(r, tm), addend);
        update(c2, tm + 1, tr, max(l, tm + 1), r, addend);

        t[v] = combine(t[c1], t[c2]);
    }
}

ll query(int v, int tl, int tr, int l, int r) {
    if (l > r) {
        return 1e18;
    }

    if (l <= tl && tr <= r) {
        return t[v];
    }

    int tm = (tl + tr) >> 1;
    int c1 = v + 1, c2 = v + 2 * (tm - tl + 1);

    push(v, c1, c2);

    return combine(query(c1, tl, tm, l, min(r, tm)),
                   query(c2, tm + 1, tr, max(l, tm + 1), r));

}

void build(int v, int tl, int tr) {
    if (tl == tr) {
        t[v] = arr[tl];
    } else {
        int tm = (tl + tr) >> 1;
        int c1 = v + 1, c2 = v + 2 * (tm - tl + 1);

        build(c1, tl, tm);
        build(c2, tm + 1, tr);

        t[v] = combine(t[c1], t[c2]);
    }
}

int main() {
    scanf("%d", &n);
    for (ll i = 0; i < n; ++i) {
        scanf("%d", &arr[i]);
    }

    build(0, 0, n - 1);

    int q;
    scanf("%d", &q);

    int l, r, val;
    char c;
    while (q--) {
        scanf("%d%d%c", &l, &r, &c);

        if (c == ' ') {
            scanf("%d", &val);
            if (l <= r) {
                update(0, 0, n - 1, l, r, val);
            } else {
                update(0, 0, n - 1, 0, r, val), update(0, 0, n - 1, l, n - 1, val);
            }
        } else {
            if (l <= r) {
                printf("%lld\n", query(0, 0, n - 1, l, r));
            } else {
                printf("%lld\n", min(query(0, 0, n - 1, 0, r), query(0, 0, n - 1, l, n - 1)));
            }
        }
    }

    return 0;
}
