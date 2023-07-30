// Luogu P5236
// DeP
#include <cctype>
#include <cstdio>
#include <vector>
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
const int MAXN = 1e4 + 5, MAXM = 2e4 + 5;

int n, m, q, sqidx;
LL sum[MAXN << 1], dist[MAXN << 1];

struct Graph {
  struct Edge { int nxt, to; LL w; } edges[MAXM << 2];
  int head[MAXN << 1], eidx;

  Graph() { init(); }
  inline void init() { memset(head, -1, sizeof head), eidx = 1; }

  inline void AddEdge(int from, int to, LL w) {
    edges[++eidx] = (Edge){ head[from], to, w }, head[from] = eidx;
  }
} G, T;

namespace RST {
  int pre[MAXN], val[MAXN], dfn[MAXN], low[MAXN], clk;

  void circle(int u, int v, const int& w) {
    LL s = w;
    for (int i = v; i != pre[u]; i = pre[i])
      sum[i] = s, s += val[i];
    sum[++sqidx] = sum[u], sum[u] = 0;
    for (int i = v; i != pre[u]; i = pre[i]) {
      LL c = min(sum[i], sum[sqidx] - sum[i]);
      T.AddEdge(i, sqidx, c), T.AddEdge(sqidx, i, c);
    }
  }

  void tarjan(int u, int fa) {
    dfn[u] = low[u] = ++clk;
    for (int v, i = G.head[u]; ~i; i = G.edges[i].nxt) {
      if ((v = G.edges[i].to) == fa) continue;
      if (!dfn[v]) {
        pre[v] = u, val[v] = G.edges[i].w;
        tarjan(v, u);
        low[u] = min(low[u], low[v]);
      } else low[u] = min(low[u], dfn[v]);
      if (low[v] <= dfn[u]) continue;
      T.AddEdge(u, v, G.edges[i].w), T.AddEdge(v, u, G.edges[i].w);
    }
    for (int v, i = G.head[u]; ~i; i = G.edges[i].nxt)
      if (pre[v = G.edges[i].to] != u && dfn[v] > dfn[u])
        circle(u, v, G.edges[i].w);
  }

  inline void build() { sqidx = n, tarjan(1, 0); }
}

namespace HLD {
  int depth[MAXN << 1], size[MAXN << 1];
  int pre[MAXN << 1], son[MAXN << 1], topfa[MAXN << 1];

  void dfs1(int u, int fa) {
    depth[u] = depth[fa] + 1;
    size[u] = 1, son[u] = -1, pre[u] = fa;
    for (int v, i = T.head[u]; ~i; i = T.edges[i].nxt) {
      if ((v = T.edges[i].to) == fa) continue;
      dist[v] = dist[u] + T.edges[i].w;
      dfs1(v, u), size[u] += size[v];
      if (son[u] == -1 || size[v] > size[son[u]]) son[u] = v;
    }
  }
  void dfs2(int u, int top) {
    topfa[u] = top;
    if (~son[u]) dfs2(son[u], top);
    for (int v, i = T.head[u]; ~i; i = T.edges[i].nxt)
      if ((v = T.edges[i].to) != pre[u] && v != son[u]) dfs2(v, v);
  }

  inline int LCA(int u, int v) {
    while (topfa[u] != topfa[v]) {
      if (depth[topfa[u]] < depth[topfa[v]]) swap(u, v);
      u = pre[topfa[u]];
    }
    return depth[u] > depth[v]? v: u;
  }

  inline void solve(int rt = 1) { dfs1(rt, 0), dfs2(rt, rt); }

  inline int Jmp(int u, const int& v) {
    while (topfa[u] != topfa[v]) {
      if (pre[topfa[u]] == v) return topfa[u];
      u = pre[topfa[u]];
    }
    return son[v];
  }

  inline LL Qry(const int& u, const int& v) {
    int lca = LCA(u, v);
    if (lca <= n)
      return dist[u] + dist[v] - 2 * dist[lca];
    int A = Jmp(u, lca), B = Jmp(v, lca);
    LL ret = dist[u] - dist[A] + dist[v] - dist[B];
    if (sum[A] < sum[B]) swap(A, B);
    return ret + min(sum[A] - sum[B], sum[lca] + sum[B] - sum[A]);
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(m), read(q);
  for (int u, v, w, i = 1; i <= m; ++i) {
    read(u), read(v), read(w);
    G.AddEdge(u, v, w), G.AddEdge(v, u, w);
  }

  RST::build(), HLD::solve();

  while (q--) {
    static int u, v;
    read(u), read(v), printf("%lld\n", HLD::Qry(u, v));
  }
  return 0;
}
