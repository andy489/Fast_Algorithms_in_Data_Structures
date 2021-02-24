// github.com/andy489

#include <iostream>
#include <vector>
#include <list>

using namespace std;

#define ios ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr)
#define pb push_back

int n, leaves;
vector<list<int>> adj{{},{2,3},{1,4,5,6},{1},{2},{2,7,8},{2},{5,9},{5,14},{7,11},{14},{9,12},{11},{14},{8,10,13}}, AL; // All Leaves
vector<int> par, dep, SL, mark, indexp; // parent, depth, Sorted Leaves, visited, max-Path index
vector<vector<int>> P, L; // P = max/long-Paths, L = ladders

void init() { // hardcoded Tree
    n = (int)adj.size();
    par.resize(n), dep.resize(n, -1);
    AL.resize(n-1), mark.resize(n), indexp.resize(n);
}

void dfs(int u = 1, int p = 0) {
    par[u] = p; // parent function fill
    dep[u] = dep[p] + 1; // depth function fill
    if (adj[u].size() == 1) AL[dep[u]].push_back(u), leaves++; // All Leaves array of lists
    for (const int &child:adj[u]) {
        if (child == p) continue;
        dfs(child, u);
    }
}

void counting() {
    SL.resize(leaves); // Sorted Leaves (decreasing)
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
        currMaxPath.reserve(20);
        int v = SL[l];
        while (v && !mark[v]) {
            currMaxPath.pb(v);
            indexp[v] = l;
            mark[v] = true;
            v = par[v];
        }
        P.pb(currMaxPath); // P = Max Paths Decomposition
    }
}

void ladders() {
    L.resize(leaves);
    // we start from the second path, the first contains the root
    L[0]=P[0];
    for (int i = 1; i < leaves; ++i) {
        int SIZE = (int)P[i].size();
        L[i].reserve(2 * SIZE + 1);
        L[i].resize(SIZE);
        int j = 0;
        for (; j < SIZE; ++j) {
            L[i][j] = P[i][j];
        }
        int v = par[P[i].back()];
        L[i].pb(v);
        while (SIZE-- && par[v]) {
            v = par[v];
            L[i].pb(v);
        }
    }
}

int LAQ3(int v, int d) {
    if (d > dep[v]) return -1;
    int u = L[indexp[v]].back(); // top element in curr ladder
    int dprim = dep[v] - dep[u];
    if (d <= dprim) {
        int depFirst = dep[L[indexp[v]][0]];
        int depStart = depFirst - dep[v];
        return L[indexp[v]][depStart + d];
    }
    return LAQ3(par[u], d - dprim - 1);
}

void solve() {
    int q, v, d;
    cout << "Enter number of queries of the form \"v d\": ";
    cin >> q;
    cout<<"Enter query \"v d\":\n";
    while (q--) {
        cin >> v >> d;
        int ans = LAQ3(v, d);
        ans == -1 ? cout << "no such ancestor\n" : cout << ans << '\n';
    }
}

int main() {
    ios;
    init();
    dfs();
    counting();
    maxPathsDecomposition();
    ladders();
    solve();
    return 0;
}
