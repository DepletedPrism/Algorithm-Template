// Luogu P3387
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

  inline int gc() {
    return p1 == p2 &&
        (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++;
  }
  template<typename T> inline void read(T& x) {
    x = 0; int f = 0, ch = gc();
    while (!isdigit(ch)) f |= ch == '-', ch = gc();
    while (isdigit(ch)) x = x * 10 + ch - '0', ch = gc();
    if (f) x = -x;
  }
}
using IO::read;

const int MAXN = 1e4 + 5, MAXM = 1e5 + 5;

int n, m;
int val[MAXN];

namespace Graph {
  struct Edge {
    int nxt, to;;
  } edges[MAXM];

  int head[MAXN], eidx;

  inline void init() {
    memset(head, -1, sizeof(head)), eidx = 1;
  }
  inline void addEdge(int u, int v) {
    edges[++eidx] = (Edge){ head[u], v }, head[u] = eidx;
  }
}

namespace SCC {
  using namespace Graph;
  vector<int> G[MAXN];
  int dfn[MAXN], low[MAXN], clk;
  int f[MAXN], deg[MAXN], SCCval[MAXN], SCCidx[MAXN], idx;

  void tarjan(int u) {
    static int stk[MAXN], top;
    low[u] = dfn[u] = ++clk;
    stk[++top] = u;
    for (int i = head[u]; ~i; i = edges[i].nxt) {
      int v = edges[i].to;
      if (!dfn[v])
        tarjan(v), low[u] = min(low[u], low[v]);
      else if (!SCCidx[v])
        low[u] = min(low[u], dfn[v]);
    }
    if (dfn[u] == low[u]) {
      ++idx;
      while (true) {
        int x = stk[top--];
        SCCidx[x] = idx, SCCval[idx] += val[x];
        if (x == u) break;
      }
    }
  }

  void build() {
    for (int u = 1; u <= n; ++u)
      if (!dfn[u]) tarjan(u);
    for (int u = 1; u <= n; ++u) {
      for (int i = Graph::head[u]; ~i; i = Graph::edges[i].nxt) {
        int su = SCCidx[u], sv = SCCidx[Graph::edges[i].to];
        if (su != sv)
          G[su].push_back(sv), ++deg[sv];
      }
    }
  }

  int toposort() {
    static int Q[MAXN], head, tail;
    int ret = 0;
    Q[head = 1] = tail = 0;
    for (int u = 1; u <= idx; ++u)
      if (!deg[u]) Q[++tail] = u, f[u] = SCCval[u];
    while (head <= tail) {
      int u = Q[head++];
      for (const int& v: G[u]) {
        f[v] = max(f[v], f[u] + SCCval[v]);
        if (--deg[v] == 0)
          Q[++tail] = v;
      }
    }
    for (int u = 1; u <= idx; ++u)
      ret = max(ret, f[u]);
    return ret;
  }
}


int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  Graph::init();

  read(n), read(m);
  for (int i = 1; i <= n; ++i)
    read(val[i]);
  for (int u, v, i = 1; i <= m; ++i) {
    read(u), read(v);
    Graph::addEdge(u, v);
  }
  SCC::build();
  printf("%d\n", SCC::toposort());
  return 0;
}
