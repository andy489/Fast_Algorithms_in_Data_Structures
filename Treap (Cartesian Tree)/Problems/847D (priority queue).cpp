// http://codeforces.com/contest/847/problem/D

// Time complexity: O(n * log(n))
// Space complexity: O(n)

#include <cstdio>
#include <queue>

using namespace std;

int main() {
    int n, T, x, res(0);
    scanf("%d %d", &n, &T);
    int fin = min(n, T - 1);

    priority_queue<int> Q;

    for (int i = 1; i <= fin; ++i) {
        while (!Q.empty() && Q.top() >= T - i) {
            //  remove all bowls that have become inaccessible
            //  after the dog has eaten the food from the next bowl
            //  (remove those bowls for which the dog has waited too long)
            Q.pop();
        }

        scanf("%d", &x);

        if (x < T) {    // if the bowl is reachable (one way or another) - add it
            Q.push(x - i);  // add the time that the dog has to wait when it reach it
        }

        res = max(res, int(Q.size()));
    }
    return printf("%d", res), 0;
}
