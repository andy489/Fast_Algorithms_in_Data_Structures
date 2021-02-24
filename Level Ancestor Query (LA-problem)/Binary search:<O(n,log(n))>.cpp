// github.com/andy489

#include <iostream>
#include <vector>
#include <list>

using namespace std;

#define ios ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr)
#define pb push_back

int n, timer; // occurance time

vector<list<int>> adj{{},{3,2},{1,6},{1,4,7},{3},{7,8},{2},{3,5,9},{5},{7}}; // adjacency list
vector<vector<int>> LVL; // level lists
vector<int> preorder, occurance, depth;

void init() { // hardcoded Tree
    n = (int)adj.size();
    LVL.resize(n-1);
    preorder.resize(n);
    occurance.resize(n);
    depth.resize(n);
}

void dfs(int u = 1, int par = -1, int d = 0) { // O(n) preprocess
    ++timer;
    depth[u] = d;
    occurance[u] = timer;
    preorder[timer] = u;
    LVL[d].pb(u);
    for (const int &child:adj[u]) {
        if (child == par) continue;
        dfs(child, u, d + 1);
    }
}

int bs(int el, int lvl) { // O(logn) binary search
    int l = 0, r = (int)LVL[lvl].size() - 1;
    int mid, ans(0);
    while (l <= r) {
        mid = (l + r) >> 1;
        int a = occurance[LVL[lvl][mid]];
        if (a < el) {
            ans = a;
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    return preorder[ans];
}

int LAQ3(int v, int d) {
    if (d == 0) return v;
    if (d > depth[v]) return -1;
    d = depth[v] - d; // O(1)
    int ancestor = bs(occurance[v], d); // O(log(n))
    return ancestor;
}

int main() {
    ios;
    init();
    dfs();

    int q, v, d; cout << "Enter number of queries of the form \"v d\": ";
    cin >> q;
    cout<<"Enter query \"v d\":\n";
    while (q--) {
        cin >> v >> d;
        int a = LAQ3(v, d);
        (a == -1) ? cout << "no such ancestor\n" : cout << LAQ3(v, d) << '\n';
    }
    return 0;
}
