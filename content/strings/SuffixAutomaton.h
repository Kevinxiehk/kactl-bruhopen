/**
 * Author: CP-algorithms
 * Date: Unknown
 * License: Unknown
 * Source: CP-algorithms
 * Description: Suffix Automaton data structure.
 * Time: $O(n log k)$, where k = size of alphabet.
 * Status: stress-tested
 */
struct SuffixAutomaton {
	struct state {
		int len, link;
		map<char, int> next;
	};
	vector<state> st;
	int n, sz = 0, last = 0; // Use sz for number of characters added
	SuffixAutomaton(int n) {
		this->n = n;
		st.resize(2 * n + 6);
		st[0].len = 0;
		st[0].link = -1;
		sz++;
		last = 0;
	}
	void extend(char c) {
		int cur = sz++;
		st[cur].len = st[last].len + 1;
		int p = last;
		while (p != -1 && !st[p].next.count(c)) {
			st[p].next[c] = cur;
			p = st[p].link;
		}
		if (p == -1) {
			st[cur].link = 0;
		} else {
			int q = st[p].next[c];
			if (st[p].len + 1 == st[q].len) {
				st[cur].link = q;
			} else {
				int clone = sz++;
				st[clone].len = st[p].len + 1;
				st[clone].next = st[q].next;
				st[clone].link = st[q].link;
				while (p != -1 && st[p].next[c] == q) {
					st[p].next[c] = clone;
					p = st[p].link;
				}
				st[q].link = st[cur].link = clone;
			}
		}
		last = cur;
	}
	SuffixAutomaton(string s) {
		*this = SuffixAutomaton(sz(s));
		for (char c: s) extend(c);
	}
};