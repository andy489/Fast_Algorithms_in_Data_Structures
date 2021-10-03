//  https://www.hackerrank.com/challenges/find-the-running-median/problem

//  Time complexity:  O(n*log(n))
//  Space complexity: O(n)

#include <cstdio>
#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;

struct Heap {
private:
    bool minFlag = true;
    vector<int> tree;

    int par(int ind) {      // parent
        return ind >> 1;    // ind / 2
    }

    int left(int ind) {     // left Child
        return ind << 1;    // ind * 2
    }

    int right(int ind) {        // right Child
        return (ind << 1) | 1;  // ind * 2 + 1
    }

    void siftUpRecursive(int ind) {
        if (ind > 1 && tree[par(ind)] > tree[ind]) {
            swap(tree[par(ind)], tree[ind]);
            siftUpRecursive(par(ind));
        }
    }

    void siftUpIterative(int ind) {
        int child = ind;
        int parent = par(child);
        while (tree[child] < tree[parent] && parent) {
            swap(tree[child], tree[parent]);
            child = parent;
            parent = par(child);
        }
    }

    void siftDownRecursive(int ind) {
        if (left(ind) >= (int) tree.size()) {
            return;
        }
        int minChildInd = left(ind);
        int r = right(ind);
        if (r < (int) tree.size() && tree[r] < tree[minChildInd]) {
            minChildInd = r;
        }
        if (tree[ind] <= tree[minChildInd]) {
            return;
        }
        swap(tree[ind], tree[minChildInd]);
        siftDownRecursive(minChildInd);
    }

    void siftDownIterative(int ind) {
        int parent = ind;
        while (true) {
            int l = left(parent), r = right(parent);
            int SIZE = tree.size(), smallest = parent;

            if (l < SIZE && tree[l] < tree[smallest]) {
                smallest = l;
            }
            if (r < SIZE && tree[r] < tree[smallest]) {
                smallest = r;
            }
            if (smallest == parent) break;
            swap(tree[smallest], tree[parent]);
            parent = smallest;
        }
    }

    void buildTreapLinear(const vector<int> vec) {
        tree.clear();
        tree.push_back(0);
        for (int x:vec) {
            if (!minFlag) {
                x *= -1;
            }
            tree.push_back(x);
        }
        int SIZE = tree.size();
        for (int i = (SIZE - 1) / 2; i > 0; --i) {
            siftDownIterative(i);
        }
        /*
                (0 * n/2) + (1 * n/4) + (2 * n/8) + ... + (h * 1) =
                = sum_{k=1}^{h}\frac{kn}{2^{k+1}} = \frac{n}{4}\sum_{k=1}^{h}\frac{k}{2^{k-1}} <
                < \frac{n}{4}\sum_{k=1}^{\infty}\frac{k}{2^{k-1}} =
                = \frac{n}{4}\sum_{k=1}^{\infty}kx^{k-1} =, x=1/2
                = \frac{n}{4}\frac{d}{dx}\left[\sum_{k=0}^{\infty}x^k\right] =
                = \frac{n}{4}\frac{d}{dx}\left[\frac{1}{1-x}\right] = \frac{n}{4}\frac{1}{(1-x)^2} =
                = \frac{n}{4}\times\frac{1}{(1-1/2)^2} = n.
        */
    }

public:
    Heap(bool minFlag = true) : minFlag(minFlag) {
        tree.push_back(0);
    }

    Heap(const vector<int> &other, bool minFlag = true) : minFlag(minFlag) {
        buildTreapLinear(other);
    }

    int size() {
        int SIZE = tree.size();
        return SIZE > 1 ? SIZE - 1 : 0;
    }

    bool empty() {
        return tree.size() == 1;
    }

    void push(int x) {
        if (!minFlag) {
            x *= -1;
        }
        tree.push_back(x);
        siftUpIterative(tree.size() - 1);
    }

    int top() {
        assert(tree.size() > 1);  // check if Heap is not empty
        if (!minFlag) {
            return -tree[1];
        }
        return tree[1];
    }

    int pop() {
        assert(tree.size() > 1);
        int toReturn = tree[1];
        if (!minFlag) {
            toReturn *= -1;
        }
        tree[1] = tree.back();
        tree.pop_back();
        siftDownIterative(1);
        return toReturn;
    }
};

vector<int> heapSort(const vector<int> vec, bool flagMin = true) {
    vector<int> sorted;
    sorted.reserve(vec.size());
    Heap H(vec, flagMin);
    while (!H.empty()) {
        sorted.push_back(H.pop());
    }
    return sorted;
}

void printVec(const vector<int> &vec) {
    int SIZE = vec.size();
    for (int i = 0; i < SIZE; ++i) {
        printf("%d ", vec[i]);
    }
    printf("\n");
}

int main() {
    Heap r;
    Heap l(false);

    int n, x;
    scanf("%d", &n);
    while (n--) {
        scanf("%d", &x);
        if (l.empty()) {
            l.push(x);
        } else if (x > l.top()) {
            r.push(x);
        } else {
            l.push(x);
        }
        // balance
        if (l.size() > r.size() + 1) {
            r.push(l.top()), l.pop();
        } else if (r.size() > l.size()) {
            l.push(r.top()), r.pop();
        }
        if (l.size() > r.size()) {
            printf("%d.0\n", l.top());
        } else {
            printf("%.1f\n", (l.top() + r.top()) / 2.0);
        }
    }
    return 0;
}
