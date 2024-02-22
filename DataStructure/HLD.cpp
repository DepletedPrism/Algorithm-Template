// LOJ #139
#include <bits/stdc++.h>
using namespace std;

using LL = long long;

template<typename T>
struct BIT {
  int n;
  vector<T> c1, c2;
  BIT(int _n): n(_n), c1(n + 1), c2(n + 1) {}

  static int lowbit(int x) {
    return x & -x;
  }

  void mdy(int p, T v) {
    for (int i = p; i <= n; i += lowbit(i))
      c1[i] += v, c2[i] += p * v;
  }
  void mdy(int l, int r, T v) {
    mdy(l, v);
    mdy(r + 1, -v);
  }

  LL qry(int p) {
    LL ret = 0;
    for (int i = p; i; i -= lowbit(i))
      ret += (p + 1) * c1[i] - c2[i];
    return ret;
  }
  LL qry(int l, int r) {
    return qry(r) - qry(l - 1);
  }
};

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  int n, q;
  cin >> n;
  vector<LL> a(n + 1);
  vector<int> pre(n + 1);
  for (int i = 1; i <= n; ++i)
    cin >> a[i];
  vector<vector<int>> G(n + 1);
  for (int u = 2; u <= n; ++u) {
    cin >> pre[u];
    G[pre[u]].push_back(u);
  }

  int clk = 0;
  vector<int> depth(n + 1), size(n + 1), son(n + 1), topfa(n + 1);
  vector<int> dfn(n + 1), rnk(n + 1);

  // it's not necessary to maintain pre[u] in this problem,
  // but this is not the case for all problems
  auto dfs0 = [&](auto self, int u, int fa) -> void {
    pre[u] = fa, son[u] = -1;
    depth[u] = depth[fa] + 1, size[u] = 1;
    for (auto v: G[u]) if (v != fa) {
      self(self, v, u);
      size[u] += size[v];
      if (son[u] == -1 || size[v] > size[son[u]])
        son[u] = v;
    }
  };
  auto dfs1 = [&](auto self, int u, int top) -> void {
    topfa[u] = top;
    rnk[dfn[u] = ++clk] = u;
    if (~son[u])
      self(self, son[u], top);
    for (auto v: G[u])
      if (v != pre[u] && v != son[u])
        self(self, v, v);
  };
  int rt = 1;
  dfs0(dfs0, rt, 0);
  dfs1(dfs1, rt, rt);
  BIT<LL> t(n);
  for (int u = 1; u <= n; ++u)
    t.mdy(u, u, a[rnk[u]]);

  auto findSon = [&](int u, int anc) {
    while (topfa[u] != topfa[anc]) {
      if (pre[topfa[u]] == anc)
        return topfa[u];
      u = pre[topfa[u]];
    }
    return son[anc];
  };

  auto mdyChn = [&](int u, int v, LL d) {
    while (topfa[u] != topfa[v]) {
      if (depth[topfa[u]] < depth[topfa[v]])
        swap(u, v);
      t.mdy(dfn[topfa[u]], dfn[u], d);
      u = pre[topfa[u]];
    }
    if (depth[u] > depth[v])
      swap(u, v);
    t.mdy(dfn[u], dfn[v], d);
  };
  auto mdySub = [&](int u, LL d) {
    if (u == rt)
      return t.mdy(1, n, d);
    if (dfn[u] <= dfn[rt] && dfn[rt] <= dfn[u] + size[u] - 1) {
      int v = findSon(rt, u);
      t.mdy(1, n, d);
      t.mdy(dfn[v], dfn[v] + size[v] - 1, -d);
    } else {
      t.mdy(dfn[u], dfn[u] + size[u] - 1, d);
    }
  };

  auto qryChn = [&](int u, int v) {
    LL ret = 0;
    while (topfa[u] != topfa[v]) {
      if (depth[topfa[u]] < depth[topfa[v]])
        swap(u, v);
      ret += t.qry(dfn[topfa[u]], dfn[u]);
      u = pre[topfa[u]];
    }
    if (depth[u] > depth[v])
      swap(u, v);
    return ret + t.qry(dfn[u], dfn[v]);
  };
  auto qrySub = [&](int u) {
    if (u == rt)
      return t.qry(1, n);
    if (dfn[u] <= dfn[rt] && dfn[rt] <= dfn[u] + size[u] - 1) {
      int v = findSon(rt, u);
      return t.qry(1, n) - t.qry(dfn[v], dfn[v] + size[v] - 1);
    }
    return t.qry(dfn[u], dfn[u] + size[u] - 1);
  };

  cin >> q;
  for (int opt, u, v, d; q; --q) {
    cin >> opt >> u;
    if (opt == 1) {
      rt = u;
    } else if (opt == 2) {
      cin >> v >> d;
      mdyChn(u, v, d);
    } else if (opt == 3) {
      cin >> d;
      mdySub(u, d);
    } else if (opt == 4) {
      cin >> v;
      cout << qryChn(u, v) << '\n';
    } else if (opt == 5) {
      cout << qrySub(u) << '\n';
    }
  }
  return 0;
}
