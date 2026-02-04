/**
 * Author: snowysecret
 * Date: Unknown
 * License: Unknown
 * Source: snowysecret
 * Description: segment tree beats, supports
 * range chmin, chmax and add operations, and sum 
 * queries. Can be easily augmented to support min and
 * max queries as well.
 * Status: tested on LibChecker.
 */
struct SegtreeBeats {
  enum Type { CHMIN, CHMAX, ADD };
  int n;
  static const ll INF = 2e18;
  struct Node {
    ll mx1, mx2, mxc, mn1, mn2, mnc, sum, lazy;
    int l, r;
  }; 
  vector<Node> st;
  void apply_chmin(Node &ts, int v) {
    if (ts.mx1 <= v) return;
    ts.sum += (v - ts.mx1) * ts.mxc;
    if (ts.mn1 == ts.mx1) ts.mn1 = v;
    else if (ts.mn2 == ts.mx1) ts.mn2 = v;
    ts.mx1 = v;
  }
  void apply_chmax(Node &ts, int v) {
    if (ts.mn1 >= v) return;
    ts.sum += (v - ts.mn1) * ts.mnc;
    if (ts.mx1 == ts.mn1) ts.mx1 = v;
    else if (ts.mx2 == ts.mn1) ts.mx2 = v;
    ts.mn1 = v;
  }
  void apply_add(Node &ts, int v) {
    ts.mx1 += v; 
    if (ts.mx2 != -INF) ts.mx2 += v;
    ts.mn1 += v; 
    if (ts.mn2 != INF) ts.mn2 += v;
    ts.sum += v * (ts.r - ts.l + 1), ts.lazy += v;
  }
  void push_down(int idx) {
    int ch = (idx<<1);
    while (++ch <= (idx<<1) + 2) {
      if (st[idx].lazy) apply_add(st[ch], st[idx].lazy);
      apply_chmin(st[ch], st[idx].mx1);
      apply_chmax(st[ch], st[idx].mn1);
    }
    st[idx].lazy = 0;
  }
  Node push_up(const Node& a, const Node& b) {
    Node res;
    res.mx1 = max(a.mx1, b.mx1);
    if (a.mx1 == b.mx1) res.mx2 = max(a.mx2, b.mx2), res.mxc = a.mxc + b.mxc;
    else if (a.mx1 > b.mx1) res.mx2 = max(a.mx2, b.mx1), res.mxc = a.mxc;
    else res.mx2 = max(a.mx1, b.mx2), res.mxc = b.mxc;
    res.mn1 = min(a.mn1, b.mn1);
    if (a.mn1 == b.mn1) res.mn2 = min(a.mn2, b.mn2), res.mnc = a.mnc + b.mnc;
    else if (a.mn1 < b.mn1) res.mn2 = min(a.mn2, b.mn1), res.mnc = a.mnc;
    else res.mn2 = min(a.mn1, b.mn2), res.mnc = b.mnc;
    res.sum = a.sum + b.sum; res.lazy = 0;
    res.l = a.l; res.r = b.r;
    return res;
  }
  void upd(int l, int r, int constl, int constr, int idx, int v, Type type) {
    if (type == CHMIN && st[idx].mx1 <= v) return;
    if (type == CHMAX && st[idx].mn1 >= v) return;
    if (l <= constl && constr <= r) {
      if (type == CHMIN && st[idx].mx2 < v) return apply_chmin(st[idx], v);
      if (type == CHMAX && st[idx].mn2 > v) return apply_chmax(st[idx], v);
      if (type == ADD) return apply_add(st[idx], v);
    }
    push_down(idx);
    int mid = (constl + constr) >> 1;
    if (mid < l || r < constl) upd(l, r, mid + 1, constr, (idx << 1) + 2, v, type);
    else if (constr < l || r < mid + 1) upd(l, r, constl, mid, (idx << 1) + 1, v, type);
    else {
      upd(l, r, constl, mid, (idx << 1) + 1, v, type);
      upd(l, r, mid + 1, constr, (idx << 1) + 2, v, type);
    }
    st[idx] = push_up(st[(idx << 1) + 1], st[(idx << 1) + 2]);
  }
  int qu(int l, int r, int constl, int constr, int idx) {
    if (l <= constl && constr <= r) return st[idx].sum;
    push_down(idx);
    int mid = (constl + constr) >> 1;
    if (mid < l || r < constl) return qu(l, r, mid + 1, constr, (idx << 1) + 2);
    else if (constr < l || r < mid + 1) return qu(l, r, constl, mid, (idx << 1) + 1);
    else return qu(l, r, constl, mid, (idx << 1) + 1) + qu(l, r, mid + 1, constr, (idx << 1) + 2);
  }
  public:
  void init(int idx, int l, int r) {
    st[idx] = {0, -INF, r - l + 1, 0, INF, r - l + 1, 0, 0, l, r};
    if (l == r) return;
    int mid = (l + r) >> 1;
    init((idx << 1) + 1, l, mid);
    init((idx << 1) + 2, mid + 1, r);
  }
  SegtreeBeats(int n) : n(n), st(4 * n + 20) { init(0, 0, n); }
  void range_chmin(int l, int r, int v) { upd(l, r, 0, n, 0, v, CHMIN); }
  void range_chmax(int l, int r, int v) { upd(l, r, 0, n, 0, v, CHMAX); }
  void range_add(int l, int r, int v) { upd(l, r, 0, n, 0, v, ADD); }
  int query_sum(int l, int r) { return qu(l, r, 0, n, 0); }
};