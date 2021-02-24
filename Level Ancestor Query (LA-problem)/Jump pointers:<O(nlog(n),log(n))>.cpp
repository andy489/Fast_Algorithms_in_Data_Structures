// github.com/andy489

#include <iostream>
#include <vector>

using namespace std;

#define ios ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr)
#define pb push_back

int n;
vector<vector<int>> adj{{},{2,3},{1,4,5,6},{1},{2},{2,7,8},{2},{5,9},{5,14},{7,11},{14},{9,12},{11,15},{14},{8,10,13},{12,16},{15}}, jump; // adjacency lists, jump lists
vector<int> par, dep, flog; // parent, depth, floor logarithm
vector<int> traversal; // shortest path from root to current node

void pre() {
    flog.resize(n);
    for (int i = 2; i < n; ++i)
        flog[i] = flog[i / 2] + 1;
}

void init() { // hardcoded Tree
    n = (int)adj.size();
    par.resize(n), dep.resize(n, -1);
    traversal.reserve(n); // making traversal vector efficient for a stack (no resizing)
    jump.resize(n);

    pre();
}

void fillJumps(int u) {
    if (u == 1) return;
    int l = (int)traversal.size() - 1;
    jump[u].resize(flog[l] + 1);
    int k = 0, j = 1;
    while (j <= l) {
        jump[u][k++] = traversal[l - j];
        j = (j << 1);
    }
}

void dfs(int u = 1, int p = 0) {
    par[u] = p; // parent function fill
    dep[u] = dep[p] + 1; // depth function fill
    traversal.pb(u); // traversal path fill
    fillJumps(u);
    for (const int &child: adj[u]) {
        if (child == p) continue;
        dfs(child, u);
        traversal.pop_back();
    }
}

int LAQ4jmp(int v, int d) {
    if (d == 0) return v;
    if (d > dep[v]) return -1;
    int k = flog[d];
    return LAQ4jmp(jump[v][k], d - (1 << k));
}

void solve() {
    int q, v, d;
    cout << "Enter number of queries of the form \"v d\": ";
    cin >> q;
    cout<<"Enter query \"v d\":\n";
    while (q--) {
        cin >> v >> d;
        int ans = LAQ4jmp(v, d);
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
