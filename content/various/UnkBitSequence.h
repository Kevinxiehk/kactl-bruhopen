/**
 * Author: snowysecret
 * Date: Unknown
 * Source: snowysecret
 * Description: There is an unknown sequence of $n$ bits
 * In one query you can specify a subset, the return value will be the sum of this subset
 * Deterministically finds the sequence using around $2.67n / \log_2(n)$ queries
 * Status: Tested on Codeforces 1705F (unintended solution).
 * Time: $O(n^2 / \log(n))$.
 */

struct UnkBitSequence { 
    // O(N / log(N)) sum queries -> N bits
    // https://codeforces.com/blog/entry/105188
    int qcnt = 0;
    int M; // N in the problem
    vector<int> filtered(vector<int> vt) { // don't include elements > N in the query!
        vector<int> res;
        for(int x: vt) {
            if(x < M) res.push_back(x);
        }
        return res;
    }
    int query_sum(vector<int> vt) {
        vt = filtered(vt);
        if(vt.empty()) return 0;
        qcnt++;
        // TO EDIT: Ask a query that is equivalent to asking sum
        
    }
    int query_count() {
        return qcnt;
    }
    vector<int> get_range(int l, int r) {
        vector<int> ww;
        for(int i=l; i<=r; i++) ww.push_back(i);
        return ww;
    }
    vector<int> set_union(vector<int> a, vector<int> b) {
        vector<int> c;
        set<int> s;
        for(int x: a) s.insert(x);
        for(int x: b) s.insert(x);
        for(int x: s) c.push_back(x);
        return c;
    }
    vector<int> set_exclude(vector<int> a, vector<int> b) { // a but not b
        set<int> s;
        for(int x: a) s.insert(x);
        for(int x: b) if(s.count(x)) s.erase(x);
        vector<int> c;
        for(int x: s) c.push_back(x);
        return c;
    }
    unordered_map<int, vector<vector<int>>> mp[21];
    vector<vector<int>> queries(int K, int L, int R) {
        if(K == 0) {
            return {{L}};
        }
        int ff = ((R-L+1) - (1<<(K-1)) + 1) / 2;
        vector<vector<int>> ql = queries(K-1, L, L+ff-1);
        vector<vector<int>> qr = queries(K-1, L+ff, L+2*ff-1);
        vector<vector<int>> new_queries;
        new_queries.push_back(get_range(L+ff, L+2*ff-1));
        for(int i=0; i+1<ql.size(); i++) {
            new_queries.push_back(set_union(ql[i], qr[i]));
            new_queries.push_back(set_union(set_union(ql[i], set_exclude(get_range(L+ff, L+2*ff-1), qr[i])), {L+2*ff+i}));
        }
        new_queries.push_back(get_range(L, R));
        mp[K][L] = new_queries;
        return new_queries;
    }
    vector<bool> answer;
    void recur(int K, int L, int R, vector<int> qres) {
        assert(qres.size() == (1ll << K));
        if(K == 0) {
            answer[L] = qres[0];
            return;
        }
        vector<vector<int>> query_list = mp[K][L]; // 2^K queries
        int ff = ((R-L+1) - (1<<(K-1)) + 1) / 2;
        // [L, L+ff)
        // [L+ff, L+2*ff)
        // [L+2*ff, R]
        int rem = 0;
        vector<int> lans, rans;
        for(int i=1; i<=(1ll<<(K-1))-1; i++) {
            // queries 2*i-1, 2*i
            lans.push_back((qres[2*i-1] + qres[2*i] - qres[0]) / 2);
            rans.push_back((qres[2*i-1] - qres[2*i] + qres[0] + 1) / 2);
            answer[L+2*ff+i-1] = (qres[2*i-1] + qres[2*i] - qres[0]) % 2;
            rem += answer[L+2*ff+i-1];
        }
        lans.push_back(qres[(1ll << K)-1] - rem - qres[0]);
        rans.push_back(qres[0]);
        recur(K-1, L, L+ff-1, lans);
        recur(K-1, L+ff, L+2*ff-1, rans);
    }
    public:
    vector<bool> find_bit_sequence(int N) {
        M = N;
        int f[N+10];
        f[0] = 1;
        int K = 0;
        for(int i=1; i<N; i++) {
            f[i] = (f[i-1] << 1) + (1ll << (i-1)) - 1;
            if(f[i] >= N) {
                N = f[i];
                K = i;
                break;
            }
        }
        answer.resize(N);
        for(int i=0; i<N; i++) answer[i] = false;
        vector<vector<int>> qq = queries(K, 0, N - 1);
        vector<int> values;
        for(vector<int> x: qq) {
            values.push_back(query_sum(x));
        }
        recur(K, 0, N - 1, values);
        vector<bool> ret(M);
        for(int i=0; i<M; i++) ret[i] = answer[i];
        return ret;
    }
  
};