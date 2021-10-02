//  https://codeforces.com/contest/1181/problem/D

//  Time complexity:    О((m+q)*log(m))
//  Space complexity:   O(m+q)

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <functional>

typedef long long ll;

using namespace std;

typedef struct Node *pnode;

struct Node {
    int key, prior, cnt;

    pnode l, r;

    Node(int key) : key(key), prior(rand()), cnt(1), l(nullptr), r(nullptr) {}
};


struct Treap {
private:
    pnode root;
    int sz;

    void clear(pnode t) {
        if (t) {
            clear(t->l);
            clear(t->r);
            delete t;
        }
    }

    int cnt(pnode it) {
        return it ? it->cnt : 0;
    }

    void updCnt(pnode it) {
        if (it) {
            it->cnt = 1 + cnt(it->l) + cnt(it->r);
        }
    }

    void merge(pnode &t, pnode l, pnode r) {
        if (!l || !r) {
            return void(t = l ? l : r);
        }
        if (l->prior < r->prior) {
            merge(r->l, l, r->l), t = r;
        } else {
            merge(l->r, l->r, r), t = l;
        }
        updCnt(t);
    }

    int kthSmallest(pnode t, int k) {
        if (cnt(t) < k) {
            return -1;
        }
        while (t) {
            int lst = cnt(t->l);
            if (lst >= k) {
                t = t->l;
            } else if (lst + 1 >= k) {
                return t->key;
            } else {
                k -= lst + 1;
                t = t->r;
            }
        }
    }

    void split(pnode t, pnode &l, pnode &r, int key) {
        if (!t)
            l = r = nullptr;
        else if (key < t->key) {
            split(t->l, l, t->l, key), r = t;
        } else {
            split(t->r, t->r, r, key), l = t;
        }
        updCnt(t);
    }

    void insert(pnode &t, pnode it) {
        if (!t) {
            t = it;
        } else if (it->prior > t->prior) {
            split(t, it->l, it->r, it->key), t = it;
        } else {
            insert(it->key < t->key ? t->l : t->r, it);
        }
        updCnt(t);
    }

public:
    Treap() : root(nullptr), sz(int()) {
        srand(time(nullptr));
    }

    ~Treap() {
        clear(this->root);
    }

    int size() {
        return this->sz;
    }

    void insert(int key) {
        ++this->sz;
        insert(this->root, new Node(key));
    }

    int kthSmallest(int k) {
        return kthSmallest(this->root, k);
    }
};

struct City {
    int hosted, id;

    City() : hosted(int()), id(int()) {}

    City(int hosted, int id) : hosted(hosted), id(id) {}

    bool operator<(const City &other) const {
        if (this->hosted == other.hosted) {
            return this->id < other.id;
        }
        return this->hosted < other.hosted;
    }
};

struct Query {
    ll year;
    int id;

    Query() : year(ll()), id(int()) {}

    Query(ll year, int id) : year(year), id(id) {}

    bool operator<(const Query &other) const {
        return this->year < other.year;
    }
};

int main() {
    int n, m, q;
    scanf("%d %d %d", &n, &m, &q);

    vector<City> cities(m + 2, City());

    for (int i = 1; i <= m; ++i) {
        cities[i].id = i;
    }

    cities[m + 1].hosted = 1e9;

    for (int i = 0, a; i < n; i++) {
        scanf("%d", &a);
        ++cities[a].hosted;
    }
    // sort cities by number of households in ascending order
    sort(cities.begin(), cities.end(), less<>());

    ll k;
    vector<Query> queries(q, Query());
    for (int i = 0; i < q; i++) {
        scanf("%lld", &k);
        queries[i] = Query(k, i);
    }

    sort(queries.begin(), queries.end(), less<>());

    Treap T;

    vector<int> ans(q, int());

    ll next = ++n, prev = next;
    int queryIndex = 0, cityIndex = 1;

    while (T.size() < m) {  //  while there are cities to skip
        Query Q = queries[queryIndex];  // take the current query Q
        if (next <= Q.year) {
            prev = next;
            T.insert(cities[cityIndex++].id);   //  insert column to fill in histogram
            //  add = blocks difference in the histogram between two cities with consecutive indices
            int add = cities[cityIndex].hosted - cities[cityIndex - 1].hosted;
            next += add * 1ll * T.size();
        } else {
            ll order = Q.year - prev;
            if (order >= T.size()) {
                order %= T.size();
            }
            ans[Q.id] = T.kthSmallest(order + 1);
            ++queryIndex;
        }
    }

    for (int i = queryIndex; i < q; ++i) {
        int res = queries[i].year % m;
        ans[queries[i].id] = res ? res : m;
    }

    for (int i = 0; i < q; ++i) {
        printf("%d\n", ans[i]);
    }

    return 0;
}

/*
8 5 4
3 2 1 2 2 3 2 2
11
16
10
34
*/
