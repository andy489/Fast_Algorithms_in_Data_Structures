/*

The idea of using Treap data structure for storing the transitions
in every Node of the automaton is thanks to Iliyan Yordanov Yordanov

The construction time complexity of the automaton is guaranteed O(n*log(Sigma)),
where Sigma is the size of the alphabet and if we treat Sigma as a constant, then
the overall time complexity will be O(n)

*/

#include <unistd.h> // F_OK, R_OK macros for the access function
#include <fstream> // For input stream class to operate on files
#include <vector>

#pragma GCC optimize ("O3")

#define ASCII 256

using namespace std;

struct Node {
    char key, prior;
    int state;
    int l, r;

    void set(char key, int state) {
        this->key = key;
        this->prior = rand() % ASCII;
        this->state = state;
        this->l = this->r = -1;
    }
};

int num_nodes;

struct Treap {
private:
    vector<Node> nodes;

    inline void split(int t, int &l, int &r, char key) {
        if (t == -1) {
            return void(l = r = -1);
        }

        if (nodes[t].key < key) {
            split(nodes[t].r, nodes[t].r, r, key), l = t;
        } else {
            split(nodes[t].l, l, nodes[t].l, key), r = t;
        }
    }

    inline void merge(int &t, int l, int r) {
        if (l == -1 || r == -1) {
            return void(t = (l == -1 ? r : l));
        }

        if (nodes[l].prior < nodes[r].prior) {
            merge(nodes[r].l, l, nodes[r].l), t = r;
        } else {
            merge(nodes[l].r, nodes[l].r, r), t = l;
        }
    }

public:
    inline void insert(int &t, char key, int state) {
        int l, m, r;

        split(t, l, r, key);

        m = num_nodes++;
        nodes[m].set(key, state);

        merge(t, l, m);
        merge(t, t, r);
    }

    inline void copy(int &cur, int &other) {
        if (other != -1) {
            cur = num_nodes++;
            nodes[cur] = nodes[other];

            if (nodes[other].l != -1) {
                copy(nodes[cur].l, nodes[other].l);
            }

            if (nodes[other].r != -1) {
                copy(nodes[cur].r, nodes[other].r);
            }
        }
    }

    inline int find(int t, char key) {
        if (t == -1) {
            return -1;
        }
        if (nodes[t].key == key) {
            return t;
        } else if (nodes[t].key < key) {
            return find(nodes[t].r, key);
        } else {
            return find(nodes[t].l, key);
        }
    }

    inline vector<Node> &get_treap_nodes() {
        return this->nodes;
    };

} T;

struct State {
    // len = state number, par = parent link, fir = index of first occurrence of the state
    int len, par, fir;

    void set(int len, int par, int fir) {
        this->len = len;
        this->par = par;
        this->fir = fir;
    }
};

vector<State> states;
vector<int> transitions;
int num_infix;

int prefix_state(0), last(0);

inline void add(char symbol) {
    int nq = ++prefix_state; // the guaranteed new prefix state
    states[nq].set(states[last].len + 1, -1, nq);
    transitions[nq] = -1;

    int curr_transition, q;
    q = last;

    while (q != -1) {   //  traverse upwards to root
        curr_transition = T.find(transitions[q], symbol);

        if (curr_transition != -1) {
            break;
        }
        T.insert(transitions[q], symbol, nq);
        q = states[q].par;
    }

    last = nq;

    if (q == -1) {
        return void(states[nq].par = 0);
    }

    int p = T.get_treap_nodes()[curr_transition].state;

    if (states[q].len + 1 == states[p].len) {
        return void(states[nq].par = p);
    }

    int clone = num_infix++;

    states[clone].set(states[q].len + 1, states[p].par, states[p].fir);
    transitions[clone] = -1;
    T.copy(transitions[clone], transitions[p]);

    while (q != -1) {
        curr_transition = T.find(transitions[q], symbol);

        if (T.get_treap_nodes()[curr_transition].state != p) {
            break;
        }

        T.get_treap_nodes()[curr_transition].state = clone;
        q = states[q].par;
    }
    states[p].par = states[nq].par = clone;
}

vector<int> prev_edge;
int cnt_edges;

void add_edge(int from, int to = cnt_edges) { // to is always equal to cnt_edges
    prev_edge[to] = transitions[from];
    transitions[from] = cnt_edges++;
}

vector<int> in, out;
int t;

void dfs(int v) {
    in[v] = t++;

    for (int to = transitions[v]; to; to = prev_edge[to]) {
        dfs(to);
    }

    out[v] = t++;
}

int cnt_squares;
vector<int> len_state;

void find_squares(int ver) {
    len_state[states[ver].len] = ver;

    int half = len_state[states[ver].len >> 1];

    if ((half != -1) && ((states[half].len << 1) == states[ver].len)) {
        int ind = states[ver].fir - (states[ver].len >> 1);

        if ((in[half] <= in[ind]) && (in[ind] <= out[half])) {
            ++cnt_squares;
        }
    }

    for (int to = transitions[ver]; to; to = prev_edge[to]) {
        find_squares(to);
    }

    len_state[states[ver].len] = -1;
}

void init(int n) {
    states.resize(2 * n);
    T.get_treap_nodes().resize(3 * n);
    num_nodes = 0;

    transitions.resize(2 * n);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Invalid executable file call! Usage: %s <file.txt>\n", argv[0]);
        
        return 1;
    }

    char *file_path = argv[1];

    if (access(file_path, F_OK) < 0 || access(file_path, R_OK) < 0) {
        printf("Provided file does not exist or is not readable.\n");
        return 2;
    }

    ifstream file(file_path, ios::binary); // treat as a binary file
    file.seekg(-1, ios::end); // seek cursor to end of file with offset -1

    int n = file.tellg(); // get length of file

    file.seekg(0, ios::beg); // seek back to beginning

    init(n);

    states[0].set(0, -1, 0);
    transitions[0] = -1;
    num_infix = n + 1;
    last = 0;

    char c;

    // check if is alpha also, because we do not know whether it will have ending terminating zero
    while (file.get(c) && isalpha(c)) {
        add(c);
    }

    file.close();

    int cnt_final = 0;
    int curr = last;

    while (curr != -1) {
        ++cnt_final;
        curr = states[curr].par;
    }

    prev_edge.resize(num_infix);

    for (int i = 0; i < num_infix; ++i) {
        transitions[i] = 0;
    }

    cnt_edges = 1;

    for (int i = 1; i < num_infix; ++i) {
        add_edge(states[i].par, i);
    }

    in.resize(num_infix);
    out.resize(num_infix);

    t = 1;
    dfs(0);

    len_state.resize(n + 1);

    for (int i = 0; i < n; ++i) {
        len_state[i] = -1;
    }

    cnt_squares = 0;
    find_squares(0);

    printf("%d %d %d %d\n", num_infix, num_nodes, cnt_final, cnt_squares);

    return 0;
}
