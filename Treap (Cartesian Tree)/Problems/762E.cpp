//  https://codeforces.com/contest/762/problem/E
//  https://codeforces.com/problemset/problem/762/E

//  Time complexity: O((2k + 1) * n * log(n))
//  Space complexity: O(n), assuming that n > k, i.e. O(min(k, n))

#include <cstdio>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MXF = 10000;

#define pb push_back

typedef struct Node *pnode;

struct Station {
    int x, r, f;

    Station(int x, int r, int f) : x(x), r(r), f(f) {}

    bool operator<(const Station &other) const {
        return this->r < other.r;
    }

    bool operator>(const Station &other) const {
        return this->r < other.r;
    }
};

struct Node {
    int key, prior, cnt;
    pnode l, r;

    Node(int key) : key(key), prior(rand()), cnt(1), l(nullptr), r(nullptr) {
    }
};

struct Treap {
private:
    pnode root;

    void clear(pnode t) {
        if (!t) {
            return;
        }
        clear(t->l);
        clear(t->r);
        delete t;
    }

    int cnt(pnode t) {
        return t ? t->cnt : 0;
    }

    void updCnt(pnode t) {
        if (t) {
            t->cnt = 1 + cnt(t->l) + cnt(t->r);
        }
    }

    void split(pnode t, int key, pnode &l, pnode &r) {
        if (!t) {
            l = r = nullptr;
        } else if (key <= t->key) {
            split(t->l, key, l, t->l), r = t;
        } else {
            split(t->r, key, t->r, r), l = t;
        }
        updCnt(t);
    }

    void merge(pnode &t, pnode l, pnode r) {
        if (!l || !r) {
            t = l ? l : r;
        } else if (l->prior > r->prior) {
            merge(l->r, l->r, r), t = l;
        } else {
            merge(r->l, l, r->l), t = r;
        }
        updCnt(t);
    }

    void insert(pnode &t, pnode it) {
        if (!t) {
            t = it;
        } else if (it->prior > t->prior) {
            split(t, it->key, it->l, it->r), t = it;
        } else {
            insert(it->key < t->key ? t->l : t->r, it);
        }
        updCnt(t);
    }

    void erase(pnode &t, int key) {
        if (!t) {
            return;
        }
        if (t->key == key) {
            pnode toDel = t;
            merge(t, t->l, t->r);
            delete toDel;
        } else {
            erase(t->key > key ? t->l : t->r, key);
        }
        updCnt(t);
    }

    unsigned leq(pnode t, int key) {
        unsigned res(0);
        while (t) {
            if (key < t->key) {
                t = t->l;
            } else {
                res += 1 + cnt(t->l);
                t = t->r;
            }
        }
        return res;
    }

public:
    Treap() : root(nullptr) {
        srand(time(0));
    }

    ~Treap() {
        clear(this->root);
    }

    void insert(int key) {
        insert(this->root, new Node(key));
    }

    void erase(int key) {
        erase(this->root, key);
    }

    unsigned leq(int key) { // less or equal to
        pnode traversal = this->root;
        return leq(traversal, key);
    }
};

// using insert(..) and erase(..) on Treaps
unsigned firstApproach(int n, int k, vector<Station> &rs, vector<Treap> &treaps) {
    int x, r, f;
    for (int i = 0; i < n; ++i) {
        scanf("%d %d %d", &x, &r, &f);
        rs.pb(Station(x, r, f));
        treaps[f].insert(x);
    }

    sort(rs.begin(), rs.end());

    unsigned ans(0);
    for (const Station &s:rs) {  // for every station from radioStations
        x = s.x, r = s.r, f = s.f;
        int left = max(1, f - k), right = min(MXF, f + k);
        for (int j = left; j <= right; ++j) {
            ans += treaps[j].leq(x + r) - treaps[j].leq(x - r - 1);
        }
        treaps[f].erase(x);
    }
    return ans - n;   // since we have also been counting the pairs {rs[i], rs[i]}
}

// using only insert(..) on Treaps
unsigned secondApproach(int n, int k, vector<Station> &rs, vector<Treap> &treaps) {
    int x, r, f;
    for (int i = 0; i < n; ++i) {
        scanf("%d %d %d", &x, &r, &f);
        rs.pb(Station(x, r, f));
    }

    sort(rs.begin(), rs.end(), greater<Station>()); // O(n * log(n))
    //sort(rs.begin(), rs.end()); // reversedBegin, reversedEnd will also work (no need to overload operator > )

    unsigned ans(0);
    for (int i = n - 1; i >= 0; --i) {  // for every station from radioStations
        x = rs[i].x, r = rs[i].r, f = rs[i].f;
        for (int j = max(1, f - k); j <= min(MXF, f + k); ++j) {    // O((2k + 1) * log(n))
            ans += treaps[j].leq(x + r) - treaps[j].leq(x - r - 1);
        }
        treaps[f].insert(x);
    }
    return ans;
}


int main() {
    int n, k, x, r, f;
    scanf("%d %d", &n, &k);

    vector<Station> rs; // radioStations
    rs.reserve(n);
    vector<Treap> treaps(MXF + 1);

    // unsigned ans = firstApproach(n, k, rs, treaps);
    unsigned ans = secondApproach(n, k, rs, treaps);    // O((2k + 1) * n * log(n))

    return printf("%u\n", ans), 0;
}
