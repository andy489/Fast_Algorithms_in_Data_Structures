//  https://codeforces.com/contest/702/problem/F

//  Time complexity:    O(max(n*log(n), n*k*log^2(k)))
//  Space complexity:   O(max(n,k))

#include <cstdio>       // scanf(...), printf(...)
#include <cstdlib>      // rand(...)
#include <ctime>        // srand(...)
#include <vector>
#include <list>
#include <algorithm>    // sort(...)
#include <functional>   // greater<Shirt>()

using namespace std;

typedef struct Node *pnode;

struct Shirt {
    int cost, quality;

    Shirt() : cost(int()), quality(int()) {}

    Shirt(int cost, int quality) : cost(cost), quality(quality) {}

    bool operator>(const Shirt &other) const {  //  descending by quality, ascending by price
        if (this->quality == other.quality) {
            return this->cost < other.cost;
        }
        return this->quality > other.quality;
    }
};

struct Node {   // every node represents a customer
    int prior, key, id, shirts;
    // prior for keeping the tree balanced
    // key for costumers sum for buying (using it as key)
    // id for every customer
    // shirts for number of purchased shirts by each customer
    int lazy1, lazy2;   //  lazy1 = money spent, lazy2 = num t-shirts bought
    pnode l, r; // children Nodes

    Node(int money, int id) :   //  money = key
            id(id), key(money), prior(rand()), lazy1(0), lazy2(0), shirts(0), l(nullptr), r(nullptr) {}
};

struct Treap {
private:
    pnode root;
    vector<int> res;

    void clear(pnode t) {
        if (t) {
            clear(t->l);
            clear(t->r);
            delete t;
        }
    }

    void upd(pnode t, int costSpent, int tsh) {    // tsh = (1) t-shirt
        t->key += costSpent;
        t->lazy1 += costSpent;
        t->shirts += tsh;
        t->lazy2 += tsh;
    }

    void pushLazy(pnode t) {
        if (t->l) {
            upd(t->l, t->lazy1, t->lazy2);
        }
        if (t->r) {
            upd(t->r, t->lazy1, t->lazy2);
        }
        t->lazy1 = t->lazy2 = 0;
    }

    // splits into < key and >= key
    void split(pnode t, pnode &l, pnode &r, int key) {
        if (!t) {
            return void(l = r = nullptr);
        }
        pushLazy(t);
        if (key <= t->key) {
            split(t->l, l, t->l, key), r = t;
        } else {
            split(t->r, t->r, r, key), l = t;
        }
    }

    void merge(pnode &t, pnode l, pnode r) {
        if (!l || !r) {
            return void(t = l ? l : r);
        }
        pushLazy(l);
        pushLazy(r);
        if (l->prior < r->prior) {
            merge(r->l, l, r->l), t = r;
        } else {
            merge(l->r, l->r, r), t = l;
        }
    }

    void insert(pnode &t, pnode it) {
        if (!t) {
            return void(t = it);
        }
        pushLazy(t);
        pnode l, r;
        split(t, l, r, it->key);
        merge(t, l, it);
        merge(t, t, r);
    }

    void output(pnode t, vector<int> &res) {
        if (t) {
            pushLazy(t);
            output(t->l, res);
            res[t->id] = t->shirts;
            output(t->r, res);
        }
    }

    void dfs(pnode &t, list<pnode> &list) {
        if (t) {
            pushLazy(t);
            dfs(t->l, list);
            list.push_back(t);
            dfs(t->r, list);
        }
    }

    void reorder(pnode &t, int cost) {
        pnode t1, t2;
        split(t, t1, t2, cost);
        if (t2) {
            upd(t2, -cost, 1);
        }
        // those from t2 which became smaller than cost cannot propagate
        pnode t3;
        split(t2, t2, t3, cost);
        list<pnode> list;
        dfs(t2, list);

        for (const pnode &n:list) {
            n->l = n->r = nullptr;  // no propagate
            insert(t1, n);
        }
        
        merge(t, t1, t3);
    }

public:

    Treap(int k) : root(nullptr) {
        res.resize(k);
        srand(time(nullptr));
    }

    ~Treap() {
        clear(this->root);
    }

    void insert(pnode it) {
        insert(this->root, it);
    }

    void reorder(int c) {
        reorder(this->root, c);
    }

    void output() {
        output(this->root, res);
        int k = res.size();
        for (int i = 0; i < k; ++i) {
            printf("%d ", res[i]);
        }
        printf("\n");
    }

};


int main() {
    int n, k, c, q, b, i;
    scanf("%d", &n);

    vector<Shirt> Tshirts(n);

    for (i = 0; i < n; ++i) {
        scanf("%d %d", &c, &q);
        Tshirts[i] = {c, q};
    }

    sort(Tshirts.begin(), Tshirts.end(), greater<Shirt>());
    // sorted by descending order of quality and ascending order of price
    scanf("%d", &k);

    Treap T(k);
    for (i = 0; i < k; ++i) {
        scanf("%d", &b);
        T.insert(new Node(b, i));
    }

    for (i = 0; i < n; ++i) {
        c = Tshirts[i].cost, q = Tshirts[i].quality;
        T.reorder(c);
    }

    return T.output(), 0;
}
