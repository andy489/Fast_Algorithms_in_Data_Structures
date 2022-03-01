// counting the number of zeros, searching for the k-th zero

#include <cstdio>
#include <algorithm>

using namespace std;

const int mxN = 1e5;
int a[mxN], t[2 * mxN], n;

int count_zeroes(int a, int b) {
    return a + b;
}

void init() {
    scanf("%d", &n);
    int x;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &x);

        a[i] = x;
    }
};

void build(int v, int tl, int tr) {
    if (tl == tr) {
        t[v] = (a[tl] == 0);
    } else {
        int tm = (tl + tr) >> 1;

        build(v + 1, tl, tm);
        build(v + 2 * (tm - tl + 1), tm + 1, tr);

        t[v] = count_zeroes(t[v + 1], t[v + 2 * (tm - tl + 1)]);
    }
}

int query(int v, int tl, int tr, int l, int r) {
    if (l > r) {
        return 0;
    }

    if (l == tl && r == tr) {
        return t[v];
    }

    int tm = (tl + tr) >> 1;

    return count_zeroes(query(v + 1, tl, tm, l, min(r, tm)),
                        query(v + 2 * (tm - tl + 1), tm + 1, tr, max(l, tm + 1), r));
}

void update(int v, int tl, int tr, int pos, int new_val) {
    if (tl == tr) {
        t[v] = (new_val == 0);
    } else {
        int tm = (tl + tr) >> 1;

        if (pos <= tm) {
            update(v + 1, tl, tm, pos, new_val);
        } else {
            update(v + 2 * (tm - tl + 1), tm + 1, tr, pos, new_val);
        }

        t[v] = count_zeroes(t[v + 1], t[v + 2 * (tm - tl + 1)]);
    }
}

int find_kth(int v, int tl, int tr, int k) {
    if (k > t[v]) {
        return -1;
    }

    if (tl == tr) {
        return tl;
    }

    int tm = (tl + tr) >> 1;

    if (t[v + 1] >= k) {
        return find_kth(v + 1, tl, tm, k);
    } else {
        return find_kth(v + 2 * (tm - tl + 1), tm + 1, tr, k - t[v + 1]);
    }
}

int main() {
    init();

    build(1, 0, n - 1);

    int q, cmd, l, r, k;
    scanf("%d", &q);
    while (q--) {
        scanf("%d%d%d", &cmd, &l, &r);
        if (cmd == 1) {
            scanf("%d", &k);
            if (k > r - l + 1) {
                printf("~Out of boundaries\n");
                continue;
            }

            --l, --r;

            int q1 = query(1, 0, n - 1, l, r);
            int kth = find_kth(1, 0, n - 1, k);

            printf("Count of zeros: %d\nk-th zero's index: %d\n", q1, kth);
        } else if (cmd == 2) {
            update(1, 0, n - 1, l - 1, r);
        } else {
            printf("~Bad command\n");
        }

    }

    return 0;
}
