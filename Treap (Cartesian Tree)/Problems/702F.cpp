//  https://codeforces.com/contest/702/problem/F

//  Time complexity:  O(max(max(n*log(n), k*log(k)), mxRanges*n*log(n))
//  Space complexity: O(max(n, max(k, mxRanges)))
//  mxRanges is defined constant equal to 2047

#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

#define mxRanges 1<<11-1

struct Shirt {
    int cost, quality;

    Shirt() : cost(int()), quality(int()) {}

    Shirt(int cost, int quality) : cost(cost), quality(quality) {}

    bool operator<(const Shirt &other) const {
        if (this->quality == other.quality) {
            return this->cost > other.cost;
        }
        return this->quality < other.quality;
    }
};

struct Buyer {
    int money, id, quantity;

    Buyer() : money(int()), id(int()), quantity(int()) {}

    Buyer(int money, int id) : money(money), id(id), quantity(int()) {}

    bool operator<(const Buyer &other) const {
        return this->money < other.money;
    }

    bool operator<(const int &money) const {
        return this->money < money;
    }
};

struct Range {
    int l, r, moneySpent, ans;

    Range() : l(int()), r(int()), moneySpent(int()), ans(int()) {}

    Range(int l, int r, int moneySpent = 0, int ans = 0) : l(l), r(r), moneySpent(moneySpent), ans(ans) {}
};

void update(vector<Range> &Ranges, vector<Buyer> &Buyers) {
    for (int i = 0, SIZE = Ranges.size(); i < SIZE; ++i) {  // for every range
        for (int j = Ranges[i].l; j < Ranges[i].r; ++j) {   // for every position of curr range scope
            Buyers[j].quantity += Ranges[i].ans;
            Buyers[j].money += Ranges[i].moneySpent;
        }
    }
    sort(Buyers.begin(), Buyers.end());
    Ranges.clear();
}

int main() {
    int n, c, q, k, m;  // n = num of T-shirt types, c = cost/price, q = quality, k = num costumers, m = money
    scanf("%d", &n);
    vector<Shirt> Shirts(n);

    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &c, &q);
        Shirts[i] = {c, q};
    }

    scanf("%d", &k);
    vector<Buyer> Buyers(k);
    for (int i = 0; i < k; ++i) {
        scanf("%d", &m);
        Buyers[i] = {m, i};
    }

    sort(Shirts.rbegin(), Shirts.rend());
    sort(Buyers.begin(), Buyers.end());

    vector<Range> Ranges;
    Ranges.reserve(mxRanges + 1);

    Ranges.emplace_back(0, k);
    // Breaking the whole interval by buyers into smaller intervals
    for (int i = 0; i < n; ++i) {   //  for all T-Shirt prices
        int cost = Shirts[i].cost;
        for (int j = 0, SIZE = Ranges.size(); j < SIZE; ++j) {  // for all Ranges
            Range cur = Ranges[j];
            if (Buyers[cur.l].money + cur.moneySpent >= cost) { // if poorest buyer from Range can buy the T-shirt
                Ranges[j].moneySpent -= cost;   // all Buyers from the interval can buy the T-shirt
                ++Ranges[j].ans;                // 1 T-shirt bought for all buyers in range
            } else if (Buyers[cur.r - 1].money + cur.moneySpent >= cost) {  // if richest buyer can afford the T-shirt
                // then there is an range of buyers, with length at least 1 that can afford the T-shirt
                // pos = position (including) from where all the buyers to right can buy the T-shirt; [pos...r]
                auto start = Buyers.begin();
                int pos = lower_bound(start + cur.l, start + cur.r, cost - cur.moneySpent) - start;
                Ranges.emplace_back(pos, cur.r, cur.moneySpent - cost, ++cur.ans);
                Ranges[j].r = pos;
            }
        }
        if (Ranges.size() > mxRanges) { // take care of ranges, they grow with quadratic complexity
            update(Ranges, Buyers);
            Ranges.emplace_back(0, k);
        }
    }
    update(Ranges, Buyers);

    vector<int> quantities(k);
    for (int i = 0; i < k; ++i) {
        quantities[Buyers[i].id] = Buyers[i].quantity;
    }

    for (int i = 0; i < k; ++i) {
        printf("%d ", quantities[i]);
    }

    return printf("\n"), 0;
}
