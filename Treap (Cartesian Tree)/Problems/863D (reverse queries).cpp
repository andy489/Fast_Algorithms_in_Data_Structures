// https://codeforces.com/contest/863/problem/D

// Ideq: for each request for index x return all queries in which x falls (works only if m is small enough)

// Time complexity: О(m * q)
// Space complexity: O(max(n,q))

#include<cstdio>
#include <vector>

using namespace std;

int main() {
    int n, q, m;
    scanf("%d %d %d", &n, &q, &m);

    vector<int> a(n + 1), type(q + 1), l(q + 1), r(q + 1);
    int x;

    for (int i = 1; i <= n; ++i) {
        scanf("%d", &a[i]);
    }

    for (int i = 1; i <= q; ++i) {
        scanf("%d %d %d", &type[i], &l[i], &r[i]);
    }

    while (m--) {
        scanf("%d", &x);
        for (int i = q; i >= 1; --i) {
            if (x >= l[i] && x <= r[i]) {   // if it is in the curr query range
                if (type[i] == 2) { // reve
                    x = l[i] + r[i] - x;
                } else if (x == l[i]) { //cyc corner case
                    x = r[i];
                } else {    // cyc
                    --x;
                }
            }
        }
        printf("%d ", a[x]);
    }
    return 0;
}
