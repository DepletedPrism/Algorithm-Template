// Luogu P2595
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
const int MAXN = 2.5e5 + 5, MAXM = 5e5 + 5;
const int INF = 0x3f3f3f3f;

int n, q;

LL f[MAXN];
bool vis[MAXN];
int Mn[MAXN], dfn[MAXN], clk;

namespace Graph {
  struct Edge { int nxt, to, w; } edges[MAXM << 1];
  int head[MAXN], eidx;

  inline void init() { memset(head, -1, sizeof head), eidx = 1; }
  inline void AddEdge(int from, int to, int w = 0) {
    edges[++eidx] = (Edge){ head[from], to, w }, head[from] = eidx;
  }

  LL dfs(int u) {
    f[u] = 0;
    for (int v, i = head[u]; ~i; i = edges[i].nxt) {
      LL fv = dfs(v = edges[i].to);
      f[u] += vis[v]? Mn[v]: min(fv, (LL) Mn[v]);
    }
    return f[u];
  }
}

namespace HLD {
  using namespace Graph;
  int depth[MAXN], size[MAXN], son[MAXN], pre[MAXN], topfa[MAXN];

  void dfs1(int u, int fa) {
    depth[u] = depth[fa] + 1;
    pre[u] = fa, son[u] = -1, size[u] = 1;
    for (int v, i = head[u]; ~i; i = edges[i].nxt) {
      if ((v = edges[i].to) == fa) continue;
      Mn[v] = min(Mn[u], edges[i].w);
      dfs1(v, u), size[u] += size[v];
      if (son[u] == -1 || size[v] > size[son[u]]) son[u] = v;
    }
  }
  void dfs2(int u, int top) {
    topfa[u] = top, dfn[u] = ++clk;
    if (~son[u])
      dfs2(son[u], top);
    for (int v, i = head[u]; ~i; i = edges[i].nxt)
      if ((v = edges[i].to) != pre[u] && v != son[u]) dfs2(v, v);
  }

  inline void solve(int rt = 1) {
    Mn[rt] = INF, dfs1(rt, 0), dfs2(rt, rt);
  }

  inline int LCA(int u, int v) {
    while (topfa[u] != topfa[v]) {
      if (depth[topfa[u]] < depth[topfa[v]]) swap(u, v);
      u = pre[topfa[u]];
    }
    return (depth[u] > depth[v])? v: u;
  }
}

int V[MAXN], nv;
int stk[MAXN], top;

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  Graph::init();

  read(n);
  for (int u, v, w, i = 1; i < n; ++i) {
    read(u), read(v), read(w);
    Graph::AddEdge(u, v, w), Graph::AddEdge(v, u, w);
  }

  HLD::solve(1);
  read(q);
  while (q--) {
    read(nv);
    for (int i = 1; i <= nv; ++i)
      read(V[i]), vis[V[i]] = true;

    sort(V + 1, V + nv + 1, [](const int& x, const int& y) {
        return dfn[x] < dfn[y];
      });
    Graph::eidx = 1, Graph::head[1] = -1, stk[top = 1] = 1;
    for (int i = 1; i <= nv; ++i) {
      int lca = HLD::LCA(V[i], stk[top]);
      if (lca != stk[top]) {
        while (top > 1 && dfn[lca] < dfn[stk[top - 1]])
          Graph::AddEdge(stk[top - 1], stk[top]), --top;
        if (top > 1 && dfn[lca] > dfn[stk[top - 1]]) {
          Graph::head[lca] = -1, Graph::AddEdge(lca, stk[top]);
          stk[top] = lca;
        } else
          Graph::AddEdge(lca, stk[top--]);
      }
      Graph::head[V[i]] = -1, stk[++top] = V[i];
    }
    for (int i = 1; i < top; ++i)
      Graph::AddEdge(stk[i], stk[i + 1]);

    printf("%lld\n", Graph::dfs(1));
    for (int i = 1; i <= nv; ++i) vis[V[i]] = false;
  }
  return 0;
}
