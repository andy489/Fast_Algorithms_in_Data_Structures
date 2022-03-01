#include <cstdio>
#include <vector>
#include <set>

using namespace std;

const int mxN = 1e5;

int n;
vector<int> a(mxN);
multiset<int> t[2 * mxN];

void init() {
    scanf("%d", &n);

    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
    }
}

void build(int v, int tl, int tr) {
    if (tl == tr) {
        auto it = a.begin() + tr;

        t[v] = multiset<int>(it, it + 1);
    } else {
        int tm = (tl + tr) >> 1;

        build(v + 1, tl, tm);
        build(v + 2 * (tm - tl + 1), tm + 1, tr);

        merge(t[v + 1].begin(), t[v + 1].end(),
              t[v + 2 * (tm - tl + 1)].begin(), t[v + 2 * (tm - tl + 1)].end(),
              inserter(t[v], t[v].end()));
    }
}


int query_succ(int v, int tl, int tr, int l, int r, int x) {
    if (l > r) {
        return 1e9;
    }

    if (l == tl && r == tr) {
        auto pos = t[v].lower_bound(x);

        if (pos != t[v].end()) {
            return *pos;
        }

        return 1e9;
    }

    int tm = (tl + tr) >> 1;

    return min(query_succ(v + 1, tl, tm, l, min(r, tm), x),
               query_succ(v + 2 * (tm - tl + 1), tm + 1, tr, max(l, tm + 1), r, x));
}

void update(int v, int tl, int tr, int pos, int new_val) {
    t[v].erase(t[v].find(a[pos]));
    t[v].insert(new_val);

    if (tl != tr) {
        int tm = (tl + tr) >> 1;

        if (pos <= tm) {
            update(v + 1, tl, tm, pos, new_val);
        } else {
            update(v + 2 * (tm - tl + 1), tm + 1, tr, pos, new_val);
        }
    } else {
        a[pos] = new_val;
    }
}


int main() {
    init();

    build(1, 0, n - 1);

    int q, cmd, l, r, x;
    scanf("%d", &q);
    while (q--) {
        scanf("%d%d%d", &cmd, &l, &r);
        if (cmd == 1) {
            --l, --r;
            scanf("%d", &x);
            printf("%d\n", query_succ(1, 0, n - 1, l, r, x));
        } else if (cmd == 2) {
            update(1, 0, n - 1, l - 1, r);
        } else {
            printf("~Bad input\n");
        }
    }

    return 0;
}
