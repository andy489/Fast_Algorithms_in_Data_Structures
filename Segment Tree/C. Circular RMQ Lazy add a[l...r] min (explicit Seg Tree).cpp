// https://codeforces.com/contest/52/problem/C

#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long ll;

static const int SIZE = 1e6+5;

int n, q;
int arr[SIZE];

struct segment_node {
    ll value;
    ll carry;

    int from;
    int to;

    segment_node* left;
    segment_node* right;

    void push() {
        value += carry;

        if (left) {
            left->carry += carry;
            right->carry += carry;
        }

        carry = 0;
    }
} *root, memory_holder[2 * SIZE];

int memory_cnt;

void input() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", arr + i);
    }
}

void build(segment_node* node, int f, int t) {
    node->from = f;
    node->to = t;
    node->carry = 0;

    if (f == t) {
        node->value = arr[f];
        node->left = node->right = nullptr;
        return;
    }

    node->left = &memory_holder[memory_cnt++]; // new Node;
    node->right = &memory_holder[memory_cnt++]; // new Node;

    int mid = (f + t) / 2;
    build(node->left, f, mid);
    build(node->right, mid + 1, t);

    node->value = min(node->left->value, node->right->value);
}

void update(segment_node* node, int f, int t, int v) {
    node->push();

    if (t < node->from || f > node->to) {
        return;
    }

    if (f <= node->from && node->to <= t) {
        node->carry += v;
        node->push();
        return;
    }

    update(node->left, f, t, v);
    update(node->right, f, t, v);

    node->value = min(node->left->value, node->right->value);
}

ll find_min(segment_node* node, int f, int t) {
    node->push();

    if (t < node->from || f > node->to) {
        return 1e18;
    }

    if (f <= node->from && node->to <= t) {
        return node->value;
    }

    return min(find_min(node->left, f, t), find_min(node->right, f, t));
}

void solve() {
    scanf("%d", &q);
    char c;
    int l, r, v;
    while(q--) {
        scanf("%d%d%c", &l,&r,&c);
        if (c==' ') {
            scanf("%d",&v);
            if(l<=r){
                update(root, l, r, v);
            } else {
                update(root, 0, r, v), update(root, l, n-1, v);
            }
        } else{
            if(l<=r){
                printf("%lld\n", find_min(root, l, r));
            } else {
                printf("%lld\n", min(find_min(root, 0, r), find_min(root, l, n-1)));
            }
        }
    }
}

int main() {
    input();
    root = new segment_node();
    build(root, 0, n - 1);
    solve();
    return 0;
}
