// Luogu P3379
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

const int MAXN = 5e5 + 5, MAXM = 5e5 + 5, LOG = 21;

int n, q, rt;

namespace Graph {
  struct Edge { int nxt, to; } edges[MAXM << 1];
  int head[MAXN], eidx;

  inline void init() { memset(head, -1, sizeof head), eidx = 1; }
  inline void AddEdge(int from, int to) {
    edges[++eidx] = (Edge){ head[from], to }, head[from] = eidx;
  }

  int lg2[MAXN], Mn[LOG][MAXN << 1];
  int depth[MAXN], elr[MAXN], ielr[MAXN << 1], elk;

  void dfs(int u, int fa) {
    depth[u] = depth[fa] + 1;
    ielr[elr[u] = ++elk] = u;
    for (int v, i = head[u]; ~i; i = edges[i].nxt)
      if ((v = edges[i].to) != fa)
        dfs(v, u), ielr[++elk] = u;
  }

  inline int cmp(const int& x, const int& y) {
    return (depth[x] < depth[y])? x: y;
  }

  void build(int rt = 1) {
    dfs(rt, 0);
    for (int i = 1; i <= elk; ++i) Mn[0][i] = ielr[i];
    for (int j = 1; (1 << j) <= elk; ++j)
      for (int i = 1; i + (1 << j) - 1 <= elk; ++i)
        Mn[j][i] = cmp(Mn[j - 1][i], Mn[j - 1][i + (1 << (j-1))]);
    lg2[1] = 0;
    for (int i = 2; i <= elk; ++i) lg2[i] = lg2[i / 2] + 1;
  }

  inline int LCA(const int& u, const int& v) {
    int k, L = elr[u], R = elr[v];
    if (L > R) swap(L, R);
    return k = lg2[R - L + 1], cmp(Mn[k][L], Mn[k][R - (1 << k) + 1]);
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  Graph::init();

  read(n), read(q), read(rt);
  for (int u, v, i = 1; i < n; ++i)
    read(u), read(v), Graph::AddEdge(u, v), Graph::AddEdge(v, u);

  Graph::build(rt);
  for (int u, v; q; --q)
    read(u), read(v), printf("%d\n", Graph::LCA(u, v));
  return 0;
}
