// github.com/andy489

#include <cstdio>

using namespace std;

const int mxN = 1e5;

int n;
int a[mxN], t[4 * mxN];

void init() {
    printf("Insert length of array:\n");
    scanf("%d", &n);
    printf("Insert array elements:\n");
    for (int i = 0; i < n; ++i)
        scanf("%d", &a[i]);
}

void build(int v, int tl, int tr) {
    if (tl == tr)
        t[v] = a[tl];
    else {
        int tm = ((tl + tr) >> 1);
        build(v * 2, tl, tm);
        build(v * 2 + 1, tm + 1, tr);
        t[v] = t[v * 2] + t[v * 2 + 1];
    }
}

int sum(int v, int tl, int tr, int l, int r) {
    if (l > r)
        return 0;
    if (l == tl && r == tr)
        return t[v];
    int tm = ((tl + tr) >> 1);
    return sum(v * 2, tl, tm, l, min(r, tm)) +
           sum(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r);
}

int update(int v, int tl, int tr, int pos, int newVal) {
    if (tl == tr)
        a[v] = newVal;
    else {
        int tm = (tl + tr) >> 1;
        if (pos <= tm)
            update(v * 2, tl, tm, pos, newVal);
        else
            update(v * 2 + 1, tm + 1, tr, pos, newVal);
        t[v] = t[v * 2] + t[v * 2 + 1];
    }
}

void queries(){
    int q, l, r;
    printf("Insert number of queries:\n");
    scanf("%d", &q);
    printf("Insert queries of the form \"l r\":\n");
    while (q--) {
        scanf("%d%d", &l, &r);
        if(l==0){
            int pos, newValue;
            printf("Please update \"pos newValue\":\n");
            scanf("%d%d", &pos, &newValue);
            --pos;
            if(pos < 0 || pos > n)
                printf("~Bad position\n");
            a[pos]=newValue;
            continue;
        }
        if (l < 1 || r > n) {
            printf("~bad interval bounds\n");
            continue;
        }
        --l, --r;
        printf("%d\n", sum(1, 0, n - 1, l, r));
    }
}

int main() {
    init();
    build(1, 0, n - 1);
    queries();
    return 0;
}
