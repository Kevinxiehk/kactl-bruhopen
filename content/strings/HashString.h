/**
 * Author: snowysecret
 * Date: Unknown
 * Source: snowysecret
 * Description: String (multi-)hashing. init(s) if you want to use single hashing;
 * init(s, k) if you want to use k-hashing, for k at most 5.
 * Status: Tested on some Codeforces problems before.
 * Time: $O(kn)$.
 */
struct HashString {
    // all indices are ZERO-BASED
    const int fac[5] = {2017, 211, 269, 397, 691};
    const int MOD[5] = {1734232211, 1000000009, 874803863, 909739993, 1388999387};
    vector<int> pau[5], ps[5];
    int n, k_;
    void init(string s, int k = 1) {
        k_ = k;
        n = s.size();
        for (int it=0; it<k; it++) {
            pau[it].push_back(1);
            for(int i=1; i<=n; i++) {
                pau[it].push_back((pau[it][i-1] * fac[it]) % MOD[it]);
            }
            ps[it].push_back(s[0]);
            for(int i=1; i<n; i++) {
                ps[it].push_back((ps[it][i-1] * fac[it] + s[i]) % MOD[it]);
            }
        }
    }
    int eval(int l, int r) {
        int ans = 0;
        for (int i=0; i<k_; i++) {
            int val = (ps[i][r] - (l == 0 ? 0 : (ps[i][l-1] * pau[i][r-l+1]) % MOD[i]) + MOD[i]) % MOD[i];
            ans ^= (val * (i + 1));
        }
        return ans;
    }
    vector<int> fulleval(int l, int r) {
        vector<int> v;
        for (int i=0; i<k_; i++) {
            int val = (ps[i][r] - (l == 0 ? 0 : (ps[i][l-1] * pau[i][r-l+1]) % MOD[i]) + MOD[i]) % MOD[i];
            v.push_back(val);
        }
        return v;
    }
    int get_hash() {
        return eval(0, n - 1);
    }
    vector<int> get_fullhash() {
        return fulleval(0, n-1);
    }
};