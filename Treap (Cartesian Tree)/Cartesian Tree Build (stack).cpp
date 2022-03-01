#include <iostream>
#include <vector>
#include <stack>

using namespace std;

vector<int> build(const int *A, int n) {
    vector<int> parent(n, -1);
    stack<int> s;

    for (int i = 0; i < n; ++i) {
        int last = -1;

        while (!s.empty() && A[s.top()] >= A[i]) {
            last = s.top();
            s.pop();
        }

        if (!s.empty()) {
            parent[i] = s.top();
        }

        if (last >= 0) {
            parent[last] = i;
        }

        s.push(i);
    }
    return parent;
}

#include <queue>

void display(int u, vector<vector<int>> &adj) {
    queue<int> Q;
    Q.push(u);
    int SZ;
    while (!Q.empty()) {
        SZ = Q.size();
        while (SZ--) {
            int cur = Q.front();
            Q.pop();
            printf("%d ", cur);
            for (const int &child: adj[cur]) {
                Q.push(child);
            }

        }
        cout << endl;
    }
}

int main() {
    int A[] = {7, 3, 500, 12, 1001, 10, 17, 128};
    int n = sizeof A / sizeof(int);

    vector<int> CartesianTree = build(A, n);

    vector<vector<int>> adj(n, vector<int>());

    int root = -1;

    for (int i = 0; i < n; ++i) {
        if (CartesianTree[i] != -1) {
            adj[CartesianTree[i]].push_back(i);
        } else {
            root = i;
        }
    }

    return display(root, adj), 0;
}

