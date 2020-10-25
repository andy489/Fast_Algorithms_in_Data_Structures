// github.com/andy489

#include <stdio.h>
#include <algorithm>

using namespace std;

struct data {
    int sum, pref, suff, ans;

    data() : sum(0), pref(0), suff(0), ans(0) {};

    data(int sum, int pref, int suff, int ans) :
            sum(sum), pref(pref), suff(suff), ans(ans) {};
};

const int mxN = 1e5;

int a[mxN], n;
data t[2 * mxN];

void init() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
        scanf("%d", a + i);
}

data makeData(int val) {
    int v = max(val, 0);
    return data{val, v, v, v};
}

data combine(data l, data r) {
    data res;
    res.sum = l.sum + r.sum;
    res.pref = max(l.pref, l.sum + r.pref);
    res.suff = max(r.suff, r.sum + l.suff);
    res.ans = max(max(l.ans, r.ans), l.suff + r.pref);
    return res;
}

void build(int v, int tl, int tr) {
    if (tl == tr)
        t[v] = makeData(a[tl]);
    else {
        int tm = tl + tr >> 1;
        build(v + 1, tl, tm);
        build(v + 2 * (tm - tl + 1), tm + 1, tr);
        t[v] = combine(t[v + 1], t[v + 2 * (tm - tl + 1)]);
    }
}

data query(int v, int tl, int tr, int l, int r) {
    if (l > r)
        return data();
    if (l == tl && r == tr)
        return t[v];
    int tm = tl + tr >> 1;
    return combine(query(v + 1, tl, tm, l, min(r, tm)),
                   query(v + 2 * (tm - tl + 1), tm + 1, tr, max(l, tm + 1), r));
}

void update(int v, int tl, int tr, int pos, int newVal) {
    if (tl == tr)
        t[v] = makeData(newVal);
    else {
        int tm = tl = tr >> 1;
        if (pos <= tm)
            update(v + 1, tl, tm, pos, newVal);
        else
            update(v + 2 * (tm - tl + 1), tm + 1, tr, pos, newVal);
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
            printf("%d\n", query(1, 0, n - 1, l, r).ans);
        } else if (cmd == 2)
            update(1, 0, n - 1, l - 1, r);
        else
            printf("~Bad input\n");
    }
    return 0;
}
