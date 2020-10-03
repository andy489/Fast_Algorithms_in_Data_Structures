// github.com/andy489

#include <iostream>
#include <vector>
#include <list>
using namespace std;

#define ios ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr)
#define pb push_back
int n;

vector<list<int>> adj; /// adjacency list
vector<int> par; /// parent function
vector<vector<int>> table; /// A_I

void init() { /// hardcoded Tree
    n = 9, adj.resize(n + 1);
    adj[1].pb(2), adj[1].pb(3);
    adj[2].pb(1), adj[2].pb(4), adj[2].pb(5), adj[2].pb(6);
    adj[3].pb(1), adj[4].pb(2);
    adj[5].pb(2), adj[5].pb(7), adj[5].pb(8);
    adj[6].pb(2), adj[7].pb(5);
    adj[8].pb(5), adj[8].pb(9), adj[9].pb(8);
    par.resize(n + 1);
}

void dfs(int u = 1, int p = -1) {
    par[u] = p;
    for (const int &child:adj[u]) {
        if (child == p) continue;
        dfs(child, u);
    }
}

void build() { /// A_I
    table.assign(n + 1, vector<int>(n, -1));
    for (int v = 1; v <= n; ++v) {
        int p = par[v], d = 1;
        while (p != -1) {
            table[v][d++] = p, p = par[p];
        }
    }
}

int query(int v, int d) {
    if (v > n || v < 1 || d >= n || d < 0) return -1;
    else if (d == 0) return v;
    return table[v][d];
}

void solve() {
    int q, v, d; cout << "Enter number of queries \"v d\":\n";
    cin >> q;
    while (q--) {
        cin >> v >> d;
        int a = query(v, d)
        (a == -1) ? cout << "no such ancestor\n" : cout << a << '\n';
    }
}

int main() {
    return init(), dfs(), build(), solve(), 0;
}
