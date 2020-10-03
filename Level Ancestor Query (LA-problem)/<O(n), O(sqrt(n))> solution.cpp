// github.com/andy489

#include <iostream>
#include <vector>
#include <list>

using namespace std;

#define ios ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr)
#define pb push_back

int n, leaves;
vector<list<int>> adj, AL;
vector<int> par, dep, SL, mark, indexp;
vector<vector<int>> P;

void init() { /// hardcoded Tree
    n = 14, adj.resize(n + 1);
    adj[1].pb(2), adj[1].pb(3);
    adj[2].pb(1), adj[2].pb(4), adj[2].pb(5), adj[2].pb(6);
    adj[3].pb(1), adj[4].pb(2), adj[5].pb(2), adj[5].pb(7), adj[5].pb(8);
    adj[6].pb(2), adj[7].pb(5), adj[7].pb(9);
    adj[8].pb(5), adj[8].pb(14), adj[9].pb(7), adj[9].pb(11);
    adj[10].pb(14), adj[11].pb(9), adj[11].pb(12);
    adj[12].pb(11), adj[13].pb(14), adj[14].pb(8), adj[14].pb(13), adj[14].pb(10);

    par.resize(n + 1), dep.resize(n + 1, -1);
    AL.resize(n), mark.resize(n + 1), indexp.resize(n + 1);
}

void dfs(int u = 1, int p = 0) {
    par[u] = p; /// parent function fill
    dep[u] = dep[p] + 1; /// depth function fill
    if (adj[u].size() == 1) AL[dep[u]].push_back(u), leaves++; /// All Leaves array of lists
    for (const int &child:adj[u]) {
        if (child == p) continue;
        dfs(child, u);
    }
}

void counting() {
    SL.resize(leaves); /// Sorted Leaves (decreasing)
    int k = 0;
    for (int d = n - 1; d >= 0; --d) {
        while (!AL[d].empty()) {
            auto it = --AL[d].end();
            int l = *it;
            AL[d].erase(it), SL[k++] = l;
        }
    }
}

void maxPathsDecomposition() {
    for (int l = 0; l <= leaves - 1; ++l) {
        vector<int> currMaxPath; 
        int v = SL[l];
        while (v && !mark[v]) {
            currMaxPath.pb(v);
            indexp[v] = l;
            mark[v] = true;
            v = par[v];
        }
        P.pb(currMaxPath); /// P = Max Paths Decomposition
    }
}

int LAQ2(int v, int d) {
    if (d > dep[v]) return -1;
    int u = P[indexp[v]].back(); /// top element in curr max path
    int dprim = dep[v] - dep[u];
    if (d <= dprim) {
        int depFirst = dep[P[indexp[v]][0]];
        int depStart = depFirst - dep[v];
        return P[indexp[v]][depStart + d];
    }
    return LAQ2(par[u], d - dprim - 1);
}

void solve() {
    int q, v, d; cout << "Enter number of queries of the form \"v d\":\n";
    cin >> q;
    while (q--) {
        cin >> v >> d;
        int ans = LAQ2(v, d); /// O(sqrt(n))
        ans == -1 ? cout << "no such ancestor\n" : cout << ans<<'\n';
    }
}

int main() { 
  ios; 
  return init(), dfs(), counting(), maxPathsDecomposition(), solve(), 0; 
}
