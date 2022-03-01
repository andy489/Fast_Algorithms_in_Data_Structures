#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

const int mxM = 1e5;

int n, m;
vector<int> a[mxM], t[4 * mxM];

void init() {
    scanf("%d%d", &n, &m);

    for (int i = 0; i < n; ++i) {
        a[i].resize(n);
    }

    for (int i = 0; i < 4 * n; ++i) {
        t[i].resize(4 * m);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            scanf("%d", &a[i][j]);
        }
    }
}

void build_y(int vx, int lx, int rx, int vy, int ly, int ry) {
    if (ly == ry) {
        if (lx == rx) {
            t[vx][vy] = a[lx][ly];
        } else {
            t[vx][vy] = t[vx * 2][vy] + t[vx * 2 + 1][vy];
        }
    } else {
        int my = (ly + ry) / 2;

        build_y(vx, lx, rx, vy * 2, ly, my);
        build_y(vx, lx, rx, vy * 2 + 1, my + 1, ry);
        t[vx][vy] = t[vx][vy * 2] + t[vx][vy * 2 + 1];
    }
}

void build_x(int vx, int lx, int rx) {
    if (lx != rx) {
        int mx = (lx + rx) / 2;
        build_x(vx * 2, lx, mx);
        build_x(vx * 2 + 1, mx + 1, rx);
    }

    build_y(vx, lx, rx, 1, 0, m - 1);
}

int sum_y(int vx, int vy, int tly, int try_, int ly, int ry) {
    if (ly > ry) {
        return 0;
    }

    if (ly == tly && try_ == ry) {
        return t[vx][vy];
    }

    int tmy = (tly + try_) / 2;

    return sum_y(vx, vy * 2, tly, tmy, ly, min(ry, tmy))
           + sum_y(vx, vy * 2 + 1, tmy + 1, try_, max(ly, tmy + 1), ry);
}

int sum_x(int vx, int tlx, int trx, int lx, int rx, int ly, int ry) {
    if (lx > rx) {
        return 0;
    }

    if (lx == tlx && trx == rx) {
        return sum_y(vx, 1, 0, m - 1, ly, ry);
    }

    int tmx = (tlx + trx) / 2;

    return sum_x(vx * 2, tlx, tmx, lx, min(rx, tmx), ly, ry)
           + sum_x(vx * 2 + 1, tmx + 1, trx, max(lx, tmx + 1), rx, ly, ry);
}

void update_y(int vx, int lx, int rx, int vy, int ly, int ry, int x, int y, int new_val) {
    if (ly == ry) {
        if (lx == rx) {
            t[vx][vy] = new_val;
        } else {
            t[vx][vy] = t[vx * 2][vy] + t[vx * 2 + 1][vy];
        }
    } else {
        int my = (ly + ry) >> 1;

        if (y <= my) {
            update_y(vx, lx, rx, vy * 2, ly, my, x, y, new_val);
        } else {
            update_y(vx, lx, rx, vy * 2 + 1, my + 1, ry, x, y, new_val);
        }

        t[vx][vy] = t[vx][vy * 2] + t[vx][vy * 2 + 1];
    }
}

void update_x(int vx, int lx, int rx, int x, int y, int new_val) {
    if (lx != rx) {
        int mx = (lx + rx) >> 1;

        if (x <= mx) {
            update_x(vx * 2, lx, mx, x, y, new_val);
        } else {
            update_x(vx * 2 + 1, mx + 1, rx, x, y, new_val);
        }
    }

    update_y(vx, lx, rx, 1, 0, m - 1, x, y, new_val);
}

int main() {
    init();

    build_x(1, 0, n - 1);

    int q, cmd, x1, y1, x2, y2, new_val;
    scanf("%d", &q);
    while (q--) {
        scanf("%d", &cmd);
        if (cmd == 1) {
            scanf("%d%d%d%d", &x1, &x2, &y1, &y2);
            --x1, --x2, --y1, --y2;
            printf("%d\n", sum_x(1, 0, n - 1, x1, x2, y1, y2));
        } else if (cmd == 2) {
            scanf("%d%d%d", &x1, &y1, &new_val);
            --x1, --y1;
            update_x(1, 0, n - 1, x1, y1, new_val);
        } else
            printf("~bad cmd!\n");
    }

    return 0;
}

/*
    y1 y2 y3 y4
x1  1  0  2  0
x2  0  3  0  4
x3  5  0  6  0
x4  0  7  0  8

*/
