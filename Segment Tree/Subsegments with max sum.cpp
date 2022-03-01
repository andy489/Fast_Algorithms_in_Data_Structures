#include <cstdio>
#include <algorithm>

using namespace std;

const int mxN = 1e5;

struct data_struct {
    int sum, pref, suff, ans;

    data_struct() : sum(0), pref(0), suff(0), ans(0) {};

    data_struct(int sum, int pref, int suff, int ans) :
            sum(sum), pref(pref), suff(suff), ans(ans) {};
} t[2 * mxN];

int a[mxN], n;

void init() {
    scanf("%d", &n);

    for (int i = 0; i < n; ++i) {
        scanf("%d", a + i);
    }
}

data_struct make_data(int val) {
    int v = max(val, 0);
    return data_struct{val, v, v, v};
}

data_struct combine(data_struct l, data_struct r) {
    data_struct res;
    
    res.sum = l.sum + r.sum;
    
    res.pref = max(l.pref, l.sum + r.pref);
    res.suff = max(r.suff, r.sum + l.suff);
    
    res.ans = max(max(l.ans, r.ans), l.suff + r.pref);
    
    return res;
}

void build(int v, int tl, int tr) {
    if (tl == tr) {
        t[v] = make_data(a[tl]);
    }
    else {
        int tm = (tl + tr) >> 1;
        
        build(v + 1, tl, tm);
        build(v + 2 * (tm - tl + 1), tm + 1, tr);
        
        t[v] = combine(t[v + 1], t[v + 2 * (tm - tl + 1)]);
    }
}

data_struct query(int v, int tl, int tr, int l, int r) {
    if (l > r) {
        return data_struct();
    }

    if (l == tl && r == tr) {
        return t[v];
    }

    int tm = (tl + tr) >> 1;

    return combine(query(v + 1, tl, tm, l, min(r, tm)),
                   query(v + 2 * (tm - tl + 1), tm + 1, tr, max(l, tm + 1), r));
}

void update(int v, int tl, int tr, int pos, int new_val) {
    if (tl == tr) {
        t[v] = make_data(new_val);
    } else {
        int tm = (tl = tr) >> 1;

        if (pos <= tm)
            update(v + 1, tl, tm, pos, new_val);
        else {
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

            printf("%d\n", query(1, 0, n - 1, l, r).ans);
        } else if (cmd == 2) {
            update(1, 0, n - 1, l - 1, r);
        } else {
            printf("~Bad input\n");
        }
    }
    
    return 0;
}
