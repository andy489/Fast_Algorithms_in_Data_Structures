// github.com/andy489

#include <iostream>
#include <vector>

using namespace std;

#define ios ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr)
#define pb push_back

int n;
vector<vector<int>> adj, jump;
vector<int> par, dep, flog;
vector<int> traversal; /// shortest path from root to current node

void pre() {
    flog.resize(n);
    for (int i = 2; i <= n; ++i)
        flog[i] = flog[i / 2] + 1;
}

void init() { /// hardcoded Tree
    n = 16, adj.resize(n + 1);
    adj[1].pb(2), adj[1].pb(3);
    adj[2].pb(1), adj[2].pb(4), adj[2].pb(5), adj[2].pb(6);
    adj[3].pb(1), adj[4].pb(2), adj[5].pb(2), adj[5].pb(7), adj[5].pb(8);
    adj[6].pb(2), adj[7].pb(5), adj[7].pb(9);
    adj[8].pb(5), adj[8].pb(14), adj[9].pb(7), adj[9].pb(11);
    adj[10].pb(14), adj[11].pb(9), adj[11].pb(12);
    adj[12].pb(11), adj[13].pb(14), adj[14].pb(8), adj[14].pb(13), adj[14].pb(10);
    adj[12].pb(15), adj[15].pb(12), adj[16].pb(15), adj[15].pb(16);
    par.resize(n + 1), dep.resize(n + 1, -1);
    traversal.reserve(n); /// making traversal vector efficient for a stack (no resizing)
    jump.resize(n + 1);

    pre();
}

void fillJumps(int u) {
    if (u == 1) return;
    int l = traversal.size() - 1;
    jump[u].resize(flog[l] + 1);
    int k = 0, j = 1;
    while (j <= l) {
        jump[u][k++] = traversal[l - j];
        j = (j << 1);
    }
}

void dfs(int u = 1, int p = 0) {
    par[u] = p; /// parent function fill
    dep[u] = dep[p] + 1; /// depth function fill
    traversal.pb(u); /// traversal path fill
    fillJumps(u);
    for (const int &child: adj[u]) {
        if (child == p) continue;
        dfs(child, u);
        traversal.pop_back();
    }
}

int LAQ4jmp(int v, int d) {
    if (d == 0) return v;
    int k = flog[d];
    return LAQ4jmp(jump[v][k], d - (1 << k));
}

void solve() {
    int q, v, d;
    cout << "Enter number of queries of the form \"v d\":\n";
    cin >> q;
    while (q--) {
        cin >> v >> d;
        int ans = LAQ4jmp(v, d); /// O(sqrt(n))
        ans == -1 ? cout << "no such ancestor\n" : cout << ans << '\n';
    }
}

int main() {
    ios;
    init();
    dfs();
    solve();
    return 0;
}
