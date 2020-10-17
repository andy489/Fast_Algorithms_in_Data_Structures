/*
github.com/andy489
Complexity: <O(n.log(n), log(n))>
Link: http://www.spoj.com/problems/DISQUERY/
*/

#include <stdio.h>
#include <vector>

using namespace std;

#define pii pair<int,int>
#define pb push_back
#define f first
#define s second

const int mxN = 1e5 + 5;
const int mxLog = 17;

vector<pii > graph[mxN];

int par[mxN][mxLog], dep[mxN], mini[mxN][mxLog], maxi[mxN][mxLog];

void dfs(int u = 1, int p = -1, int w = 0) {
    int i;
    par[u][0] = p;
    if (p != -1) {
        dep[u] = dep[p] + 1;
        mini[u][0] = maxi[u][0] = w;
    }
    for (i = 1; i < mxLog; i++) {
        if (par[u][i - 1] != -1) {
            par[u][i] = par[par[u][i - 1]][i - 1];
            maxi[u][i] = max(maxi[u][i - 1], maxi[par[u][i - 1]][i - 1]);
            mini[u][i] = min(mini[u][i - 1], mini[par[u][i - 1]][i - 1]);
        } else
            break;
    }
    for (const auto &child : graph[u])
        if (child.f != p)
            dfs(child.f, u, child.s);
}

pii lca(int u, int v) {
    int i;
    int ansMini = 1e10, ansMaxi = -1;
    if (dep[u] > dep[v])
        swap(u, v);
    for (i = mxLog - 1; i >= 0; --i) {
        if (par[v][i] != -1 && dep[par[v][i]] >= dep[u]) {
            ansMini = min(ansMini, mini[v][i]);
            ansMaxi = max(ansMaxi, maxi[v][i]);
            v = par[v][i];
        }
    }
    if (v == u)
        return {ansMini, ansMaxi};
    for (i = mxLog - 1; i >= 0; --i) {
        if (par[v][i] != par[u][i]) {
            ansMini = min(ansMini, min(mini[v][i], mini[u][i]));
            ansMaxi = max(ansMaxi, max(maxi[v][i], maxi[u][i]));
            v = par[v][i];
            u = par[u][i];
        }
    }
    ansMini = min(ansMini, min(mini[v][0], mini[u][0]));
    ansMaxi = max(ansMaxi, max(maxi[v][0], maxi[u][0]));
    return {ansMini, ansMaxi};
}

int main() {
    int n, i, j;
    scanf("%d", &n);
    for (i = 1; i < n; i++) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        graph[u].pb({v, w});
        graph[v].pb({u, w});
    }
    for (i = 1; i <= n; ++i) {
        for (j = 0; j < mxLog; ++j) {
            par[i][j] = -1;
            maxi[i][j] = -1;
            mini[i][j] = 1e10;
        }
    }
    dfs();
    
    int q;
    scanf("%d", &q);
    while (q--) {
        int u, v;
        scanf("%d%d", &u, &v);
        pii ans = lca(u, v);
        printf("%d%d\n", ans.f, ans.s);
    }
    
    return 0;
}
