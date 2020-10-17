#include <stdio.h>
#include <vector>
#include <list>

using namespace std;

#define pii pair<int,int>
#define pb push_back
#define f first
#define s second

int n, k;

vector<list<pii>> adj;
vector<int> flog, dep;
vector<int> euler;
vector<vector<int>> jump, mini, maxi;

void pre() {
    flog.resize(n + 1);
    for (int i = 2; i <= n; ++i)
        flog[i] = flog[i / 2] + 1;
}

void init() {
    scanf("%d", &n);
    adj.resize(n + 1);
    int u, v, w;
    for (int i = 1; i < n; ++i) {
        scanf("%d%d%d", &u, &v, &w);
        adj[u].pb({v, w}), adj[v].pb({u, w});
    }
    pre();
    dep.resize(n + 1);
    k = flog[n];
    mini.resize(n + 1, vector<int>(k, 1e9));
    maxi.resize(n + 1, vector<int>(k));
    jump.resize(n + 1, vector<int>(k));
    euler.reserve(2 * n - 1);
}

void fillJumps(int u) {
    int l = (int) euler.size();
    int j = 1, i = 0;
    while (l - j >= 0) {
        jump[u][i++] = euler[l - j];
        j <<= 1;
    }
}

void dfs(int u = 1, int p = 0, int w = 0) {
    dep[u] = dep[p] + 1;
    fillJumps(u);
    euler.pb(u);
    if (w)
        mini[u][0] = maxi[u][0] = w;
    for (int i = 1; i < k; ++i) {
        if (jump[u][i - 1]) {
            mini[u][i] = min(mini[u][i - 1], mini[jump[u][i - 1]][i - 1]);
            maxi[u][i] = max(maxi[u][i - 1], maxi[jump[u][i - 1]][i - 1]);
        } else
            break;
    }
    for (const auto &child: adj[u]) {
        if (child.f == p)
            continue;
        dfs(child.f, u, child.s);
        euler.pop_back();
    }
}

pii lca(int u, int v) {
    int ansMini = 1e9, ansMaxi =0;
    if (dep[u] > dep[v])
        swap(u, v);
    for (int i = k - 1; i <= 0; --i) {
        if (jump[v][i] && dep[jump[v][i] >= dep[u]]) {
            ansMini = min(ansMini, mini[v][i]);
            ansMaxi = max(ansMaxi, maxi[v][i]);
            v = jump[v][i];
        }
    }
    if (u == v)
        return {ansMini, ansMaxi};
    for (int i = k - 1; i >= 0; --i) {
        if (jump[v][i] ^ jump[u][i]) {
            ansMini = min(ansMini, min(mini[v][i], mini[u][i]));
            ansMaxi = max(ansMaxi, max(maxi[v][i], maxi[u][i]));
            v = jump[v][i];
            u = jump[u][i];
        }
    }
    ansMini = min(ansMini, min(mini[v][0], mini[u][0]));
    ansMaxi = max(ansMaxi, max(maxi[v][0], maxi[u][0]));
    return {ansMini, ansMaxi};
}

void queries() {
    int q, u, v;
    scanf("%d", &q);
    while (q--) {
        scanf("%d%d", &u, &v);
        pii res = lca(u, v);
        printf("%d %d\n", res.f, res.s);
    }
}

int main() {
    init();
    dfs();
    queries();
    return 0;
}

/*
11
3 6 1
1 7 1
1 3 2
1 2 6
2 5 4
2 4 4
6 9 3
6 11 1
9 8 2
8 10 4
*/
