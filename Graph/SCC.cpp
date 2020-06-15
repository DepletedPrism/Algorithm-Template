// Luogu P3387
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

const int MAXN = 1e4 + 5, MAXM = 1e5 + 5;
const int INF = 0x3f3f3f3f;

int n, m;
int A[MAXN];

int deg[MAXN], W[MAXN], f[MAXN];

struct Graph {
  struct Edge { int nxt, to; } edges[MAXM];
  int head[MAXN], eidx;

  Graph() { init(); }
  inline void init() { memset(head, -1, sizeof head), eidx = 1; }

  inline void AddEdge(int from, int to) {
    edges[++eidx] = (Edge){ head[from], to }, head[from] = eidx;
  }
} G, D;

namespace SCC {
  int dfn[MAXN], low[MAXN], clk;
  int stk[MAXN], Idx[MAXN], cnt, top;

  void tarjan(int u) {
    dfn[u] = low[u] = ++clk, stk[++top] = u;
    for (int v, i = G.head[u]; ~i; i = G.edges[i].nxt) {
      if (!dfn[v = G.edges[i].to])
        tarjan(v), low[u] = min(low[u], low[v]);
      else if (!Idx[v])
        low[u] = min(low[u], dfn[v]);
    }
    if (dfn[u] == low[u]) {
      ++cnt;
      while (true) {
        int x = stk[top--];
        Idx[x] = cnt, W[cnt] += A[x];
        if (x == u) break;
      }
    }
  }

  void solve() {
    clk = top = cnt = 0;
    for (int u = 1; u <= n; ++u)
      if (!dfn[u]) tarjan(u);
    for (int u = 1; u <= n; ++u)
      for (int i = G.head[u]; ~i; i = G.edges[i].nxt) {
        int v = G.edges[i].to, su = Idx[u], sv = Idx[v];
        if (su != sv)
          D.AddEdge(su, sv), ++deg[sv];
      }
  }
}

void Toposort() {
  static int Q[MAXN], head, tail;
  Q[head = 1] = tail = 0;
  for (int u = 1; u <= SCC::cnt; ++u)
    if (deg[u] == 0) Q[++tail] = u, f[u] = W[u];
  while (head <= tail) {
    int u = Q[head++];
    for (int v, i = D.head[u]; ~i; i = D.edges[i].nxt) {
      v = D.edges[i].to, f[v] = max(f[v], f[u] + W[v]);
      if (!(--deg[v])) Q[++tail] = v;
    }
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(m);
  for (int i = 1; i <= n; ++i) read(A[i]);
  for (int u, v, i = 1; i <= m; ++i)
    read(u), read(v), G.AddEdge(u, v);

  SCC::solve(), Toposort();
  int ans = -INF;
  for (int u = 1; u <= SCC::cnt; ++u)
    ans = max(ans, f[u]);

  printf("%d\n", ans);
  return 0;
}
