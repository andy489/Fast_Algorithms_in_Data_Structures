#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <cassert>

using namespace std;

typedef long long ll;

class VanEmdeBoasTree{
    struct Node{
        ll u, size; // 2^u capacity (Universe size), size
        ll min, max;
        Node *summary;
        vector<Node*> cluster;

        Node(int u=1) { // Constructor for Base Node with capacity=2
            min = max = -1;
            this->u=u; // size of Universe 2^u
            size = 0; // No elements at the initialization
            summary = nullptr;
            cluster.assign(1LL << ((u + 1)>>1), nullptr);
        }

        Node(ll x, int _u) : u(_u){ // Constructor with one element x
            size = 1;   // capacity is 2^u, but size is 1
            min = max = x; // only one element x=min=max
            summary = nullptr;
            cluster.assign(1LL << ((u + 1)>>1), nullptr);
        }
    } *root;
    
    ll U;
    int logU;
    
    void insert(Node *V, ll x){ // Inserting el. x in vEB with root V
        if(V->size == 0){ // Insert in empty vEB
            V->min = V->max = x;
            V->size = 1;
            return;
        }
        if(x == V->min || x == V->max){ // already present in vEB
            return;
        }
        if(V->size == 1){ //V->min==V->max
            if(x < V->max) {
                V->min = x;
            } else {
                V->max = x;
            }
            ++V->size;
            return;
        }
        if(x > V->max){
            swap(x, V->max); // exchange max with x and insert max
            return insert(V, x);
        }
        if(x < V->min){
            swap(x, V->min); // exchange min with x and insert min
            return insert(V, x);
        }
        ll U1 = (V->u>>1);
        ll high = (x>>U1); // x/(2^u/2)
        ll low = x-(high<<U1); // the right shifted bits are lost (0)
        if(V->cluster[high]->size == 0){ // cluster where x is is empty
            insert(V->summary, high); // insert in that cluster trough summary
        }
        ll prevSize = V->cluster[high]->size; // previous size of current cluster
        insert(V->cluster[high], low); // insert in cluster with offset low
        V->size += (V->cluster[high]->size - prevSize); // actualize size properly
    }
    
    void erase(Node *V, ll x){
        if(V->size == 0 || x < V->min || x > V->max){ // such element not reached
            return;
        }
        if(V->size == 1){
            V->min = V->max = -1; // empty vEB
            V->size = 0;
            return;
        }
        if(V->size == 2){ // Base Case vEB
            if(x == V->min){
                V->min = V->max; // actualize new min (old is deleted)
                --V->size;
            }
            else if(x == V->max){
                V->max = V->min; // actualize new max (old is deleted)
                --V->size;
            }
            return;
        }
        if(x == V->max){ // we want to erase the max element (it is not preasent in subClusters)
            ll newMaxHigh = V->summary->max; // fast find next prev max el. cluster
            ll newMaxLow = V->cluster[newMaxHigh]->max; // fast find prev max el. position
            V->max = (newMaxHigh<<(V->u>>1)) + newMaxLow; // create index to new max el. (erasing max)
            erase(V->cluster[newMaxHigh], newMaxLow); // delete prev max, which is assigned to max
            if(V->cluster[newMaxHigh]->size == 0){ // actualize summary
                erase(V->summary, newMaxHigh);
            }
            --V->size; // actualize size
            return;
        }
        if(x == V->min){ // Dual to x == V->max
            ll newMinHigh = V->summary->min;
            ll newMinLow = V->cluster[newMinHigh]->min;
            V->min = (newMinHigh<<(V->u>>1)) + newMinLow;
            erase(V->cluster[newMinHigh], newMinLow);
            if(V->cluster[newMinHigh]->size == 0){
                erase(V->summary, newMinHigh);
            }
            --V->size;
            return;
        } // if we are here, we have V->min < x < V->max (general case)
        ll high = (x>>(V->u>>1));
        ll low = x-(high<<(V->u>>1));
        ll prevSize = V->cluster[high]->size;
        erase(V->cluster[high], low); // traverse with shrink factor u/2 (sqrt(U))
        V->size -= (prevSize - V->cluster[high]->size); // actualize size
        if(V->cluster[high]->size == 0){
            erase(V->summary, high); // actialize summary
        }
    }
 
    ll predecessor(Node *V, ll x) const{
        if(V->size == 0 || V->min > x){ // Empty vEB + check min
            return -1;
        }
        if(V->size <= 2){ // Base Case vEB
            if(x >= V->max){
                return V->max;
            }
            return V->min;
        }
        if(V->max <= x){ // x is largest in current vEB
            return V->max;
        }
        if(V->min == x){
            return x;
        }
        ll high = (x>>(V->u>>1));
        ll low = x-(high<<(V->u>>1));
        if(V->cluster[high]->size != 0 && V->cluster[high]->min <= low){ // pred in same cluster
            return (high<<(V->u>>1)) + predecessor(V->cluster[high], low);
        } // check prev cluster
        ll nextHigh = predecessor(V->summary, high-1);
        if(nextHigh == -1){
            return V->min;
        } else{
            return (nextHigh<<(V->u>>1))+V->cluster[nextHigh]->max;
        }
    }

    ll successor(Node *V, ll x) const{
        if(V->size == 0 || V->max < x){ // Empty vEB + check min
            return -1;
        }
        if(V->size <= 2){ // Base Case vEB
            if(x <= V->min){
                return V->min;
            }
            return V->max;
        }
        if(V->min >= x){ // x is smallest in current vEB
            return V->min;
        }
        if(V->max == x){
            return x;
        }
        ll high = (x>>(V->u>>1));
        ll low = x-(high<<(V->u>>1));
        if(V->cluster[high]->max >= low){ // succ in same cluster
            return (high<<(V->u>>1)) + successor(V->cluster[high], low);
        } // check succ cluster
        ll nextHigh = successor(V->summary, high+1);
        if(nextHigh != -1){
            return (nextHigh<<(V->u>>1))+V->cluster[nextHigh]->min;
        } else{
            return V->max;
        }
    }
    
    bool member(Node *V, ll x) const{
        if(x == V->min || x == V->max)
            return true;
        if(V->u == 1){ // Base Case vEB:2^u=2(has only min & max, which is the upper if statement)
            return false;
        }
        ll high = (x>>(V->u>>1)); // cluster position
        ll low = x-(high<<(V->u>>1)); // cluster element
        return member(V->cluster[high], low); // recursive search with shrink factor u/2 (sqrt(U))
    }

    void build(Node *&V, int u){
        V = new Node(u);
        if(u == 1){ // Base case vEB
            return;
        }
        build(V->summary, (u+1)>>1);
        ll SIZE = (ll)V->cluster.size(); // cluster size can be sqrt(ll_max) which is ll
        for(ll i = 0; i < SIZE; ++i){
            build(V->cluster[i], u>>1);
        }
    }

    void deleteNodes(Node *V){
        if(V->u == 1){
            return delete V;
        }
        for(Node *next : V->cluster){ // next = traversal node
            deleteNodes(next); // recur
        }
        deleteNodes(V->summary);
        delete V;
    }
    
public:

    VanEmdeBoasTree(ll _U) : U(_U){ // Constructor of vEB Tree
        logU = log2(U);
        if((1LL << logU) != U){ // if U is not exact power of 2
            ++logU; // get next power of 2
            U = (1LL << logU); // actualize U properly
        }
        build(root, logU);
    }

    ~VanEmdeBoasTree(){ // Destructor (recursive)
        deleteNodes(root);
    }

    void insert(ll x){
        insert(root, x);
    }

    void erase(ll x){
        erase(root, x);
    }

    bool member(ll x) const{
        return member(root, x);
    }

    ll size() const{
        return root->size;
    }

    ll predecessor(ll x) const{
        return predecessor(root, x);
    }
    
    ll successor(ll x) const{
        return successor(root, x);
    }

};

void checkPredecessor(const VanEmdeBoasTree &vEB, const set<ll>&S, ll U){
    for(ll i = 0; i < U; ++i){ // check predecessor function
        ll vEBpred = vEB.predecessor(i); // immediate next smaller or equal to j element (<=)
        auto setUpper = S.upper_bound(i); // immediate next element which is just greater than j
        if(setUpper == S.begin()){ // the searched element is smaller than the minimal el. in the ordered tree
            assert(vEBpred == -1); // then this searched element will not have predecessor in the vEB tree
            continue;
        }
        if(setUpper == S.end()){ // the seached element is greater than the maximal el. in the ordered tree
            assert(vEBpred == *S.rbegin()); // then the pred. of the s. el. will be the max in the o. t.
            continue;
        } // the searched element x satisfies: min < x < max
        --setUpper; // upper - 1 is the vEB's predecessor element
        assert(vEBpred == *setUpper);
    }
}

void checkSuccessor(const VanEmdeBoasTree &vEB, const set<ll>&S, ll U){
    for(ll i = 0; i < U; ++i){ // check predecessor function
        ll vEBsucc = vEB.successor(i); // immediate next greater or equal to j element (>=)
        auto setLower = S.lower_bound(i); // first element not less than j
        if(setLower == S.end()){ // if searched element is greater than max in ordered tree
            assert(vEBsucc == -1);
            continue;
        }
        assert(vEBsucc==*setLower);
    }
}

void testVanEmdeBoasTree(){
    ll U = 1LL << 12; // Univese size 4096
    VanEmdeBoasTree vEB(U);
    set<ll> S;
    int n = 1<<11; // <= 2048 elements (without repetitions)
    srand((int)time(nullptr));
    for(int i = 0; i < n; ++i){
        ll x = rand() % U;
        S.insert(x);
        vEB.insert(x);
    }
    assert(vEB.size()==S.size()); // check equal sizes of data structures
    for(int i = 0; i < U; ++i){ // check member function
        assert(vEB.member(i) == S.count(i));
    }
    checkPredecessor(vEB, S, U);
    checkSuccessor(vEB, S, U);
    for(int i = 0; i < n; ++i){ // check erase function (erase random 1024 elements)
        ll x = rand() % U;
        S.erase(x);
        vEB.erase(x);
        assert(S.size() == vEB.size());
        for(int j = 0; j < U; ++j){
            assert(vEB.member(j) == S.count(j));
        }
        checkPredecessor(vEB, S, U);
        checkSuccessor(vEB, S, U);
    }
}

int main(){
    return testVanEmdeBoasTree(), 0;
}
