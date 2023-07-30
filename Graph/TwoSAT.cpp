// Luogu P4782
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

const int MAXN = 2e6 + 5;

int n, m;

namespace Graph {
  struct Edge { int nxt, to; } edges[MAXN];
  int head[MAXN], eidx;

  inline void init() { memset(head, -1, sizeof head), eidx = 1; }
  inline void AddEdge(int from, int to) {
    edges[++eidx] = (Edge){ head[from], to }, head[from] = eidx;
  }
}

namespace SCC {
  using namespace Graph;
  int low[MAXN], dfn[MAXN], clk;
  int Idx[MAXN], stk[MAXN], idx, top;

  void tarjan(int u) {
    low[u] = dfn[u] = ++clk, stk[++top] = u;
    for (int v, i = head[u]; ~i; i = edges[i].nxt) {
      if (!dfn[v = edges[i].to]) {
        tarjan(v);
        low[u] = min(low[u], low[v]);
      } else if (!Idx[v]) low[u] = min(low[u], dfn[v]);
    }
    if (dfn[u] == low[u]) {
      static int x; ++idx;
      do {
        x = stk[top--], Idx[x] = idx;
      } while (x != u);
    }
  }

  void solve() {
    clk = idx = 0;
    for (int u = 1; u <= 2 * n; ++u)
      if (!dfn[u]) tarjan(u);
  }
}

namespace TwoSAT {
  inline void AddClause(int u, int x, int v, int y) {
    Graph::AddEdge(u + n * x, v + n * (y ^ 1));
    Graph::AddEdge(v + n * y, u + n * (x ^ 1));
  }

  bool solve() {
    SCC::solve();
    for (int u = 1; u <= n; ++u)
      if (SCC::Idx[u] == SCC::Idx[u + n]) return false;
    return true;
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  Graph::init();

  read(n), read(m);
  for (int i = 1; i <= m; ++i) {
    static int u, a, v, b;
    read(u), read(a), read(v), read(b);
    TwoSAT::AddClause(u, a, v, b);
  }

  if (!TwoSAT::solve())
    puts("IMPOSSIBLE");
  else {
    puts("POSSIBLE");
    for (int i = 1; i <= n; ++i)
      printf("%d%c", SCC::Idx[i] < SCC::Idx[i + n], " \n"[i == n]);
  }
  return 0;
}
