/**
 * Author: cp-algorithms, ChatGPT and Copilot
 * Description: polynomial class that supports
 * convolution, derivative, integral, 
 * inv, ln, exp, pow and sqrt.
 * Status: all tested on LibChecker.
 */
typedef vector<ll> vl;
struct poly : vl {
  const ll mod = (119 << 23) + 1, root = 62; // = 998244353
  // For p < 2^30 there is also e.g. 5 << 25, 7 << 26, 479 << 21
  // and 483 << 21 (same root). The last two are > 10^9.
  poly(int n) : vl(n) {}
  poly(std::initializer_list<ll> x) : vl(all(x)) {}
  poly(vl v) : vl(all(v)) {}
  poly(int n, int v) : vl(n, v) {}
  poly(const poly &b) {
    this->clear();
    for (auto x : b) this->pb(x);
  }
  private:
  ll modpow(ll b, ll e, const ll mod = 998244353) const {
    ll ans = 1;
    for (; e; b = b * b % mod, e /= 2)
      if (e & 1) ans = ans * b % mod;
    return ans;
  }
  ll modinv(ll b, const ll mod = 998244353) const {
    return modpow(b, mod - 2, mod);
  }
  void ntt(vl &a) const {
    int n = sz(a), L = 31 - __builtin_clz(n);
    static vl rt(2, 1);
    for (static int k = 2, s = 2; k < n; k *= 2, s++) {
      rt.resize(n);
      ll z[] = {1, modpow(root, mod >> s)};
      rep(i,k,2*k) rt[i] = rt[i / 2] * z[i & 1] % mod;
    }
    vi rev(n);
    rep(i,0,n) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    rep(i,0,n) if (i < rev[i]) ::swap(a[i], a[rev[i]]);
    for (int k = 1; k < n; k *= 2)
      for (int i = 0; i < n; i += 2 * k) rep(j,0,k) {
        ll z = rt[j + k] * a[i + j + k] % mod, &ai = a[i + j];
        a[i + j + k] = ai - z + (z > ai ? mod : 0);
        ai += (ai + z >= mod ? z - mod : z);
      }
  }
  poly conv(const poly &a, const poly &b) const {
    if (a.empty() || b.empty()) return poly(0);
    int s = sz(a) + sz(b) - 1, B = 32 - __builtin_clz(s),
        n = 1 << B;
    int inv = modpow(n, mod - 2);
    poly L(a), R(b), out(n);
    L.resize(n), R.resize(n);
    ntt(L), ntt(R);
    rep(i,0,n)
      out[-i & (n - 1)] = (ll)L[i] * R[i] % mod * inv % mod;
    ntt(out);
    while (sz(out) > s) out.pop_back();
    return out;
  }
  public:
  poly operator+(const poly &b) const {
    poly res(max(sz(*this), sz(b)), 0);
    rep(i,0,sz(res)) {
      ll av = (i < sz(*this) ? (*this)[i] : 0);
      ll bv = (i < sz(b) ? b[i] : 0);
      res[i] = (av + bv >= mod ? av + bv - mod : av + bv);
    }
    return res;
  }
  poly operator-(const poly &b) const {
    poly res(max(sz(*this), sz(b)), 0);
    rep(i,0,sz(res)) {
      ll av = (i < sz(*this) ? (*this)[i] : 0);
      ll bv = (i < sz(b) ? b[i] : 0);
      res[i] = (av - bv < 0 ? av - bv + mod : av - bv);
    }
    return res;
  }
  poly operator*(const poly &b) const {
    return conv(*this, b);
  }
  poly operator*(const ll &b) const {
    poly res(sz(*this), 0);
    rep(i,0,sz(res)) res[i] = ((*this)[i] * b) % mod;
    return res;
  }
  poly operator=(const poly &b) {
    this->clear();
    for (auto x : b) this->pb(x);
    return *this;
  }
  poly operator+=(const poly &b) {
    return *this = *this + b;
  }
  poly operator-=(const poly &b) {
    return *this = *this - b;
  }
  poly operator*=(const poly &b) {
    return *this = *this * b;
  } 
  poly modxk(int k) const {
    k = min(k, (int)this->size());
    return poly(vl(this->begin(), this->begin() + k));
  }
  poly deriv() const {
    if (this->empty()) return poly(0);
    poly res(max(0LL, (ll)this->size() - 1), 0);
    for (int i = 1; i < (int)this->size(); i++)
      res[i - 1] = (*this)[i] * i % mod;
    return res;
  }
  poly integ() const {
    poly res((int)this->size() + 1, 0);
    for (int i = 0; i < (int)this->size(); i++)
      res[i + 1] = (*this)[i] * modpow(i + 1, mod - 2) % mod;
    return res;
  }
  poly inv(int n) const { // requires (*this)[0] != 0
    assert(!this->empty() && (*this)[0] % mod != 0);
    poly r(vl{modpow((*this)[0], mod - 2)});
    for (int m = 1; m < n; m <<= 1) {
      poly f = this->modxk(m << 1);
      r = (r * (poly(vl{2}) - f * r)).modxk(m << 1);
    }
    return r.modxk(n);
  }
  poly ln(int n) const { // requires (*this)[0] == 1
    assert(!this->empty() && (*this)[0] % mod == 1);
    poly f = this->modxk(n);
    return (f.deriv() * f.inv(n - 1)).integ().modxk(n);
  }
  poly exp(int n) const {
    assert(this->empty() || (*this)[0] % mod == 0);
    poly g(vl{1});
    for (int m = 1; m < n; m <<= 1) {
      poly cur = this->modxk(m << 1);
      g = (g * (poly(vl{1}) - g.ln(m << 1) + cur)).modxk(m << 1);
    }
    return g.modxk(n);
  }
  // calculate this^k mod x^n
  poly pow(int k, int n) const {
    if (this->empty()) return *this;
    if (k == 0) {
      poly one(n, 0);
      one[0] = 1;
      return one;
    }
    int lead = 0;
    while (lead < (int)this->size() && (*this)[lead] == 0) lead++;
    if (lead == (int)this->size() || (__int128) lead * k >= n)
      return poly(n, 0);
    ll inv = modinv((*this)[lead]);
    poly f((int)this->size() - lead, 0);
    for (int i = 0; i < (int)f.size(); i++)
      f[i] = (*this)[i + lead] * inv % mod;
    poly res = (f.ln(n - lead * k) * (k % mod)).exp(n - lead * k);
    res = res * modpow((*this)[lead], k);
    poly ans(n, 0); 
    for (int i = 0; i < (int)res.size(); i++)
      ans[i + lead * k] = res[i];
    return ans;
  }
  poly sqrt(int n) const { // calculate f(x) where f(x)^2 = (*this) mod x^n
    if (n == 0) return poly(0);
    poly a = *this;
    a.resize(n);
    int k = 0;
    while (k < n && a[k] == 0) k++;
    if (k == n) return poly(n, 0);
    if (k & 1) { // NO SOLUTION
      assert(false && "poly::sqrt: no square root exists (odd valuation)");
    }
    if (k) {
      poly b(n - k);
      for (int i = k; i < n; i++) b[i - k] = a[i];
      poly h = b.sqrt(n - (k / 2));
      poly res(n, 0);
      for (int i = 0; i < (int)h.size() && i + k / 2 < n; i++) res[i + k / 2] = h[i];
      return res;
    }
    auto mod_sqrt = [&](ll A) -> ll {
      A %= mod;
      if (A < 0) A += mod;
      if (A == 0) return 0;
      if (modpow(A, (mod - 1) / 2) != 1) { // NO SOLUTION
        assert(false && "poly::sqrt: constant term is not a quadratic residue");
      }
      ll q = mod - 1;
      int s = 0;
      while ((q & 1) == 0) q >>= 1, s++;
      ll z = 2;
      while (modpow(z, (mod - 1) / 2) != mod - 1) z++;
      ll c = modpow(z, q);
      ll x = modpow(A, (q + 1) / 2);
      ll t = modpow(A, q);
      int m = s;
      while (t != 1) {
        int i = 1;
        ll tt = (t * t) % mod;
        while (i < m && tt != 1) {
          tt = (tt * tt) % mod;
          i++;
        }
        ll b = c;
        for (int e = 0; e < m - i - 1; e++) b = (b * b) % mod;
        x = (x * b) % mod;
        ll b2 = (b * b) % mod;
        t = (t * b2) % mod;
        c = b2;
        m = i;
      }
      return x;
    };
    ll r0 = mod_sqrt(a[0]);
    poly g(1);
    g[0] = r0;
    const ll inv2 = (mod + 1) / 2;
    for (int m = 1; m < n; m <<= 1) {
      int m2 = min(n, m << 1);
      poly a_cut = a;
      a_cut.resize(m2);
      poly invg = g.inv(m2);
      poly q = a_cut * invg;
      q.resize(m2);
      poly g_ext = g;
      g_ext.resize(m2);
      for (int i = 0; i < m2; i++) {
        q[i] = ( (g_ext[i] + q[i]) % mod ) * inv2 % mod;
      }
      g = q;
    }
    g.resize(n);
    for (int i = 0; i < n; i++) {
      g[i] = (mod - g[i]) % mod;
    }
    return g;
  }  
  friend ostream& operator<<(ostream& os, const poly& p) {
    os << "(";
    for (int i=0; i<sz(p); i++) {
      os << p[i];
      if (i+1 < sz(p)) os << "x^" << i;
      if (i+1 < sz(p)) os << " + ";
    }
    os << ")";
    return os;
  }
};