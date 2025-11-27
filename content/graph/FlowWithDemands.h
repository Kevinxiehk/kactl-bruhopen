/**
 * Author: snowysecret, adapted from KACTL
 * Date: Unknown
 * Source: snowysecret
 * Description: addEdge(u, v, l, r) adds a directed edge from node u to node v with demand l and
 * capacity r. It is also possible to add edges normally using addEdge(u, v, c).
 * Supersource node is node n-2, supersource node is node n-1 as seen inside the template,
 * and nodes n and n+1 respectively seen outside the template. calcMax() returns either the 
 * maximum flow satisfying all constraints from s to t, or -1 if a valid flow cannot be found.
 * To find a minimum flow satisfying all constraints, binary search on the parameter X passed to
 * calcMax(s, t, X) to find the smallest X such that calcMax(s, t, X) is not -1.
 * Status: Tested maxflow on HKOJ A422, minflow not tested.
 */

struct FlowWithDemands {
	struct Edge {
		int to, rev;
		ll c, oc;
		ll flow() { return max(oc - c, 0LL); } // if you need flows
	};
	vi lvl, ptr, q;
	vector<vector<Edge>> adj, adjcpy;
    int n;
    ll sumDemands = 0;
        FlowWithDemands(int n_) {
        n_ += 2;
        n = n_;
        lvl.resize(n);
        ptr.resize(n); 
        q.resize(n);
        adj.resize(n);
        adjcpy.resize(n);
    }
	void addEdge_(int a, int b, ll c, ll rcap = 0) {
		adj[a].push_back({b, sz(adj[b]), c, c});
		adj[b].push_back({a, sz(adj[a]) - 1, rcap, rcap});
        adjcpy[a].push_back({b, sz(adj[b]), c, c});
		adjcpy[b].push_back({a, sz(adj[a]) - 1, rcap, rcap});
	}
    void addEdge(int a, int b, ll lc, ll rc, ll rcap = 0) {
		addEdge_(a, b, rc - lc);
        addEdge_(n-2, b, lc);
        addEdge_(a, n-1, lc);
        sumDemands += lc;
	}
    void reset() { 
		// Resets entire adjacency list to state before any flow calculations
        adj = adjcpy;
    }
	ll dfs(int v, int t, ll f) {
		if (v == t || !f) return f;
		for (int& i = ptr[v]; i < sz(adj[v]); i++) {
			Edge& e = adj[v][i];
			if (lvl[e.to] == lvl[v] + 1)
				if (ll p = dfs(e.to, t, min(f, e.c))) {
					e.c -= p, adj[e.to][e.rev].c += p;
					return p;
				}
		}
		return 0;
	}
	ll calc_(int s, int t) {
		ll flow = 0; q[0] = s;
		rep(L,0,31) do { // 'int L=30' maybe faster for random data
			lvl = ptr = vi(sz(q));
			int qi = 0, qe = lvl[s] = 1;
			while (qi < qe && !lvl[t]) {
				int v = q[qi++];
				for (Edge e : adj[v])
					if (!lvl[e.to] && e.c >> (30 - L))
						q[qe++] = e.to, lvl[e.to] = lvl[v] + 1;
			}
			while (ll p = dfs(s, t, LLONG_MAX)) flow += p;
		} while (lvl[t]);
		return flow;
	}
    ll calcMax(int s, int t, ll X = 1e15) { 
		// finds MAXIMUM flow satisfying the constraints
        addEdge(t, s, 0, X);
        ll fi = calc_(n-2, n-1);
        adj[t].pop_back();
        adj[s].pop_back();
        if (fi != sumDemands) return -1;
        return calc_(s, t);
    }
	bool leftOfMinCut(int a) { return lvl[a] != 0; }
};