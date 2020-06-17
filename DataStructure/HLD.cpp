// LOJ #139
// DeP
#include <cctype>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

namespace IO {
  const int MAXSIZE = 1 << 18 | 1;
  char buf[MAXSIZE], *p1, *p2;

  inline int Gc() {
    return p1 == p2 &&
      (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++;
  }
  template<typename T> inline void read(T& x) {
    x = 0; int f = 0, ch = Gc();
    while (!isdigit(ch)) f |= ch == '-', ch = Gc();
    while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
    if (f) x = -x;
  }
}
using IO::read;

typedef long long LL;
const int MAXN = 1e5 + 5;

int n, q;
int A[MAXN];

int rt;
int pre[MAXN], dfn[MAXN], rnk[MAXN], clk;

namespace Graph {
  struct Edge { int nxt, to; } edges[MAXN];
  int head[MAXN], eidx;

  inline void init() { memset(head, -1, sizeof head), eidx = 1; }
  inline void AddEdge(int from, int to) {
    edges[++eidx] = (Edge){ head[from], to }, head[from] = eidx;
  }
}

namespace BIT {
  LL C1[MAXN], C2[MAXN];

  inline int lowbit(int x) { return x & -x; }

  inline void Mdy(int p, const LL& v) {
    for (int i = p; i <= n; i += lowbit(i))
      C1[i] += v, C2[i] += p * v;
  }
  inline void Mdy(int L, int R, const LL& v) { Mdy(L, v), Mdy(R + 1, -v); }

  inline LL Qry(int p) {
    LL ret = 0;
    for (int i = p; i; i -= lowbit(i))
      ret += (p + 1) * C1[i] - C2[i];
    return ret;
  }
  inline LL Qry(int L, int R) { return Qry(R) - Qry(L - 1); }
}

namespace HLD {
  using namespace Graph;
  int depth[MAXN], size[MAXN], son[MAXN], topfa[MAXN];

  void dfs1(int u, int fa) {
    depth[u] = depth[fa] + 1, son[u] = -1, size[u] = 1;
    for (int v, i = head[u]; ~i; i = edges[i].nxt) {
      dfs1(v = edges[i].to, u), size[u] += size[v];
      if (son[u] == -1 || size[v] > size[son[u]]) son[u] = v;
    }
  }
  void dfs2(int u, int top) {
    topfa[u] = top, rnk[dfn[u] = ++clk] = u;
    if (~son[u])
      dfs2(son[u], top);
    for (int v, i = head[u]; ~i; i = edges[i].nxt)
      if ((v = edges[i].to) != son[u]) dfs2(v, v);
  }

  inline void solve(int u = 1) { dfs1(u, 0), dfs2(u, u); }

  int Fndson(int u, const int& v) {
    while (topfa[u] != topfa[v]) {
      if (pre[topfa[u]] == v) return topfa[u];
      u = pre[topfa[u]];
    }
    return son[v];
  }

  void MdyChn(int u, int v, const LL& d) {
    while (topfa[u] != topfa[v]) {
      if (depth[topfa[u]] < depth[topfa[v]]) swap(u, v);
      BIT::Mdy(dfn[topfa[u]], dfn[u], d);
      u = pre[topfa[u]];
    }
    if (depth[u] > depth[v]) swap(u, v);
    BIT::Mdy(dfn[u], dfn[v], d);
  }
  LL QryChn(int u, int v) {
    LL ret = 0;
    while (topfa[u] != topfa[v]) {
      if (depth[topfa[u]] < depth[topfa[v]]) swap(u, v);
      ret += BIT::Qry(dfn[topfa[u]], dfn[u]);
      u = pre[topfa[u]];
    }
    if (depth[u] > depth[v]) swap(u, v);
    return ret + BIT::Qry(dfn[u], dfn[v]);
  }

  void MdySub(int u, const LL& d) {
    if (u == rt)
      return BIT::Mdy(1, n, d);
    if (dfn[u] <= dfn[rt] && dfn[rt] <= dfn[u] + size[u] - 1) {
      int t = Fndson(rt, u);
      return BIT::Mdy(1, n, d), BIT::Mdy(dfn[t], dfn[t] + size[t] - 1, -d);
    }
    BIT::Mdy(dfn[u], dfn[u] + size[u] - 1, d);
  }
  LL QrySub(int u) {
    if (u == rt)
      return BIT::Qry(1, n);
    if (dfn[u] <= dfn[rt] && dfn[rt] <= dfn[u] + size[u] - 1) {
      int t = Fndson(rt, u);
      return BIT::Qry(1, n) - BIT::Qry(dfn[t], dfn[t] + size[t] - 1);
    }
    return BIT::Qry(dfn[u], dfn[u] + size[u] - 1);
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  Graph::init();

  read(n);
  for (int i = 1; i <= n; ++i) read(A[i]);
  for (int i = 2; i <= n; ++i) read(pre[i]);

  for (int i = 2; i <= n; ++i)
    Graph::AddEdge(pre[i], i);
  HLD::solve(rt = 1);
  for (int i = 1; i <= n; ++i)
    BIT::Mdy(i, i, A[rnk[i]]);

  read(q);
  for (int opt, u, v; q; --q) {
    static LL d; read(opt), read(u);
    switch (opt) {
      case 1: rt = u; break;
      case 2: read(v), read(d), HLD::MdyChn(u, v, d); break;
      case 3: read(d), HLD::MdySub(u, d); break;
      case 4: read(v), printf("%lld\n", HLD::QryChn(u, v)); break;
      case 5: printf("%lld\n", HLD::QrySub(u)); break;
      default: fprintf(stderr, "ERR\n");
    }
  }
  return 0;
}
