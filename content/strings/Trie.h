/**
 * Author: Isaac Chan
 * Date: 2025-10-17
 * License: CC0
 * Source: own work
 * Description: This trie has the regular trie features but also an additional \texttt{walk id}
 *  that you can use to walk on the trie. You can treat it as a text editor;
 *  use the operations append(c) and backspace() to walk on the trie, or directly
 *  edit the \texttt{walk id} variable (dangerous).
 * Status: stress-tested
 */
struct trie_basic {
    const static int ALPHA = 94;
    int f(int x) { // function that converts character to id
        return x - 33;
    }
    struct node {
        int contains_as_prefix = 0;
        int contains_as_word = 0;
        vector<int> child = vector<int>(ALPHA, -1);
        int parent = -1;
        int len = 0;
    } emptynode;
    int walk_id = 0;
    vector<node> v = {emptynode};
    int new_node(int parent_id) { // Creates a new node and returns its id
        int id = v.size();
        v.push_back(emptynode);
        v[id].parent = parent_id;
        v[id].len = v[parent_id].len + 1;
        return id;
    }
    void insert(string str) {
        vector<int> seq;
        for (char c: str) seq.push_back(f(c));
        int cur = 0;
        v[cur].contains_as_prefix++;
        for (int x: seq) {
            if (v[cur].child[x] == -1) {
                v[cur].child[x] = new_node(cur);
            }
            cur = v[cur].child[x];
            v[cur].contains_as_prefix++;
        }
        v[cur].contains_as_word++;
    }
    void erase(string str) {
        vector<int> seq;
        for (char c: str) seq.push_back(f(c));
        int cur = 0;
        v[cur].contains_as_prefix--;
        for (int x: seq) {
            if (v[cur].child[x] == -1) {
                v[cur].child[x] = new_node(cur);
            }
            cur = v[cur].child[x];
            v[cur].contains_as_prefix--;
        }
        v[cur].contains_as_word--;
    }
    void append(char c) {
        if (v[walk_id].child[f(c)] == -1) {
            v[walk_id].child[f(c)] = new_node(walk_id);
        }
        walk_id = v[walk_id].child[f(c)];
    }
    bool backspace() {
        if (walk_id == 0) return 0;
        walk_id = v[walk_id].parent;
        return 1;
    }
    int query_asPrefix(string str) { // str is a prefix of how many strings in the trie?
        int save = walk_id;
        walk_id = 0;
        for (char c: str) append(c);
        int ans = v[walk_id].contains_as_prefix;
        walk_id = save;
        return ans;
    }
    int query_asSuffix(string str) { // str is a suffix of how many strings in the trie?
        int save = walk_id;
        walk_id = 0;
        int ans = v[walk_id].contains_as_word;
        for (char c: str) {
            append(c);
            ans += v[walk_id].contains_as_word;
        }
        walk_id = save;
        return ans;
    }
    int query_count(string str) {
        int save = walk_id;
        walk_id = 0;
        for (char c: str) append(c);
        int ans = v[walk_id].contains_as_word;
        walk_id = save;
        return ans;
    }
};