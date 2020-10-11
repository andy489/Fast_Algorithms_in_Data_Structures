// github.com/andy489

#include <iostream>
#include <vector>
#include <list>

using namespace std;

#define ios ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr)
#define pb push_back

int n, c;

vector<list<int>> adj;
vector<vector<int>> LVL;
vector<int> preorder, occurance, depth;

void init() { /// hardcoded Tree
    n = 9, adj.resize(n + 1);
    adj[1].pb(3), adj[1].pb(2);
    adj[3].pb(1), adj[3].pb(4), adj[3].pb(7);
    adj[2].pb(1), adj[2].pb(6);
    adj[4].pb(3);
    adj[7].pb(3), adj[7].pb(9), adj[7].pb(5);
    adj[6].pb(2);
    adj[9].pb(7);
    adj[5].pb(7), adj[5].pb(8);
    adj[8].pb(5);

    LVL.resize(n);
    preorder.resize(n + 1);
    occurance.resize(n + 1);
    depth.resize(n + 1);
}

void dfs(int u = 1, int par = -1, int d = 0) { /// O(n) preprocess
    ++c;
    depth[u] = d;
    occurance[u] = c;
    preorder[c] = u;
    LVL[d].pb(u);
    for (const int &child:adj[u]) {
        if (child == par) continue;
        dfs(child, u, d + 1);
    }
}

int bs(int el, int lvl) { /// O(logn) binary search
    int l = 0, r = LVL[lvl].size() - 1;
    int mid, ans;
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
    d = depth[v] - d; /// O(1)
    int ancestor = bs(occurance[v], d); /// O(log(n))
    return ancestor;
}


int main() {
    ios;
    init();
    dfs();

    int q, v, d;
    cin >> q;
    while (q--) {
        cin >> v >> d;
        int a = LAQ3(v, d);
        (a == -1) ? cout << "no such ancestor\n" : cout << LAQ3(v, d) << '\n';
    }

    return 0;
}
