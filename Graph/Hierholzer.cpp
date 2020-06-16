// UOJ #117
// DeP
#include <cctype>
#include <cstdio>
#include <cstring>
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

template<typename T> inline T Abs(const T& x) { return x < 0? -x: x; }

const int MAXN = 1e5 + 5, MAXM = 2e5 + 5;

int t, n, m;
int stk[MAXM], top;

namespace Graph {
  struct Edge { int nxt, to, idx; } edges[MAXM << 1];
  int head[MAXN], eidx;

  inline void init() { memset(head, -1, sizeof head), eidx = 1; }
  inline void AddEdge(int from, int to, int i) {
    edges[++eidx] = (Edge){ head[from], to, i }, head[from] = eidx;
  }
}

namespace Euler1 {
  using namespace Graph;
  bool vis[MAXM];
  int deg[MAXN], cur[MAXN];

  void dfs(int u, const int& w) {
    vis[Abs(w)] = true;
    for (int& i = cur[u]; ~i; ) {
      int idx = edges[i].idx, v = edges[i].to;
      i = edges[i].nxt;
      if (!vis[Abs(idx)]) dfs(v, idx);
    }
    stk[++top] = w;
  }

  bool solve() {
    memcpy(cur, head, sizeof cur);
    for (int u = 1; u <= n; ++u) {
      for (int i = head[u]; ~i; i = edges[i].nxt) ++deg[u];
      if (deg[u] & 1) return false;
    }
    // Hierholzer
    for (int u = 1; u <= n; ++u)
      if (~head[u]) { dfs(u, 0); break; }
    return true;
  }
}

namespace Euler2 {
  using namespace Graph;
  int in[MAXN], out[MAXN], cur[MAXN];

  void dfs(int u, const int& w) {
    for (int& i = cur[u]; ~i; ) {
      int idx = edges[i].idx, v = edges[i].to;
      i = edges[i].nxt, dfs(v, idx);
    }
    stk[++top] = w;
  }

  bool solve() {
    memcpy(cur, head, sizeof cur);
    for (int u = 1; u <= n; ++u)
      for (int i = head[u]; ~i; i = edges[i].nxt)
        ++in[edges[i].to], ++out[u];
    for (int u = 1; u <= n; ++u)
      if (in[u] != out[u]) return false;
    // Hierholzer
    for (int u = 1; u <= n; ++u)
      if (~head[u]) { dfs(u, 0); break; }
    return true;
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  Graph::init();

  read(t), read(n), read(m);
  for (int u, v, i = 1; i <= m; i++) {
    read(u), read(v);
    Graph::AddEdge(u, v, i);
    if (t == 1) Graph::AddEdge(v, u, -i);
  }

  bool flag = (t == 1)? Euler1::solve(): Euler2::solve();

  if (!flag || (m > 0 && top-1 < m))
    puts("NO");
  else {
    puts("YES");
    for (int i = top-1; i > 0; --i)
      printf("%d%c", stk[i], " \n"[i == 1]);
  }
  return 0;
}
