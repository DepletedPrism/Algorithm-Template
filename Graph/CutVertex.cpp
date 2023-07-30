// Luogu P3388
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

const int MAXN = 2e4 + 5, MAXM = 1e5 + 5;

int n, m;
int Ans[MAXN], nA;

namespace Graph {
  struct Edge { int nxt, to; } edges[MAXM << 1];
  int head[MAXN], eidx;

  inline void init() { memset(head, -1, sizeof head), eidx = 1; }
  inline void AddEdge(int from, int to) {
    edges[++eidx] = (Edge){ head[from], to }, head[from] = eidx;
  }
}

namespace BCC {
  using namespace Graph;
  bool isCut[MAXN];
  int dfn[MAXN], low[MAXN], clk;

  void tarjan(int u, int fa) {
    int ch = 0;
    low[u] = dfn[u] = ++clk;
    for (int v, i = head[u]; ~i; i = edges[i].nxt) {
      if (!dfn[v = edges[i].to]) {
        ++ch, tarjan(v, u);
        low[u] = min(low[u], low[v]);
        if (low[v] >= dfn[u]) isCut[u] = true;
      } else if (v != fa && dfn[v] < dfn[u])
        low[u] = min(low[u], dfn[v]);
    }
    if (!fa && ch == 1) isCut[u] = false;
  }

  inline void solve() {
    clk = 0;
    for (int i = 1; i <= n; ++i)
      if (!dfn[i]) tarjan(i, 0);
    for (int i = 1; i <= n; ++i)
      if (isCut[i]) Ans[++nA] = i;
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  Graph::init();

  read(n), read(m);
  for (int u, v, i = 1; i <= m; ++i)
    read(u), read(v), Graph::AddEdge(u, v), Graph::AddEdge(v, u);

  BCC::solve();

  printf("%d\n", nA);
  for (int i = 1; i <= nA; ++i)
    printf("%d%c", Ans[i], " \n"[i == nA]);
  return 0;
}
