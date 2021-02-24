// github.com/andy489

#include <iostream>
#include <vector>
#include <list>
using namespace std;

#define ios ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr)
#define pb push_back
int n;

vector<list<int>> adj{{},{2,3},{1,4,5,6},{1},{2},{2,7,8},{2},{5},{5,9},{8}}; // adjacency list
vector<int> par; // parent function
vector<vector<int>> table; // A_I

void init() { // hardcoded Tree
    n = adj.size();
    par.resize(n);
}

void dfs(int u = 1, int p = -1) {
    par[u] = p;
    for (const int &child:adj[u]) {
        if (child == p) continue;
        dfs(child, u);
    }
}

void build() { // A_I
    table.assign(n, vector<int>(n, -1));
    for (int v = 1; v < n; ++v) {
        int p = par[v], d = 1;
        while (p != -1) {
            table[v][d++] = p;
            p = par[p];
        }
    }
}

int query(int v, int d) {
    if (v > n || v < 1 || d >= n || d < 0) return -1;
    else if (d == 0) return v;
    return table[v][d];
}

void solve() {
    int q, v, d; cout << "Enter number of queries \"v d\": ";
    cin >> q;
    cout<<"Enter query \"v d\":\n";
    while (q--) {
        cin >> v >> d;
        int a = query(v, d);
        (a == -1) ? cout << "no such ancestor\n" : cout << a << '\n';
    }
}

int main() {
    ios;
    return init(), dfs(), build(), solve(), 0;
}
